#include "resolv.h"
#include "resolv-cache.h"

#include <strings/strings.h>
#include <string.h>

#include <random/random.h>

#include <stdlib.h>
#include <stdio.h>


#define NETWORK_RESOLV_DNS_MESSAGES 5


static struct network_endpoint resolv_server_addr; 


__attribute__((__constructor__))
static void
resolv_initialize(void) {
  network_endpoint_new(&resolv_server_addr, "8.8.8.8" /* 8.8.8.8 | 8.8.4.4 */, NETWORK_DNS_PROTOCOL_PORT); /* Google */
}


typedef enum {
  RESOLV_HOSTENTRY, RESOLV_CNAME, RESOLV_MX, RESOLV_NS, RESOLV_PTR, RESOLV_SOA, RESOLV_TXT,
} resolv_class_type_t;

struct resolv_class {
  char *hostname;
};


static struct resolv_class *
resolv_answers(struct resolv_class *resolv_class, size_t resolv_class_size, resolv_class_type_t resolv_class_type, uint8_t *response, size_t length) {
  struct network_dns_question question = {0};
  network_dns_parser_t *parser = NULL;

  if (!(parser = network_dns_parser_new(response, length)))
    goto _return;

  if (!network_dns_parser_nanswers(parser) || !network_dns_parser_nquestions(parser))
    goto _return;

  network_dns_parser_question(parser, &question);

  if (!(resolv_class = (struct resolv_class *)calloc(1, resolv_class_size)))
    goto _return;
  resolv_class->hostname = string_new(question.name);

  network_dns_parser_answers(parser);

  for (;;) {
    struct network_dns_resource resource = {0};

    if (network_dns_parser_resource(parser, &resource) == -1)
      break;

    switch (resource.rtype) {
      case NETWORK_DNS_TYPE_A: {
        if (resolv_class_type == RESOLV_HOSTENTRY) {
          struct resolv_hostentry *hostentry = (struct resolv_hostentry *)resolv_class;
          struct network_dns_a a = {0};

          if (!(hostentry->addrs = (char **)realloc(hostentry->addrs, (hostentry->addrs_count+1)*sizeof(char *))))
            break;

          network_dns_parser_a(parser, &a);

          hostentry->addrs[hostentry->addrs_count++] = string_new(a.a);
        }

        break;
      }

      case NETWORK_DNS_TYPE_AAAA: {
        if (resolv_class_type == RESOLV_HOSTENTRY) {
          struct resolv_hostentry *hostentry = (struct resolv_hostentry *)resolv_class;
          struct network_dns_aaaa aaaa = {0};

          if (!(hostentry->addrs = (char **)realloc(hostentry->addrs, (hostentry->addrs_count+1)*sizeof(char *))))
            break;

          network_dns_parser_aaaa(parser, &aaaa);

          hostentry->addrs[hostentry->addrs_count++] = string_new(aaaa.aaaa);
        }

        break;
      }

      case NETWORK_DNS_TYPE_CNAME: {
        if (resolv_class_type == RESOLV_HOSTENTRY) {
          struct resolv_hostentry *hostentry = (struct resolv_hostentry *)resolv_class;
          struct network_dns_cname cname = {0};

          if (!(hostentry->names = (char **)realloc(hostentry->names, (hostentry->names_count+1)*sizeof(char *))))
            break;

          network_dns_parser_cname(parser, &cname);

          hostentry->names[hostentry->names_count++] = string_new(cname.cname);
        }
     // @ CNAME class
     //
     // else if (resolv_class_type == RESOLV_CNAME) {
     //
     // }

        break;
      }

      case NETWORK_DNS_TYPE_MX: {
        if (resolv_class_type == RESOLV_MX) {
          struct resolv_mx *resolv_mx = (struct resolv_mx *)resolv_class;
          struct network_dns_mx mx = {0};

          if (!(*((void **)&resolv_mx->mx) = realloc(resolv_mx->mx, (resolv_mx->mx_count+1)*sizeof(*resolv_mx->mx))))
            break;

          network_dns_parser_mx(parser, &mx);

          resolv_mx->mx[resolv_mx->mx_count].preference = mx.preference; 
          resolv_mx->mx[resolv_mx->mx_count++].exchange = string_new(mx.exchange);
        }

        break;
      }

      case NETWORK_DNS_TYPE_NS: {
        if (resolv_class_type == RESOLV_NS) {
          struct resolv_ns *resolv_ns = (struct resolv_ns *)resolv_class;
          struct network_dns_ns ns = {0};

          if (!(resolv_ns->nsnames = (char **)realloc(resolv_ns->nsnames, (resolv_ns->nsnames_count+1)*sizeof(char *))))
            break;

          network_dns_parser_ns(parser, &ns);

          resolv_ns->nsnames[resolv_ns->nsnames_count++] = string_new(ns.nsname);
        }

        break;
      }

      case NETWORK_DNS_TYPE_PTR: {
        if (resolv_class_type == RESOLV_HOSTENTRY) {
          struct resolv_hostentry *hostentry = (struct resolv_hostentry *)resolv_class;
          struct network_dns_ptr ptr = {0};

          if (!(hostentry->names = (char **)realloc(hostentry->names, (hostentry->names_count+1)*sizeof(char *))))
            break;

          network_dns_parser_ptr(parser, &ptr);

          hostentry->names[hostentry->names_count++] = string_new(ptr.ptr);
        }

        break;
      }

      case NETWORK_DNS_TYPE_SOA: {
        if (resolv_class_type == RESOLV_SOA) {
          struct resolv_soa *resolv_soa = (struct resolv_soa *)resolv_class;
          struct network_dns_soa soa = {0};

          network_dns_parser_soa(parser, &soa);

          resolv_soa->mname   = string_new(soa.mname);
          resolv_soa->rname   = string_new(soa.rname);
          resolv_soa->serial  = soa.serial;
          resolv_soa->refresh = soa.refresh;
          resolv_soa->retry   = soa.retry;
          resolv_soa->expire  = soa.expire;
          resolv_soa->minimum = soa.minimum;
        }

        break;
      }

      case NETWORK_DNS_TYPE_TXT: {
        if (resolv_class_type == RESOLV_TXT) {
          struct resolv_txt *resolv_txt = (struct resolv_txt *)resolv_class;
          struct network_dns_txt txt = {0};

          if (!(resolv_txt->txts = (char **)realloc(resolv_txt->txts, (resolv_txt->txts_count+1)*sizeof(char *))))
            break;

          network_dns_parser_txt(parser, &txt);

          resolv_txt->txts[resolv_txt->txts_count++] = string_new(txt.txt);
        }

        break;
      }
    }
  }

_return:
  network_dns_parser_free(parser);
  return resolv_class;
}

