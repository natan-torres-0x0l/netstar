#include "netstar-filter.h"

#include <netstar-forward.h>

#include <netstar-utils.h>
#include <netstar.h>

#include <strings/strings.h>
#include <string.h>

#include <ctype.h>

#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>


static const char *netstar_filter_protocols[] = {
// @ netstar-ethernethdr
// "ethernet",

// @ netstar-arphdr
  "arp", "arp-request", "arp-reply",

// @ netstar-iphdr | netstar-ipv6hdr
  "ip", "ipv4", "ipv6",

// @ netstar-icmphdr | netstar-icmpv6hdr
  "icmp", "icmpv4", "icmpv6",

// @ netstar-ndphdr
  "ndp", "ndp-solicit", "ndp-advert",

// @ netstar-tcphdr
  "tcp", "tcpv4", "tcpv6",

// @ netstar-udphdr
  "udp", "udpv4", "udpv6",

  NULL
};

static const char *netstar_filter_reserved_keywords[] = {
// @ netstar-capture-packet attributes
  "mac", "smac", "dmac", "addr", "addr4", "addr6", "saddr", "daddr", "sport", "dport",
  NULL
};

typedef enum {
  NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD = 5,
  NETSTAR_FILTER_TOKEN_CLASS_PROTOCOL         = 7,

  NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802       = 6,
  NETSTAR_FILTER_TOKEN_CLASS_INETV4           = 4,
  NETSTAR_FILTER_TOKEN_CLASS_INETV6           = 16,

  NETSTAR_FILTER_TOKEN_CLASS_PORT             = 2,

  NETSTAR_FILTER_TOKEN_CLASS_INVALID          = -1
} netstar_filter_token_class_t;


static bool
netstar_filter_symbol_reserved_keyword(const char *symbol) {
  const char **keywords = NULL;

  for (keywords = netstar_filter_reserved_keywords; *keywords; keywords++)
    if (string_equals(*keywords, symbol, true))
      return true;

  return false;
}

static netstar_forward_protocol_t
netstar_filter_protocol(const char *protocol) {
  if (string_equals(protocol, "arp", true))
    return NETSTAR_FORWARD_ARP;
  
  if (string_equals(protocol, "arp-request", true))
    return NETSTAR_FORWARD_ARP_REQUEST;
  
  if (string_equals(protocol, "arp-reply", true))
    return NETSTAR_FORWARD_ARP_REPLY;

  if (string_equals(protocol, "ip", true))
    return NETSTAR_FORWARD_IP;

  if (string_equals(protocol, "ipv4", true))
    return NETSTAR_FORWARD_IPV4;

  if (string_equals(protocol, "ipv6", true))
    return NETSTAR_FORWARD_IPV6;

  if (string_equals(protocol, "icmp", true))
    return NETSTAR_FORWARD_ICMP;

  if (string_equals(protocol, "icmpv4", true))
    return NETSTAR_FORWARD_ICMPV4;

  if (string_equals(protocol, "icmpv6", true))
    return NETSTAR_FORWARD_ICMPV6;

  if (string_equals(protocol, "ndp", true))
    return NETSTAR_FORWARD_NDP;

  if (string_equals(protocol, "ndp-solicit", true))
    return NETSTAR_FORWARD_NDP_SOLICIT;

  if (string_equals(protocol, "ndp-advert", true))
    return NETSTAR_FORWARD_NDP_ADVERT;

  if (string_equals(protocol, "tcp", true))
    return NETSTAR_FORWARD_TCP;

  if (string_equals(protocol, "tcpv4", true))
    return NETSTAR_FORWARD_TCPV4;

  if (string_equals(protocol, "tcpv6", true))
    return NETSTAR_FORWARD_TCPV6;

  if (string_equals(protocol, "udp", true))
    return NETSTAR_FORWARD_UDP;

  if (string_equals(protocol, "udpv4", true))
    return NETSTAR_FORWARD_UDPV4;

  if (string_equals(protocol, "udpv6", true))
    return NETSTAR_FORWARD_UDPV6;

  return NETSTAR_FORWARD_ALL;
}

