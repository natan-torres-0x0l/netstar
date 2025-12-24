#include "netstar-dissectors.h"

#include <netstar.h>


struct netstar_icmp_type {
  uint8_t u8;
  const char *description;
};

static struct netstar_icmp_type netstar_icmp_types[] = {
  { NETSTAR_ICMP_TYPE_ECHO_REPLY             , "Echo reply"              },
  { NETSTAR_ICMP_TYPE_ECHO                   , "Echo request"            },
  { NETSTAR_ICMP_TYPE_DESTINATION_UNREACHABLE, "Destination unreachable" },
  { NETSTAR_ICMP_TYPE_TIME_EXCEEDED          , "Time exceeded"           },
  { NETSTAR_ICMP_TYPE_PARAMETER_PROBLEM      , "Parameter problem"       },
  { NETSTAR_ICMP_TYPE_REDIRECT               , "Redirect"                },
  { NETSTAR_ICMP_TYPE_ROUTER_ADVERT          , "Router advertisement"    },
  { NETSTAR_ICMP_TYPE_ROUTER_SOLICIT         , "Router solicitation"     },
  { NETSTAR_ICMP_TYPE_TIMESTAMP_REQUEST      , "Timestamp request"       },
  { NETSTAR_ICMP_TYPE_TIMESTAMP_REPLY        , "Timestamp reply"         },
  { NETSTAR_ICMP_TYPE_UNFORMATION_REQUEST    , "Unformation request"     },
  { NETSTAR_ICMP_TYPE_UNFORMATION_REPLY      , "Unformation reply"       },
  { NETSTAR_ICMP_TYPE_NETMASK_REQUEST        , "Address mask request"    },
  { NETSTAR_ICMP_TYPE_NETMASK_REPLY          , "Address mask reply"      },
  { 0x00                                     , NULL                      }

};

static const struct netstar_icmp_type *
netstar_icmp_type(uint8_t type) {
  const struct netstar_icmp_type *types = NULL;

  for (types = netstar_icmp_types; types->description; types++)
    if (types->u8 == type)
      return types;

  return NULL;
}

static inline void
netstar_dissector_dissect_icmpv4(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  struct netstar_icmphdr *icmph = (struct netstar_icmphdr *)(packet->layer3.payload);
  const struct netstar_icmp_type *type = NULL;

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "%s ",
    ((type = netstar_icmp_type(icmph->type)) ? type->description : "Message type"));

  netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "(0x%02" PRIX8 ")", icmph->type);
}


static struct netstar_dissector_protocol netstar_dissector_icmpv4 = {
  .routine = netstar_dissector_dissect_icmpv4, .type = NETSTAR_DISSECTOR_IP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER3, .service = NETSTAR_IP_PROTOCOL_ICMPV4
};


__attribute__((__constructor__))
static void
netstar_dissector_icmpv4_register(void) {
  netstar_dissectors_register(&netstar_dissector_icmpv4);
}
