#ifndef _NETSTAR_PACKET_H
#define _NETSTAR_PACKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar.h>

typedef struct netstar_packet_block netstar_packet_block_t;
typedef struct netstar_packet netstar_packet_t;


extern netstar_packet_block_t *netstar_packet_buildethernet(netstar_packet_t *, netstar_packet_block_t *, const network_macaddr_t *, const network_macaddr_t *, uint16_t, uint8_t *, uint16_t);

extern netstar_packet_block_t *netstar_packet_buildarp(netstar_packet_t *, netstar_packet_block_t *, uint16_t, uint16_t, uint8_t, uint8_t, uint16_t, const network_macaddr_t *, const network_ipaddr4_t *, const network_macaddr_t *, const network_ipaddr4_t *, uint8_t *, uint16_t);

extern netstar_packet_block_t *netstar_packet_buildipv4(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint16_t, const network_ipaddr4_t *, const network_ipaddr4_t *, uint8_t *, uint16_t);
extern netstar_packet_block_t *netstar_packet_buildipv6(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint32_t, uint16_t, uint8_t, uint8_t, const network_ipaddr6_t *, const network_ipaddr6_t *, uint8_t *, uint16_t);

extern netstar_packet_block_t *netstar_packet_buildicmpredirect(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint8_t, const network_ipaddr4_t *, uint16_t, uint8_t *, uint16_t);
extern netstar_packet_block_t *netstar_packet_buildicmpecho(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint8_t, uint16_t, uint16_t, uint16_t, uint8_t *, uint16_t);

extern netstar_packet_block_t *netstar_packet_buildicmpv6echo(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint8_t, uint16_t, uint16_t, uint16_t, uint8_t *, uint16_t);

extern netstar_packet_block_t *netstar_packet_buildndpsolicit(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint8_t, uint16_t, const network_ipaddr6_t *, uint8_t *, uint16_t);
extern netstar_packet_block_t *netstar_packet_buildndpadvert(netstar_packet_t *, netstar_packet_block_t *, uint8_t, uint8_t, uint16_t, uint32_t, const network_ipaddr6_t *, uint8_t *, uint16_t);
extern netstar_packet_block_t *netstar_packet_buildndpoption(netstar_packet_t *, netstar_packet_block_t *, uint8_t, const void *, uint8_t);

extern netstar_packet_block_t *netstar_packet_buildtcp(netstar_packet_t *, netstar_packet_block_t *, uint16_t, uint16_t, uint32_t, uint32_t, uint8_t, uint16_t, uint16_t, uint16_t, uint8_t *, uint16_t);
extern netstar_packet_block_t *netstar_packet_buildtcpoptions(netstar_packet_t *, netstar_packet_block_t *, const uint8_t *, uint8_t);

extern netstar_packet_block_t *netstar_packet_buildudp(netstar_packet_t *, netstar_packet_block_t *, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t *, uint16_t);

extern void netstar_packet_coalesce(netstar_packet_t *);

extern uint8_t *netstar_packet_buffer(netstar_packet_t *);
extern size_t netstar_packet_length(netstar_packet_t *);

extern netstar_packet_t *netstar_packet_new(void);
extern void netstar_packet_free(netstar_packet_t *);

#ifdef __cplusplus
}
#endif

#endif
