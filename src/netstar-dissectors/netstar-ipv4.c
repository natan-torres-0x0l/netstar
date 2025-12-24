#include "netstar-dissectors.h"

#include <netstar-vendors.h>

#include <netstar.h>


static inline void
netstar_dissector_dissect_ipv4(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  const struct netstar_ip_protocol *protocol = NULL;
  char addr[NETWORK_IPADDR4_STRLENGTH] = {0};

  if (!(dissector->layers & NETSTAR_DISSECTOR_LAYER2)) {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS, "%s %s", network_ipaddr4_format(&packet->layer3.saddr.v4, addr, sizeof(addr)),
      netstar_vendors_namebymac(&packet->layer2.smac));

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS, "%s %s", netstar_vendors_namebymac(&packet->layer2.dmac),
      network_ipaddr4_format(&packet->layer3.daddr.v4, addr, sizeof(addr)));
  } else {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS, "%s", network_ipaddr4_format(&packet->layer3.saddr.v4, addr, sizeof(addr)));

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS, "%s", network_ipaddr4_format(&packet->layer3.daddr.v4, addr, sizeof(addr)));
  }

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_IP_PROTOCOL, "%s ",
    ((protocol = netstar_ip_protocol(packet->layer3.protocol)) ? protocol->name : "Protocol"));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_IP_PROTOCOL, "(0x%02" PRIX8 ")", packet->layer3.protocol);

  netstar_dissector(dissector, packet, NETSTAR_DISSECTOR_IP_PROTOCOL, packet->layer3.protocol);
}


static struct netstar_dissector_protocol netstar_dissector_ipv4 = {
  .routine = netstar_dissector_dissect_ipv4, .type = NETSTAR_DISSECTOR_ETHERNET_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER3, .service = NETSTAR_ETHERNET_PROTOCOL_IPV4
};


__attribute__((__constructor__))
static void
netstar_dissector_ipv4_register(void) {
  netstar_dissectors_register(&netstar_dissector_ipv4);
}