static bool
netstar_filter_symbol_protocol(const char *symbol) {
  const char **protocols = NULL;

  for (protocols = netstar_filter_protocols; *protocols; protocols++)
    if (string_equals(*protocols, symbol, true))
      return true;

  return false;
}

static inline bool
netstar_filter_symbol_macieee802(const char *symbol) {
  return network_macaddr_parse(NULL, symbol) == 1;
}

static inline bool
netstar_filter_symbol_ipaddr4(const char *symbol) {
  return network_ipaddr4_parse(NULL, symbol) == 1;
}

static inline bool
netstar_filter_symbol_ipaddr6(const char *symbol) {
  return network_ipaddr6_parse(NULL, symbol) == 1;
}

static bool
netstar_filter_symbol_port(const char *symbol) {
  while (*symbol && isdigit(*symbol))
    symbol++;

  return !*symbol;
}

static netstar_filter_token_class_t
netstar_filter_symbol_token_class(const char *symbol) {
  if (netstar_filter_symbol_reserved_keyword(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD;

  if (netstar_filter_symbol_port(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_PORT;

  if (netstar_filter_symbol_protocol(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_PROTOCOL;

  if (netstar_filter_symbol_macieee802(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802;

  if (netstar_filter_symbol_ipaddr4(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_INETV4;

  if (netstar_filter_symbol_ipaddr6(symbol))
    return NETSTAR_FILTER_TOKEN_CLASS_INETV6;

  return NETSTAR_FILTER_TOKEN_CLASS_INVALID;
}

static uint8_t
netstar_filter_token_class(netstar_filter_compiler_t *state, uintptr_t rx) {
  if (*(uint8_t *)rx != NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD)
    return *(uint8_t *)rx;

  if (string_endswith((const char *)(uint8_t *)rx+1, "mac", true))
    return NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802;

// if (string_equals((const char *)(uint8_t *)rx+1, "addr6", true))
//   return NETSTAR_FILTER_TOKEN_CLASS_INETV6;  

  if (string_equals((const char *)(uint8_t *)rx+1, "addr6", true))
    return NETSTAR_FILTER_TOKEN_CLASS_INETV6;  

  if (string_search((const char *)(uint8_t *)rx+1, "addr", true)) {
    if (state->packet->type & NETSTAR_FORWARD_IPV6)
      return NETSTAR_FILTER_TOKEN_CLASS_INETV6;

// if (state->packet->type & NETSTAR_FORWARD_IPV4)
    return NETSTAR_FILTER_TOKEN_CLASS_INETV4;
  }

  if (string_endswith((const char *)(uint8_t *)rx+1, "port", true))
    return NETSTAR_FILTER_TOKEN_CLASS_PORT;

  return *(uint8_t *)rx;
}


static inline bool
netstar_filter_symbol(netstar_filter_compiler_t *state, const char *symbol) {
  size_t length = string_length(symbol);
  uintptr_t rx;

  state->token_value = (uintptr_t)(buffer_length(state->data)+2);
// state->token_value = (buffer_length(state->data)+1)*-1;

  switch (netstar_filter_symbol_token_class(symbol)) {
    case NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD: {
      rx = NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD;
      buffer_append(state->data, &rx, sizeof(uint8_t));
      buffer_append(state->data, symbol, length);
      buffer_append(state->data, "\0", 1);

      break;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_PROTOCOL: {
      rx = NETSTAR_FILTER_TOKEN_CLASS_PROTOCOL;
      buffer_append(state->data, &rx, sizeof(uint8_t));
      buffer_append(state->data, symbol, length);
      buffer_append(state->data, "\0", 1);

      break;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802: {
      network_macaddr_t mac = {0};

      rx = NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802;
      buffer_append(state->data, &rx, sizeof(uint8_t));

      network_macaddr_parse(&mac, symbol);
      buffer_append(state->data, &mac, NETWORK_MACADDR_SIZE);

      break;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_INETV4: {
      network_ipaddr4_t addr = {0};

      rx = NETSTAR_FILTER_TOKEN_CLASS_INETV4;
      buffer_append(state->data, &rx, sizeof(uint8_t));

      network_ipaddr4_parse(&addr, symbol);
      buffer_append(state->data, &addr, NETWORK_IPADDR4_SIZE);

      break;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_INETV6: {
      network_ipaddr6_t addr = {0};

      rx = NETSTAR_FILTER_TOKEN_CLASS_INETV6;
      buffer_append(state->data, &rx, sizeof(uint8_t));

      network_ipaddr6_parse(&addr, symbol);
      buffer_append(state->data, &addr, NETWORK_IPADDR6_SIZE);

      break;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_PORT: {
      uint16_t port = (uint16_t)netstar_utils_string_number(symbol);

      rx = NETSTAR_FILTER_TOKEN_CLASS_PORT;
      buffer_append(state->data, &rx, sizeof(uint8_t));

      buffer_append(state->data, &port, sizeof(uint16_t));

      break;
    }

    default:
      return false;
  }

  state->shift += length;
  state->source += length;

  return true;
}

#define NETSTAR_FILTER_SYMBOL_LENGTH 256

static bool
netstar_filter_symbol_new(netstar_filter_compiler_t *state, char *symbol, size_t length) {
  const char *source = state->source;

  while (*source && (isalnum(*source) || *source == ':' || *source == '.' || *source == '-')) {
    if (*source && !length)
      return false;

    *symbol++ = *source++;
    length--;
  }

  if (source == state->source || netstar_filter_symbol_token_class(symbol) == NETSTAR_FILTER_TOKEN_CLASS_INVALID)
    return false;

  return true;
}

static netstar_filter_token_t
netstar_filter_token_next(netstar_filter_compiler_t *state) {
  for (;;) {
    switch (*state->source) {
      case ' ': case '\t': case '\r': {
        state->source++, state->shift++;
        continue;
      }

      case '\n': {
        state->source++, state->line++, state->shift = 1;
        continue;
      }

      case '\0': {
        return NETSTAR_FILTER_TOKEN_END;
      }

      case '|': {
        if (*(state->source+1) == '|') {
          state->source += 2, state->shift += 2;
          return NETSTAR_FILTER_TOKEN_OR;
        }

        return NETSTAR_FILTER_TOKEN_INVALID;
      }

      case '&': {
        if (*(state->source+1) == '&') {
          state->source += 2, state->shift += 2;
          return NETSTAR_FILTER_TOKEN_AND;
        }

        return NETSTAR_FILTER_TOKEN_INVALID;
      }

      case '=': {
        if (*(state->source+1) == '=') {
          state->source += 2, state->shift += 2; 
          return NETSTAR_FILTER_TOKEN_EQUALS;
        }

        return NETSTAR_FILTER_TOKEN_INVALID;
      }

      case '!': {
        if (*(state->source+1) == '=') {
          state->source += 2, state->shift += 2; 
          return NETSTAR_FILTER_TOKEN_NOTEQUALS;
        }

        state->source++, state->shift++;
        return NETSTAR_FILTER_TOKEN_NOT;
      }

      case '>': {
        if (*(state->source+1) == '=') {
          state->source += 2, state->shift += 2; 
          return NETSTAR_FILTER_TOKEN_GREATEROREQUALS;
        }

        state->source++, state->shift++;
        return NETSTAR_FILTER_TOKEN_GREATER;
      }

      case '<': {
        if (*(state->source+1) == '=') {
          state->source += 2, state->shift += 2; 
          return NETSTAR_FILTER_TOKEN_LESSOREQUALS;
        }

        state->source++, state->shift++;
        return NETSTAR_FILTER_TOKEN_LESS;
      }

      case '(': {
        state->source++, state->shift++;
        return NETSTAR_FILTER_TOKEN_STARTBRACE;
      }

      case ')': {
        state->source++, state->shift++;
        return NETSTAR_FILTER_TOKEN_ENDBRACE;
      }

      default: {
        char identifier[NETSTAR_FILTER_SYMBOL_LENGTH] = {0};

        if (!netstar_filter_symbol_new(state, identifier, sizeof(identifier)))
          return NETSTAR_FILTER_TOKEN_INVALID;

        if (netstar_filter_symbol(state, identifier))
          return NETSTAR_FILTER_TOKEN_IDENTIFIER;

     // netstar_filter_syntax_error(state, "unexpected symbol '%c'\r\n\r\n", *state->source);
        return NETSTAR_FILTER_TOKEN_INVALID;
      }
    }
  }

  return NETSTAR_FILTER_TOKEN_INVALID;
}

static void
netstar_filter_token_match(netstar_filter_compiler_t *state, netstar_filter_token_t token) {
  if (state->token != token) {
    state->token = NETSTAR_FILTER_TOKEN_INVALID;
    return;
  }

  state->token = netstar_filter_token_next(state);
}

static void
netstar_filter_expression(netstar_filter_compiler_t *state, netstar_filter_token_t token_level) {
  uintptr_t bytecode;

  switch (state->token) {
    case NETSTAR_FILTER_TOKEN_STARTBRACE: {
      netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_STARTBRACE);
      netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_OR);
      netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_ENDBRACE);

      break;
    }

    case NETSTAR_FILTER_TOKEN_NOT: {
      netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_NOT);
      if (state->token == NETSTAR_FILTER_TOKEN_IDENTIFIER) {
        bytecode = NETSTAR_FILTER_INSTRUCTION_MOV;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        bytecode = state->token_value;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_IDENTIFIER);

        bytecode = NETSTAR_FILTER_INSTRUCTION_NOT;
        buffer_append(state->text, &bytecode, sizeof(bytecode));
      } else {
        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_IDENTIFIER);

        bytecode = NETSTAR_FILTER_INSTRUCTION_NOT;
        buffer_append(state->text, &bytecode, sizeof(bytecode));
      }

      break;
    }

    case NETSTAR_FILTER_TOKEN_IDENTIFIER: {
      bytecode = NETSTAR_FILTER_INSTRUCTION_MOV;
      buffer_append(state->text, &bytecode, sizeof(bytecode));

      bytecode = state->token_value;
      buffer_append(state->text, &bytecode, sizeof(bytecode));

      netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_IDENTIFIER);

      break;
    }

    default:
      state->token = NETSTAR_FILTER_TOKEN_INVALID;
      return;
  }

  while (state->token >= token_level)
    switch (state->token) {
      case NETSTAR_FILTER_TOKEN_OR: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_OR);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_AND);

        bytecode = NETSTAR_FILTER_INSTRUCTION_OR;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_AND: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_AND);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_AND);

        bytecode = NETSTAR_FILTER_INSTRUCTION_AND;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_EQUALS: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_EQUALS;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_NOTEQUALS: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_NOTEQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_NOTEQUALS;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_GREATER: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_GREATER);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_GREATER;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_GREATEROREQUALS: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_GREATEROREQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_GREATEROREQUALS;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_LESS: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_LESS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_LESS;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      case NETSTAR_FILTER_TOKEN_LESSOREQUALS: {
        netstar_filter_token_match(state, NETSTAR_FILTER_TOKEN_LESSOREQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_PUSH;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_EQUALS);

        bytecode = NETSTAR_FILTER_INSTRUCTION_LESSOREQUALS;
        buffer_append(state->text, &bytecode, sizeof(bytecode));

        break;
      }

      default:
        state->token = NETSTAR_FILTER_TOKEN_INVALID;
     // netstar_filter_syntax_error(state, "malformed expression\r\n\r\n");
        return;
    }
}

static int
netstar_filter_syntax(netstar_filter_compiler_t *state) {
  uintptr_t bytecode;

  for (state->token = netstar_filter_token_next(state); state->token != NETSTAR_FILTER_TOKEN_END;) {
    netstar_filter_expression(state, NETSTAR_FILTER_TOKEN_OR);

    if (state->token == NETSTAR_FILTER_TOKEN_INVALID) {
   // netstar_filter_syntax_error(state, "malformed expression\r\n\r\n");
      buffer_truncate(state->text, 0);
      return -1;
    }
  }

  bytecode = NETSTAR_FILTER_INSTRUCTION_END;
  buffer_append(state->text, &bytecode, sizeof(bytecode));

  return 1;
}


static bool
netstar_filter_instruction_evaluated(uintptr_t rx) {
  return rx == true || rx == false;
}

static inline void
netstar_filter_instruction_adr(netstar_filter_compiler_t *state, uintptr_t *rx) {
  uintptr_t addr = *rx-2;
  *rx = (uintptr_t)&(((uint8_t *)buffer_ptr(state->data))[addr]);
}

static uintptr_t
netstar_filter_instruction_tst(netstar_filter_compiler_t *state, uintptr_t rx) {
  if (netstar_filter_instruction_evaluated(rx))
    return rx;

  netstar_filter_instruction_adr(state, &rx);

  switch ((netstar_filter_token_class_t)*(uint8_t *)rx) {
    case NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD: {
      const char *keyword = (const char *)((uint8_t *)rx+1);

      if (string_equals(keyword, "mac", true))
        return !network_macaddr_compare(&state->packet->layer2.smac, &state->netstar->managed.iface->mac) || !network_macaddr_compare(&state->packet->layer2.dmac, &state->netstar->managed.iface->mac);
   
      if (string_equals(keyword, "addr", true)) {
        if (state->packet->type & NETSTAR_FORWARD_IPV4)
          return !network_ipaddr4_compare(&state->packet->layer3.saddr.v4, &state->netstar->managed.iface->addr) || !network_ipaddr4_compare(&state->packet->layer3.daddr.v4, &state->netstar->managed.iface->addr);

        if (state->packet->type & NETSTAR_FORWARD_IPV6)
          return !network_ipaddr6_compare(&state->packet->layer3.saddr.v6, &state->netstar->managed.iface->addr6) || !network_ipaddr6_compare(&state->packet->layer3.daddr.v6, &state->netstar->managed.iface->addr6);
      }

      if (string_equals(keyword, "addr4", true) && state->packet->type & NETSTAR_FORWARD_IPV4)
        return !network_ipaddr4_compare(&state->packet->layer3.saddr.v4, &state->netstar->managed.iface->addr) || !network_ipaddr4_compare(&state->packet->layer3.daddr.v4, &state->netstar->managed.iface->addr);

      if (string_equals(keyword, "addr6", true) && state->packet->type & NETSTAR_FORWARD_IPV6)
        return !network_ipaddr6_compare(&state->packet->layer3.saddr.v6, &state->netstar->managed.iface->addr6) || !network_ipaddr6_compare(&state->packet->layer3.daddr.v6, &state->netstar->managed.iface->addr6);

      return false;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_PROTOCOL: {
      netstar_forward_protocol_t protocol = netstar_filter_protocol((const char *)((uint8_t *)rx+1));
      return state->packet->type & protocol;
    }

    case NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802: {
      network_macaddr_t *mac = (network_macaddr_t *)((uint8_t *)rx+1);
      return !network_macaddr_compare(&state->packet->layer2.smac, mac) || !network_macaddr_compare(&state->packet->layer2.dmac, mac);
    }

    case NETSTAR_FILTER_TOKEN_CLASS_INETV4: {
      network_ipaddr4_t *addr = (network_ipaddr4_t *)((uint8_t *)rx+1);
      return (state->packet->type & NETSTAR_FORWARD_IPV4) && (!network_ipaddr4_compare(&state->packet->layer3.saddr.v4, addr) || !network_ipaddr4_compare(&state->packet->layer3.daddr.v4, addr));
    }

    case NETSTAR_FILTER_TOKEN_CLASS_INETV6: {
      network_ipaddr6_t *addr = (network_ipaddr6_t *)((uint8_t *)rx+1);
      return (state->packet->type & NETSTAR_FORWARD_IPV6) && (!network_ipaddr6_compare(&state->packet->layer3.saddr.v6, addr) || !network_ipaddr6_compare(&state->packet->layer3.daddr.v6, addr));
    }

    case NETSTAR_FILTER_TOKEN_CLASS_PORT: {
      uint16_t *port = (uint16_t *)((uint8_t *)rx+1);
      return state->packet->layer4.sport == *port || state->packet->layer4.dport == *port;
    }
  }

  return rx;
}

static uintptr_t
netstar_filter_instruction_or(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_tst(state, r0) || netstar_filter_instruction_tst(state, r1);
}

static uintptr_t
netstar_filter_instruction_and(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_tst(state, r0) && netstar_filter_instruction_tst(state, r1);
}

static uintptr_t
netstar_filter_instruction_not(netstar_filter_compiler_t *state, uintptr_t rx) {
  return !netstar_filter_instruction_tst(state, rx);
}

static void *
netstar_filter_instruction_ldr(netstar_filter_compiler_t *state, uintptr_t rx) {
  if (string_equals((const char *)rx, "mac", true))
    return &state->netstar->managed.iface->mac;

  if (string_equals((const char *)rx, "addr", true)) {
    if (state->packet->type & NETSTAR_FORWARD_IPV6)
      return &state->netstar->managed.iface->addr6;
   
// if (state->packet->type & NETSTAR_FORWARD_IPV4)
    return &state->netstar->managed.iface->addr;
  }

  if (string_equals((const char *)rx, "addr4", true))
    return &state->netstar->managed.iface->addr;

  if (string_equals((const char *)rx, "addr6", true))
    return &state->netstar->managed.iface->addr6;

  if (string_equals((const char *)rx, "smac", true))
    return &state->packet->layer2.smac;

  if (string_equals((const char *)rx, "dmac", true))
    return &state->packet->layer2.dmac;

  if (string_equals((const char *)rx, "saddr", true))
    return &state->packet->layer3.saddr;

  if (string_equals((const char *)rx, "daddr", true))
    return &state->packet->layer3.daddr;

  if (string_equals((const char *)rx, "sport", true))
    return &state->packet->layer4.sport;

  if (string_equals((const char *)rx, "dport", true))
    return &state->packet->layer4.dport;

  return (void *)rx;
}

static inline int
netstar_filter_instruction_network_macaddr_cmp(const void *left, const void *right) {
  return network_macaddr_compare((network_macaddr_t *)left, (network_macaddr_t *)right);
}

static inline int
netstar_filter_instruction_network_ipaddr4_cmp(const void *left, const void *right) {
  return network_ipaddr4_compare((network_ipaddr4_t *)left, (network_ipaddr4_t *)right);
}

static inline int
netstar_filter_instruction_network_ipaddr6_cmp(const void *left, const void *right) {
  return network_ipaddr6_compare((network_ipaddr6_t *)left, (network_ipaddr6_t *)right);
}

static inline int
netstar_filter_instruction_port_cmp(const void *left, const void *right) {
  if (*(uint16_t *)left > *(uint16_t *)right)
    return 1;
  else if (*(uint16_t *)left < *(uint16_t *)right)
    return -1;

  return 0;
}


typedef int (*netstar_filter_instruction_compare_t)(const void *, const void *);


static inline bool
netstar_filter_instruction_ecmp(const void *left, const void *right, netstar_filter_instruction_compare_t compare, netstar_filter_instruction_t instruction) {
  int comparation = compare(left, right);

// == | !=
  if (instruction == NETSTAR_FILTER_INSTRUCTION_NOTEQUALS && comparation != 0)
    return true;
  if (instruction == NETSTAR_FILTER_INSTRUCTION_EQUALS && !comparation)
    return true;

// > | >=
  if (instruction == NETSTAR_FILTER_INSTRUCTION_GREATER && comparation == 1)
    return true;
  if (instruction == NETSTAR_FILTER_INSTRUCTION_GREATEROREQUALS && comparation >= 0)
    return true;

// < | <=
  if (instruction == NETSTAR_FILTER_INSTRUCTION_LESS && comparation == -1)
    return true;
  if (instruction == NETSTAR_FILTER_INSTRUCTION_LESSOREQUALS && comparation <= 0)
    return true;

  return false;
}

static uintptr_t
netstar_filter_instruction_cmp(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1, netstar_filter_instruction_t instruction) {
  if (!netstar_filter_instruction_evaluated(r0) && !netstar_filter_instruction_evaluated(r1)) {
    uint8_t r2, r3, r4, r5;

// @ load stack address into the following registers r0 and r1
    netstar_filter_instruction_adr(state, &r0);
    netstar_filter_instruction_adr(state, &r1);

// @ dereference and load the first byte which is the class of the token that was stored on the stack
    r4 = *(uint8_t *)r0;
    r5 = *(uint8_t *)r1;

// @ check if the actual class of both addresses are same to continue comparison
    if ((r2 = netstar_filter_token_class(state, r0)) != (r3 = netstar_filter_token_class(state, r1)))
      return false;

// @ dereference the pointer stored in r0 register and move the value to it
    r0 = (uintptr_t)(uint8_t *)r0+1;

// @ check if the stored class of the stack is a named register(reserved keyword) and then load the stored value of that register
    if (r4 == NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD)
      r0 = (uintptr_t)netstar_filter_instruction_ldr(state, r0);

// @ dereference the pointer stored in r1 register and move the value to it
    r1 = (uintptr_t)(uint8_t *)r1+1;

// @ check if the stored class of the stack is a named register(reserved keyword) and then load the stored value of that register
    if (r5 == NETSTAR_FILTER_TOKEN_CLASS_RESERVED_KEYWORD)
      r1 = (uintptr_t)netstar_filter_instruction_ldr(state, r1);

    switch ((netstar_filter_token_class_t)r2) {
      case NETSTAR_FILTER_TOKEN_CLASS_MACIEEE802: {
        network_macaddr_t *left = (network_macaddr_t *)r1;
        network_macaddr_t *right = (network_macaddr_t *)r0;

        return netstar_filter_instruction_ecmp(left, right, netstar_filter_instruction_network_macaddr_cmp, instruction);
      }

      case NETSTAR_FILTER_TOKEN_CLASS_INETV4: {
        network_ipaddr4_t *left = (network_ipaddr4_t *)r1;
        network_ipaddr4_t *right = (network_ipaddr4_t *)r0;

        return netstar_filter_instruction_ecmp(left, right, netstar_filter_instruction_network_ipaddr4_cmp, instruction);
      }

      case NETSTAR_FILTER_TOKEN_CLASS_INETV6: {
        network_ipaddr6_t *left = (network_ipaddr6_t *)r1;
        network_ipaddr6_t *right = (network_ipaddr6_t *)r0;

        return netstar_filter_instruction_ecmp(left, right, netstar_filter_instruction_network_ipaddr6_cmp, instruction);
      }

      case NETSTAR_FILTER_TOKEN_CLASS_PORT: {
        uint16_t *left = (uint16_t *)r1;
        uint16_t *right = (uint16_t *)r0;

        return netstar_filter_instruction_ecmp(left, right, netstar_filter_instruction_port_cmp, instruction);
      }
    }

    return false;
  } else if (!netstar_filter_instruction_evaluated(r0)) {
    r0 = netstar_filter_instruction_tst(state, r0);
  } else if (!netstar_filter_instruction_evaluated(r1)) {
    r1 = netstar_filter_instruction_tst(state, r1);
  }

  return r0 == r1;
}

static uintptr_t
netstar_filter_instruction_equals(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_EQUALS);
}

static uintptr_t
netstar_filter_instruction_notequals(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_NOTEQUALS);
}

static uintptr_t
netstar_filter_instruction_greater(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_GREATER);
}

static uintptr_t
netstar_filter_instruction_greaterorequals(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_GREATEROREQUALS);
}

