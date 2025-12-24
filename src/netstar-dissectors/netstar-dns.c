#include "netstar-dissectors.h"

#include <network/dns/dns.h>

#include <strings/strings.h>

#include <netstar.h>

#define NETSTAR_DISSECT_DNS_RESPONSE_RECORDS 4

static void
netstar_dissector_dissect_dns_response(netstar_dissector_t *dissector, struct netstar_capture_packet *packet, network_dns_parser_t *parser) {
  uint16_t answers = ((network_dns_parser_nanswers(parser) > NETSTAR_DISSECT_DNS_RESPONSE_RECORDS) ? NETSTAR_DISSECT_DNS_RESPONSE_RECORDS : network_dns_parser_nanswers(parser));

  network_dns_parser_answers(parser);

  for (; answers; answers--) {
    struct network_dns_resource resource = {0};

    if (network_dns_parser_resource(parser, &resource) == -1)
      return;

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s response %s %s",
      network_dns_opcode_format(network_dns_parser_header(parser)->opcode), resource.name, network_dns_type_format(resource.rtype));

    switch (resource.rtype) {
      case NETWORK_DNS_TYPE_A: {
        struct network_dns_a a = {0};

        if (network_dns_parser_a(parser, &a))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", a.a);

        break;
      }

      case NETWORK_DNS_TYPE_AAAA: {
        struct network_dns_aaaa aaaa = {0};

        if (network_dns_parser_aaaa(parser, &aaaa))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", aaaa.aaaa);

        break;
      }

      case NETWORK_DNS_TYPE_CNAME: {
        struct network_dns_cname cname = {0};

        if (network_dns_parser_cname(parser, &cname))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", cname.cname);

        break;
      }

      case NETWORK_DNS_TYPE_MX: {
        struct network_dns_mx mx = {0};

        if (network_dns_parser_mx(parser, &mx))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %" PRIu16 " %s", mx.preference, mx.exchange);

        break;
      }

      case NETWORK_DNS_TYPE_NS: {
        struct network_dns_ns ns = {0};

        if (network_dns_parser_ns(parser, &ns))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", ns.nsname);

        break;
      }

      case NETWORK_DNS_TYPE_PTR: {
        struct network_dns_ptr ptr = {0};

        if (network_dns_parser_ptr(parser, &ptr))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", ptr.ptr);

        break;
      }

      case NETWORK_DNS_TYPE_SOA: {
        struct network_dns_soa soa = {0};

        if (network_dns_parser_soa(parser, &soa))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s %s %" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRIu32,
            soa.mname, soa.rname, soa.serial, soa.refresh, soa.retry, soa.expire, soa.minimum);

        break;
      }

      case NETWORK_DNS_TYPE_TXT: {
        struct network_dns_txt txt = {0};

        if (network_dns_parser_txt(parser, &txt))
          netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %s", txt.txt);

        break;
      }
    }

    if (answers && answers-1)
      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "; ");
  }
}

static void
netstar_dissector_dissect_dns_query(netstar_dissector_t *dissector, struct netstar_capture_packet *packet, network_dns_parser_t *parser) {
  struct network_dns_question question = {0};

  if (!network_dns_parser_nquestions(parser) || network_dns_parser_question(parser, &question) == -1)
    return;

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s %s %s",
    network_dns_opcode_format(network_dns_parser_header(parser)->opcode), question.name, network_dns_type_format(question.qtype));
}

static void
netstar_dissector_dissect_dns(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  const uint8_t *payload = (const uint8_t *)packet->layer4.payload;
  size_t payload_length = packet->layer4.payload_length;

  network_dns_parser_t *parser = NULL;

  if (packet->type & NETSTAR_FORWARD_TCP /* && packet->layer4.payload_length == sizeof(uint16_t)*/) {
    payload += sizeof(uint16_t), payload_length -= sizeof(uint16_t);
  }
  if (!(parser = network_dns_parser_new(payload, payload_length)))
    return;

  if (network_dns_parser_header(parser)->response)
    netstar_dissector_dissect_dns_response(dissector, packet, parser);
  else
    netstar_dissector_dissect_dns_query(dissector, packet, parser);

  network_dns_parser_free(parser);
}


static struct netstar_dissector_protocol netstar_dissector_tcp_dns = {
  .routine = netstar_dissector_dissect_dns, .type = NETSTAR_DISSECTOR_TCP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER7, .service = NETSTAR_DNS_PROTOCOL_PORT
};

static struct netstar_dissector_protocol netstar_dissector_udp_dns = {
  .routine = netstar_dissector_dissect_dns, .type = NETSTAR_DISSECTOR_UDP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER7, .service = NETSTAR_DNS_PROTOCOL_PORT
};


__attribute__((__constructor__))
static void
netstar_dissector_dns_register(void) {
  netstar_dissectors_register(&netstar_dissector_tcp_dns);
  netstar_dissectors_register(&netstar_dissector_udp_dns);
}
