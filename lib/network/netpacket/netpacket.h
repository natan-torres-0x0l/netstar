#ifndef _NETWORK_NETPACKET_NETPACKET_H
#define _NETWORK_NETPACKET_NETPACKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/netlink/netlink.h>

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "netpacket-windows.h"

#elif defined __linux__ || defined __linux
  #include "netpacket-linux.h"

#elif defined __APPLE__ && defined __MACH__
  #include "netpacket-darwin.h"

#elif defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD
  #include "netpacket-bsd.h"

#else
  #include "netpacket-null.h"

#endif

#include "netpacket-errors.h"

#include <sys/types.h>
#include <stddef.h>

typedef enum {
  NETPACKET_PROTOCOL_ETHERNET, NETPACKET_PROTOCOL_IPV4 = 0x04, NETPACKET_PROTOCOL_IPV6 = 0x06
} netpacket_protocol_t;

#define NETPACKET_LINK NETPACKET_PROTOCOL_ETHERNET
#define NETPACKET_IPV4 NETPACKET_PROTOCOL_IPV4
#define NETPACKET_IPV6 NETPACKET_PROTOCOL_IPV6

typedef enum {
  NETPACKET_LAYER_LAYER2 = 0x02, NETPACKET_LAYER_LAYER3 = 0x03
} netpacket_layer_t;

#define NETPACKET_LAYER2 NETPACKET_LAYER_LAYER2
#define NETPACKET_LAYER3 NETPACKET_LAYER_LAYER3

#define netpacket_layer2()         NETPACKET_LAYER_LAYER2, NETPACKET_PROTOCOL_ETHERNET
#define netpacket_layer3(protocol) NETPACKET_LAYER_LAYER3, protocol

struct netpacket_timeout {
  long sec, usec;
};

typedef enum {
  NETPACKET_LIVE_MODE_NORMAL, NETPACKET_LIVE_MODE_PROMISCUOS
} netpacket_live_mode_t;

#define NETPACKET_LIVE_NORMAL     NETPACKET_LIVE_MODE_NORMAL
#define NETPACKET_LIVE_PROMISCUOS NETPACKET_LIVE_MODE_PROMISCUOS


extern netpacket_live_t *netpacket_live_bind(const struct netlink_interface *, netpacket_layer_t, netpacket_protocol_t, netpacket_live_mode_t);

extern netpacket_protocol_t netpacket_live_protocol(netpacket_live_t *);
extern netpacket_layer_t netpacket_live_layer(netpacket_live_t *);

extern netpacket_live_mode_t netpacket_live_mode(netpacket_live_t *);

extern ssize_t netpacket_live_recv(netpacket_live_t *, void *, size_t, const struct netpacket_timeout *);
extern ssize_t netpacket_live_send(netpacket_live_t *, const void *, size_t);

extern void netpacket_live_close(netpacket_live_t *);

#ifdef __cplusplus
}
#endif

#endif
