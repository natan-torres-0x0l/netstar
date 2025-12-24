#include "netstar-dissectors.h"
#include "netstar-ethernet.h"

#include <netstar-vendors.h>

#include <netstar.h>


static inline void
netstar_dissector_dissect_ethernet(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  const struct netstar_ethernet_type *type = NULL;
  char mac[NETWORK_MACADDR_STRLENGTH] = {0};

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_HARDWARE, "%s %s", network_macaddr_format(&packet->layer2.smac, mac, sizeof(mac)),
    netstar_vendors_namebymac(&packet->layer2.smac));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_HARDWARE, "%s %s", netstar_vendors_namebymac(&packet->layer2.dmac),
    network_macaddr_format(&packet->layer2.dmac, mac, sizeof(mac)));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_ETHERNET_PROTOCOL, "%s ",
    ((type = netstar_ethernet_type(packet->layer2.protocol)) ? type->name : "Ethernet type"));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_ETHERNET_PROTOCOL, "(0x%04" PRIX16 ")", packet->layer2.protocol);

  netstar_dissector(dissector, packet, NETSTAR_DISSECTOR_ETHERNET_PROTOCOL, packet->layer2.protocol);
}


static struct netstar_dissector_protocol netstar_dissector_ethernet = {
  .routine = netstar_dissector_dissect_ethernet,
};

struct netstar_dissector_protocol *netstar_dissector_root = &netstar_dissector_ethernet;