static uintptr_t
netstar_filter_instruction_less(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_LESS);
}

static uintptr_t
netstar_filter_instruction_lessorequals(netstar_filter_compiler_t *state, uintptr_t r0, uintptr_t r1) {
  return netstar_filter_instruction_cmp(state, r0, r1, NETSTAR_FILTER_INSTRUCTION_LESSOREQUALS);
}

static inline void
netstar_filter_instruction_mov(netstar_filter_compiler_t *state, uintptr_t *rx) {
  *rx = *state->r15++;
}

static inline void
netstar_filter_instruction_push(netstar_filter_compiler_t *state, uintptr_t rx) {
  buffer_append(state->stack, &rx, sizeof(uintptr_t));
}

static inline void
netstar_filter_instruction_pop(netstar_filter_compiler_t *state, uintptr_t *rx) {
  *rx = *(uintptr_t *)(buffer_ptr(state->stack)+(buffer_length(state->stack)-sizeof(uintptr_t)));
  buffer_truncate(state->stack, buffer_length(state->stack)-sizeof(uintptr_t));
}

static void
netstar_filter_assembly(netstar_filter_compiler_t *state) {
  for (;;) {
    uintptr_t bytecode = *state->r15++;

    switch (bytecode) {
      case NETSTAR_FILTER_INSTRUCTION_MOV: {
        netstar_filter_instruction_mov(state, &state->r0);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_PUSH: {
        netstar_filter_instruction_push(state, state->r0);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_OR: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_or(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_AND: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_and(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_NOT: {
        state->r0 = netstar_filter_instruction_not(state, state->r0);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_EQUALS: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_equals(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_NOTEQUALS: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_notequals(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_GREATER: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_greater(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_GREATEROREQUALS: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_greaterorequals(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_LESS: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_less(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_LESSOREQUALS: {
        netstar_filter_instruction_pop(state, &state->r1);
        state->r0 = netstar_filter_instruction_lessorequals(state, state->r0, state->r1);
        break;
      }

      case NETSTAR_FILTER_INSTRUCTION_END: {
        state->r0 = netstar_filter_instruction_tst(state, state->r0);
        return;
      }
    }
  }
}

bool
netstar_filter_compile(netstar_filter_t *filter, struct netstar_capture_packet *packet) {
  if (!buffer_length(filter->state.text))
    return false;

  filter->state.packet = packet;

  filter->state.r15 = (uintptr_t *)buffer_ptr(filter->state.text);

  filter->state.r0 = 0;
  filter->state.r1 = 0;

  netstar_filter_assembly(&filter->state);

  return filter->state.r0;
}

bool
netstar_filter_initialized(netstar_filter_t *filter) {
  return filter->state.source != NULL;
}

int
netstar_filter_new(netstar_filter_t *filter, netstar_t *netstar, const char *source) {
  if (!*source || !(filter->state.expression = string_new(source)))
    goto _return;

  filter->state.source = filter->state.expression;

  if (!(filter->state.stack = buffer_make(0, NETSTAR_FILTER_STACK_SIZE)))
    goto _return;
  if (!(filter->state.text = buffer_make(0, NETSTAR_FILTER_TEXT_SIZE)))
    goto _return;
  if (!(filter->state.data = buffer_make(0, NETSTAR_FILTER_DATA_SIZE)))
    goto _return;

  filter->state.shift = 1;
  filter->state.line  = 1;

  filter->state.netstar = netstar;

  return netstar_filter_syntax(&filter->state);

_return:
  netstar_filter_free(filter);
  return -1;
}

void
netstar_filter_free(netstar_filter_t *filter) {
  if (filter) {
    buffer_free(filter->state.stack);
    buffer_free(filter->state.text);
    buffer_free(filter->state.data);

    free(filter->state.expression);
  }
}