static inline uint16_t
resolv_network_dnshdr_id(uint8_t *message) {
  return ntohs(((struct network_dnshdr *)message)->id);
}

static inline uint8_t
resolv_network_dnshdr_truncated(const uint8_t *message) {
  return ((struct network_dnshdr *)message)->truncated;
}

/*
static inline uint8_t
resolv_network_dnshdr_opcode(const uint8_t *message) {
  return ((struct network_dnshdr *)message)->opcode;
}
*/

static inline uint8_t
resolv_network_dnshdr_rcode(const uint8_t *message) {
  return ((struct network_dnshdr *)message)->rcode;
}

static ssize_t
resolv_send_message(uint8_t *message, size_t length, uint8_t *buffer, size_t buffer_length, struct resolv_timeout *timeout) {
  uint16_t receive_length, send_length = htons((uint16_t)length);

  socket_t socket = SOCKET_INVALID;

  ssize_t received = -1, bytes;
  size_t messages;

  for (messages = 0; messages < NETWORK_RESOLV_DNS_MESSAGES; messages++) {
    if ((socket && socket_close(socket)) || !(socket = socket_new(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
      return -1;

    if (socket_connect(socket, (struct sockaddr *)&resolv_server_addr, resolv_server_addr.size, NULL) < 0)
      continue;

    if (socket_send(socket, &send_length, sizeof(send_length), 0) < 0)
      continue;
    if (socket_send(socket, message, length, 0) < 0)
      continue;

    if (socket_recv(socket, &receive_length, sizeof(receive_length), 0, (struct socket_timeout *)timeout) <= 0)
      continue;

    receive_length = ntohs(receive_length);

    for (received = 0; received < receive_length && (bytes = socket_recv(socket, &buffer[received], buffer_length-(size_t)received, 0, (struct socket_timeout *)timeout)) > 0;)
      received += bytes;

    break;
  }

  socket_close(socket);

  return received;
}

static ssize_t
resolv_usend_message(uint8_t *message, size_t length, uint8_t *buffer, size_t buffer_length, struct resolv_timeout *timeout) {
//struct network_dnshdr *dnsh = NULL;
  socket_t socket = SOCKET_INVALID;

  ssize_t received = -1;
  size_t messages;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    return -1;

// if (socket_connect(socket, (struct socket_addr *)&resolv_server_addr, NULL) < 0) {
//   socket_close(socket);
//   return -1;
// }

  for (messages = 0; messages < NETWORK_RESOLV_DNS_MESSAGES; messages++) {
 // struct socket_addr4 server_raddr = {0};
 //
 // if (socket_send(socket, buffer, buffer_length, 0) < 0)
 //   continue;
 
    if (socket_sendto(socket, message, length, 0, (struct sockaddr *)&resolv_server_addr, resolv_server_addr.size) < 0)
      continue;

_receive_response:
 // if ((received = socket_recvfrom(socket, buffer, buffer_length, 0, (struct socket_addr *)&server_raddr, timeout)) <= 0)
 //   continue;

    if ((received = socket_recvfrom(socket, buffer, buffer_length, 0, NULL, NULL, (struct socket_timeout *)timeout)) <= 0)
      continue;

 // dnsh = (struct network_dnshdr *)buffer;

    if (resolv_network_dnshdr_id(message) != resolv_network_dnshdr_id(buffer))
      goto _receive_response;

    if (resolv_network_dnshdr_truncated(buffer))
      received = resolv_send_message(message, length, buffer, buffer_length, timeout);

    break;
  }

  socket_close(socket);

  return received;
}

ssize_t
resolv_search(struct resolv_search *search, const char *name, uint8_t *buffer, size_t buffer_length, struct resolv_timeout *timeout) {
  uint8_t message[NETWORK_DNS_MESSAGE_SIZE] = {0};
  size_t shift;

  struct network_dnshdr dnsh = {
    .id                = random_uint16(),
    .recursion_desired = true,
    .opcode            = (uint8_t)(search->opcode & 0x0F),
    .questions         = 1,
  };

  network_dns_builder_t *builder = NULL;

  ssize_t received = -1;

// if ((resolv_cache = resolv_cache_search(search->question.qtype, search->question.qclass, name))) {
//   memcpy(buffer, resolv_cache->response, resolv_cache->length);
//   return (ssize_t)resolv_cache->length;
// }

  if (!(builder = network_dns_builder_new(&dnsh, message, sizeof(message))))
    goto _return;

  network_dns_builder_questions(builder);
  network_dns_builder_question(builder, &search->question);

  shift = network_dns_builder_build(builder);
  
  network_dns_builder_free(builder);

  if ((received = resolv_usend_message(message, shift, buffer, buffer_length, timeout)) <= 0) {
    resolv_seterror(RESOLV_ESOCKET);
    return received;
  }

// resolv_cache_push(search, buffer, (size_t)received);

_return:
  return received;
}


static const resolv_error_t resolv_errors[] = {
  RESOLV_EFORMER, RESOLV_ESERVFAIL, RESOLV_ENXDOMAIN, RESOLV_ENOTIMP, RESOLV_EREFUSED, RESOLV_EYXDOMAIN, RESOLV_EYXRRSET, RESOLV_ENOTAUTH, RESOLV_ENOTZONE,
};


static inline resolv_error_t
resolv_rcode_to_error(uint8_t rcode) {
  if (rcode >= NETWORK_DNS_RCODE_FORMER && rcode <= NETWORK_DNS_RCODE_NOTZONE)
    return resolv_errors[rcode-1];

  return RESOLV_EUNKNOWN;
}

static struct resolv_class *
resolv_search_by_class(struct resolv_search *search, const char *name, resolv_class_type_t resolv_class_type, size_t resolv_class_size) {
  struct resolv_class *resolv_class = NULL;

  static struct resolv_timeout timeout = { 5, 5000 };

  uint8_t response[NETWORK_DNS_RESPONSE_SIZE] = {0};
  uint8_t rcode;

  ssize_t received = -1;

  if ((received = resolv_search(search, name, response, sizeof(response), &timeout)) <= 0)
    return NULL;

  if ((rcode = resolv_network_dnshdr_rcode(response))) {
    resolv_seterror(resolv_rcode_to_error(rcode));
    return NULL;
  }

  return resolv_answers(resolv_class, resolv_class_size, resolv_class_type, response, (size_t)received);
}

static struct resolv_hostentry *
resolv_hostentry_search(struct resolv_search *search, const char *name) {
  struct resolv_hostentry *hostentry = NULL;

  if (!(hostentry = (struct resolv_hostentry *)resolv_search_by_class(search, name, RESOLV_HOSTENTRY, sizeof(struct resolv_hostentry))))
    return NULL;

  hostentry->addr = ((hostentry->addrs) ? *hostentry->addrs : NULL);
  hostentry->name = ((hostentry->names) ? *hostentry->names : NULL);

  return hostentry;
}


static const struct resolv_hostentry resolv_hostentry4_reserved[] = {
  { "any"      , (char *[]){ "any"      , "all"     , NULL }, NULL,  (char *[]){ "0.0.0.0"        , NULL }, NULL, 2, 1 },
  { "localhost", (char *[]){ "localhost", "loopback", NULL }, NULL,  (char *[]){ "127.0.0.1"      , NULL }, NULL, 2, 1 },
  { "broadcast", (char *[]){ "broadcast", NULL             }, NULL,  (char *[]){ "255.255.255.255", NULL }, NULL, 1, 1 },
  { NULL       , NULL                                       , NULL,  NULL                                 , NULL, 0, 0 },
};

static const struct resolv_hostentry resolv_hostentry6_reserved[] = {
  { "any"      , (char *[]){ "any"      , "all"     , NULL }, NULL,  (char *[]){ "::" , NULL }, NULL, 2, 1 },
  { "localhost", (char *[]){ "localhost", "loopback", NULL }, NULL,  (char *[]){ "::1", NULL }, NULL, 2, 1 },
  { NULL       , NULL                                       , NULL,  NULL                     , NULL, 0, 0 },
};


static struct resolv_hostentry *
resolv_hostentry_reserved(const struct resolv_hostentry *reserved) {
  struct resolv_hostentry *hostentry = NULL;

  if (!(hostentry = (struct resolv_hostentry *)calloc(1, sizeof(struct resolv_hostentry))))
    goto _return;

  if (!(hostentry->names = (char **)calloc(reserved->names_count, sizeof(char *))))
    goto _return;

  for (hostentry->names_count = 0; hostentry->names_count < reserved->names_count; hostentry->names_count++)
    hostentry->names[hostentry->names_count] = string_new(reserved->names[hostentry->names_count]);
  hostentry->name = *hostentry->names;

  if (!(hostentry->addrs = (char **)calloc(reserved->addrs_count, sizeof(char *))))
    goto _return;

  hostentry->addrs[hostentry->addrs_count++] = string_new(*reserved->addrs);
  hostentry->addr = *hostentry->addrs;

  return hostentry;

_return:
  resolv_hostentry_free(hostentry);
  return NULL;
}

static struct resolv_hostentry *
resolv_hostentry_reserved_by_name(const struct resolv_hostentry *reserved, const char *name) {
  char **names = NULL;

  for (; reserved && reserved->hostname; reserved++)
    for (names = reserved->names; names && *names; names++)
      if (string_equals(*names, name, false))
        return resolv_hostentry_reserved(reserved);

  return NULL;
}

static struct resolv_hostentry *
resolv_hostentry_reserved_by_addr(const struct resolv_hostentry *reserved, const char *addr) {
  char **addrs = NULL;

  for (; reserved && reserved->hostname; reserved++)
    for (addrs = reserved->addrs; addrs && *addrs; addrs++)
      if (string_equals(*addrs, addr, false))
        return resolv_hostentry_reserved(reserved);

  return NULL;
}

static struct resolv_hostentry *
resolv_reserved(struct resolv_search *search, const char *name) {
  switch (search->question.qtype) {
    case NETWORK_DNS_TYPE_A:
      return resolv_hostentry_reserved_by_name(resolv_hostentry4_reserved, name);

    case NETWORK_DNS_TYPE_AAAA:
      return resolv_hostentry_reserved_by_name(resolv_hostentry6_reserved, name);

    case NETWORK_DNS_TYPE_PTR:
      if (network_ipaddr(name) == NETWORK_IPADDR4)
        return resolv_hostentry_reserved_by_addr(resolv_hostentry4_reserved, name);

      return resolv_hostentry_reserved_by_addr(resolv_hostentry6_reserved, name);
  }

  return NULL;
}

static inline struct resolv_hostentry *
resolv_hostname(struct resolv_search *search, const char *name) {
  struct resolv_hostentry *hostentry = NULL;

  if ((hostentry = resolv_reserved(search, name)))
    return hostentry;

  return resolv_hostentry_search(search, name);
}

struct resolv_hostentry *
resolv_host4byname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_A, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_hostname(&search, name);
}

struct resolv_hostentry *
resolv_host6byname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_AAAA, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_hostname(&search, name);
}

struct resolv_hostentry *
resolv_hostbyaddr(const char *addr) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_PTR, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_addr_new(search.question.name, addr) == -1)
    return NULL;

