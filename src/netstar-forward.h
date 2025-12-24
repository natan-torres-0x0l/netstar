#ifndef _NETSTAR_FORWARD_H
#define _NETSTAR_FORWARD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  NETSTAR_FORWARD_LAYER2 = 0x02, NETSTAR_FORWARD_LAYER3 = 0x04, NETSTAR_FORWARD_LAYER4 = 0x08
} netstar_forward_layer_t;

typedef enum {
  NETSTAR_FORWARD_ANY         = 0x0000,
  NETSTAR_FORWARD_ALL         = NETSTAR_FORWARD_ANY,

  NETSTAR_FORWARD_LOOPBACK    = 0x0001,

  NETSTAR_FORWARD_ARP_REQUEST = 0x0002,
  NETSTAR_FORWARD_ARP_REPLY   = 0x0004,
  NETSTAR_FORWARD_ARP         = (NETSTAR_FORWARD_ARP_REQUEST | NETSTAR_FORWARD_ARP_REPLY),

  NETSTAR_FORWARD_IPV4        = 0x0008,
  NETSTAR_FORWARD_IPV6        = 0x0010,
  NETSTAR_FORWARD_IP          = (NETSTAR_FORWARD_IPV4 | NETSTAR_FORWARD_IPV6),

  NETSTAR_FORWARD_ICMPV4      = 0x0020,
  NETSTAR_FORWARD_ICMPV6      = 0x0040,
  NETSTAR_FORWARD_ICMP        = (NETSTAR_FORWARD_ICMPV4 | NETSTAR_FORWARD_ICMPV6),

  NETSTAR_FORWARD_NDP_SOLICIT = 0x0080,
  NETSTAR_FORWARD_NDP_ADVERT  = 0x0100,
  NETSTAR_FORWARD_NDP         = (NETSTAR_FORWARD_NDP_SOLICIT | NETSTAR_FORWARD_NDP_ADVERT),

  NETSTAR_FORWARD_TCPV4       = 0x0200,
  NETSTAR_FORWARD_TCPV6       = 0x0400,
  NETSTAR_FORWARD_TCP         = (NETSTAR_FORWARD_TCPV4 | NETSTAR_FORWARD_TCPV6),

  NETSTAR_FORWARD_UDPV4       = 0x0800,
  NETSTAR_FORWARD_UDPV6       = 0x1000,
  NETSTAR_FORWARD_UDP         = (NETSTAR_FORWARD_UDPV4 | NETSTAR_FORWARD_UDPV6),
} netstar_forward_protocol_t;

#include <netstar-capture.h>
#include <netstar.h>

typedef void (*netstar_forward_routine_t)(netstar_t *, struct netstar_capture_packet *, void *);


extern void netstar_forward(netstar_t *, struct netstar_capture_packet *);

extern void netstar_forward_add(netstar_forward_routine_t, netstar_forward_protocol_t, void *);
extern void netstar_forward_remove(netstar_forward_routine_t);

extern void netstar_forward_initialize(void);
extern void netstar_forward_deinitialize(void);

#ifdef __cplusplus
}
#endif

#endif
