#include "netstar-dissectors.h"

#include <netstar.h>


static void
netstar_dissector_dissect_igmp(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  struct netstar_igmphdr *igmph = (struct netstar_igmphdr *)(packet->layer3.payload);

  switch (igmph->type) {
    case NETSTAR_IGMP_TYPE_MEMBERSHIP_QUERY: {
      netstar_dissector_write(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Membership query", 16);
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V1: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Membership report v1 %s",
        network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V2: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Membership report v2 %s",
        network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_LEAVE_GROUP: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Leave group %s",
        network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V3: {
      struct netstar_igmpv3hdr *igmpv3h = (struct netstar_igmpv3hdr *)igmph;

      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Membership report v3 %" PRIu16, igmpv3h->groups_count);
      break;
    }

    default:
      netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Message type (0x%02" PRIX8 ")", igmph->type);
      break;
  }
}


static struct netstar_dissector_protocol netstar_dissector_igmp = {
  .routine = netstar_dissector_dissect_igmp, .type = NETSTAR_DISSECTOR_IP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER3, .service = NETSTAR_IP_PROTOCOL_IGMP
};


__attribute__((__constructor__))
static void
netstar_igmp_register(void) {
  netstar_dissectors_register(&netstar_dissector_igmp);
}
