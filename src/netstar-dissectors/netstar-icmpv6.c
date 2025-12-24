#include "netstar-dissectors.h"
#include "netstar-icmpv6.h"

#include <netstar.h>


struct netstar_icmpv6_type {
  uint8_t u8;
  const char *description;
};

static struct netstar_icmpv6_type netstar_icmpv6_types[] = {
  { NETSTAR_ICMPV6_TYPE_ECHO_REPLY             , "Echo reply"              },
  { NETSTAR_ICMPV6_TYPE_ECHO                   , "Echo request"            },
  { NETSTAR_ICMPV6_TYPE_DESTINATION_UNREACHABLE, "Destination unreachable" },
  { NETSTAR_ICMPV6_TYPE_PACKET_TOO_BIG         , "Packet too big"          },
  { NETSTAR_ICMPV6_TYPE_TIME_EXCEEDED          , "Time exceeded"           },
  { NETSTAR_ICMPV6_TYPE_PARAMETER_PROBLEM      , "Parameter problem"       },
  { 0x00                                       , NULL                      },
};


static const struct netstar_icmpv6_type *
netstar_icmpv6_type(uint8_t type) {
  const struct netstar_icmpv6_type *types = NULL;

  for (types = netstar_icmpv6_types; types->description; types++)
    if (types->u8 == type)
      return types;

  return NULL;
}


static void
netstar_dissector_dissect_icmpv6_ndp(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  struct netstar_ndphdr *ndph = (struct netstar_ndphdr *)(packet->layer3.payload);

  switch (ndph->type) {
    case NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT: {
      char dmac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Neighbor solicitation from %s to %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
        network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));
      break;
    }

    case NETSTAR_NDP_TYPE_ROUTER_SOLICIT: {
      char dmac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Router solicitation from %s to %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
        network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));
      break;
    }

    case NETSTAR_NDP_TYPE_NEIGHBOR_ADVERT: {
      char smac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Neighbor advertisement %s is at %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
        network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)));
      break;
    }

    case NETSTAR_NDP_TYPE_ROUTER_ADVERT: {
      char smac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Router advertisement %s is at %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
        network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)));
      break;
    }

    case NETSTAR_NDP_TYPE_REDIRECT: {
      netstar_dissector_write(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Redirect", 8);
      break;
    }

    default: {
      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Message type 0x%02" PRIX8, ndph->type);
      break;
    }
  }
}

static void
netstar_dissector_dissect_icmpv6(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  struct netstar_icmpv6hdr *icmpv6h = (struct netstar_icmpv6hdr *)(packet->layer3.payload);
  const struct netstar_icmpv6_type *type = NULL;

  if (icmpv6h->type >= NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT && icmpv6h->type <= NETSTAR_NDP_TYPE_REDIRECT) {
    netstar_dissector_dissect_icmpv6_ndp(dissector, packet);
    return;
  }

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s ",
    ((type = netstar_icmpv6_type(icmpv6h->type)) ? type->description : "Message type"));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "(0x%02" PRIX8 ")", icmpv6h->type);
}


static struct netstar_dissector_protocol netstar_dissector_icmpv6 = {
  .routine = netstar_dissector_dissect_icmpv6, .type = NETSTAR_DISSECTOR_IP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER3, .service = NETSTAR_IP_PROTOCOL_ICMPV6
};


__attribute__((__constructor__))
static void
netstar_dissector_icmpv6_register(void) {
  netstar_dissectors_register(&netstar_dissector_icmpv6);
}