// return resolv_hostentry_search(&search, addr);
  return resolv_hostname(&search, addr);
}

void
resolv_hostentry_free(struct resolv_hostentry *hostentry) {
  if (hostentry) {
    if (hostentry->hostname)
      free(hostentry->hostname), hostentry->hostname = NULL;
    if (hostentry->addrs) {
      do {
        free(hostentry->addrs[--hostentry->addrs_count]);
        hostentry->addrs[hostentry->addrs_count] = NULL;
      } while (hostentry->addrs_count);

      free(hostentry->addrs), hostentry->addrs = NULL;
    }
    if (hostentry->names) {
      do {
        free(hostentry->names[--hostentry->names_count]);
        hostentry->names[hostentry->names_count] = NULL;
      } while (hostentry->names_count);

      free(hostentry->names), hostentry->names = NULL;
    }

    free(hostentry);
  }
}

static inline struct resolv_mx *
resolv_mx_search(struct resolv_search *search, const char *name) {
  struct resolv_mx *mx = NULL;

  if (!(mx = (struct resolv_mx *)resolv_search_by_class(search, name, RESOLV_MX, sizeof(struct resolv_mx))))
    return NULL;

  return mx;
}

struct resolv_mx *
resolv_mxbyname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_MX, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_mx_search(&search, name);
}

