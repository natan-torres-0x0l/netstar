#include "netstar-dissectors.h"
#include "netstar-gre.h"

#include <netstar.h>


static inline void
netstar_dissector_dissect_gre(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  struct netstar_grehdr *greh = (struct netstar_grehdr *)(packet->layer3.payload);
  struct netstar_ethernet_type *type = NULL;

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s ",
    ((type = netstar_ethernet_type(packet->layer2.protocol)) ? type->name : "Ethernet type"));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "(0x%04" PRIX16 ")", greh->protocol);
}


static struct netstar_dissector_protocol netstar_dissector_gre = {
  .routine = netstar_dissector_dissect_gre, .type = NETSTAR_DISSECTOR_IP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER3, .service = NETSTAR_IP_PROTOCOL_GRE
};


__attribute__((__constructor__))
static void
netstar_dissector_gre_register(void) {
  netstar_dissectors_register(&netstar_dissector_gre);
}
