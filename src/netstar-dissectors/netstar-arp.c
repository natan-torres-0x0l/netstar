#include "netstar-dissectors.h"
#include "netstar-arp.h"

#include <netstar-vendors.h>

#include <netstar.h>


static inline void
netstar_dissector_dissect_arp(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  char saddr[NETWORK_MACADDR_STRLENGTH] = {0}, daddr[NETWORK_MACADDR_STRLENGTH] = {0};
  struct netstar_arphdr *arph = (struct netstar_arphdr *)packet->layer2.payload;
  uint16_t opcode = ntohs(arph->opcode);

  if (!(dissector->layers & NETSTAR_DISSECTOR_LAYER2)) {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS, "%s %s", network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)),
      netstar_vendors_namebymac(&packet->layer2.smac));

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS, "%s %s", netstar_vendors_namebymac(&packet->layer2.dmac),
      network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)));
  } else {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS, "%s", network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)));

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS, "%s", network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)));
  }

  if (opcode == NETSTAR_ARP_OPCODE_REQUEST) {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Request who has %s? Tell %s", network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)),
      network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)));
  } else if (opcode == NETSTAR_ARP_OPCODE_REPLY) {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "Reply %s is at %s", network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)),
      network_macaddr_format(&packet->layer2.smac, daddr, sizeof(daddr)));
  } else {
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s Operation code (0x%04" PRIX16 ") %s", network_macaddr_format(&packet->layer2.smac, saddr, sizeof(saddr)),
      network_macaddr_format(&packet->layer2.dmac, daddr, sizeof(daddr)));
  }
}


static struct netstar_dissector_protocol netstar_dissector_arp = {
  .routine = netstar_dissector_dissect_arp, .layer = NETSTAR_DISSECTOR_LAYER3, .type = NETSTAR_DISSECTOR_ETHERNET_PROTOCOL, .service = NETSTAR_ETHERNET_PROTOCOL_ARP
};


__attribute__((__constructor__))
static void
netstar_dissector_arp_register(void) {
  netstar_dissectors_register(&netstar_dissector_arp);
}