void
resolv_mx_free(struct resolv_mx *mx) {
  if (mx) {
    if (mx->hostname)
      free(mx->hostname), mx->hostname = NULL;
    if (mx->mx) {
      do {
        free(mx->mx[--mx->mx_count].exchange);
        mx->mx[mx->mx_count].exchange = NULL;
      } while (mx->mx_count);

      free(mx->mx), mx->mx = NULL;
    }

    free(mx);
  }
}

static inline struct resolv_ns *
resolv_ns_search(struct resolv_search *search, const char *name) {
  struct resolv_ns *ns = NULL;

  if (!(ns = (struct resolv_ns *)resolv_search_by_class(search, name, RESOLV_NS, sizeof(struct resolv_ns))))
    return NULL;

  return ns;
}

struct resolv_ns *
resolv_nsbyname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_NS, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_ns_search(&search, name);
}

void
resolv_ns_free(struct resolv_ns *ns) {
  if (ns) {
    if (ns->nsnames) {
      do {
        free(ns->nsnames[--ns->nsnames_count]);
        ns->nsnames[ns->nsnames_count] = NULL;
      } while (ns->nsnames_count);

      free(ns->nsnames), ns->nsnames = NULL;
    }

    free(ns);
  }
}

static inline struct resolv_soa *
resolv_soa_search(struct resolv_search *search, const char *name) {
  struct resolv_soa *soa = NULL;

  if (!(soa = (struct resolv_soa *)resolv_search_by_class(search, name, RESOLV_SOA, sizeof(struct resolv_soa))))
    return NULL;

  return soa;
}

