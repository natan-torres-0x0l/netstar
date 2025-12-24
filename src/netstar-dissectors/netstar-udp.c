#include "netstar-dissectors.h"

#include <strings/strings.h>

#include <netstar.h>


static inline void
netstar_dissector_dissect_udp(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
// struct netstar_udphdr *udph = (struct netstar_udphdr *)packet->layer4.ptr;

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_SOURCE_PORT, "%" PRIu16, packet->layer4.sport);
  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_DESTINATION_PORT, "%" PRIu16, packet->layer4.dport);

  if (packet->layer4.protocol != NETWORK_IANA_UNKNOWN_PROTOCOL)
    netstar_dissector_write(dissector, NETSTAR_DISSECTOR_PACKET_APPLICATION_PROTOCOL, packet->layer4.protocol->name, string_length(packet->layer4.protocol->name));

  netstar_dissector(dissector, packet, NETSTAR_DISSECTOR_UDP_PROTOCOL, packet->layer4.protocol->port);
}


static struct netstar_dissector_protocol netstar_dissector_udp = {
  .routine = netstar_dissector_dissect_udp, .type = NETSTAR_DISSECTOR_IP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER4, .service = NETSTAR_IP_PROTOCOL_UDP
};


__attribute__((__constructor__))
static void
netstar_dissector_udp_register(void) {
  netstar_dissectors_register(&netstar_dissector_udp);
}