struct resolv_soa *
resolv_soabyname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_SOA, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_soa_search(&search, name);
}

void
resolv_soa_free(struct resolv_soa *soa) {
  if (soa) {
    if (soa->hostname)
      free(soa->hostname), soa->hostname = NULL;
    if (soa->mname)
      free(soa->mname), soa->mname = NULL;
    if (soa->rname)
      free(soa->rname), soa->rname = NULL;

    free(soa);
  }
}

static inline struct resolv_txt *
resolv_txt_search(struct resolv_search *search, const char *name) {
  struct resolv_txt *txt = NULL;

  if (!(txt = (struct resolv_txt *)resolv_search_by_class(search, name, RESOLV_TXT, sizeof(struct resolv_txt))))
    return NULL;

  return txt;
}

struct resolv_txt *
resolv_txtbyname(const char *name) {
  struct resolv_search search = { { .qtype = NETWORK_DNS_TYPE_TXT, .qclass = NETWORK_DNS_CLASS_IN }, NETWORK_DNS_OPCODE_QUERY };

  if (network_dns_name_new(search.question.name, name) == -1)
    return NULL;

  return resolv_txt_search(&search, name);
}

void
resolv_txt_free(struct resolv_txt *txt) {
  if (txt) {
    if (txt->txts) {
      do {
        free(txt->txts[--txt->txts_count]);
        txt->txts[txt->txts_count] = NULL;
      } while (txt->txts_count);

      free(txt->txts), txt->txts = NULL;
    }

    free(txt);
  }
}
