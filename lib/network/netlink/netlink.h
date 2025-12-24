#ifndef _NETWORK_NETLINK_NETLINK_H
#define _NETWORK_NETLINK_NETLINK_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "netlink-windows.h"

#elif defined __linux__ || defined __linux
  #include "netlink-linux.h"

#elif defined __APPLE__ && defined __MACH__
  #include "netlink-darwin.h"

#elif defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD
  #include "netlink-bsd.h"

#else
  #include "netlink-null.h"

#endif

#include "netlink-errors.h"

#include <stdbool.h>

typedef enum {
  NETLINK_INTERFACE_FLAGS_UP           = 0x0001,
#define NETLINK_IFUP NETLINK_INTERFACE_FLAGS_UP

  NETLINK_INTERFACE_FLAGS_RUNNING      = 0x0002,
#define NETLINK_IFRUNNING NETLINK_INTERFACE_FLAGS_RUNNING

  NETLINK_INTERFACE_FLAGS_BROADCAST    = 0x0004,
#define NETLINK_IFBROADCAST NETLINK_INTERFACE_FLAGS_BROADCAST

  NETLINK_INTERFACE_FLAGS_LOOPBACK     = 0x0008,
#define NETLINK_IFLOOPBACK NETLINK_INTERFACE_FLAGS_LOOPBACK

  NETLINK_INTERFACE_FLAGS_POINTTOPOINT = 0x0010,
#define NETLINK_IFPOINTTOPOINT NETLINK_INTERFACE_FLAGS_POINTTOPOINT

  NETLINK_INTERFACE_FLAGS_MULTICAST    = 0x0020,
#define NETLINK_IFMULTICAST NETLINK_INTERFACE_FLAGS_MULTICAST
} netlink_interface_flags_t;


extern bool netlink_interface_isreachable(const struct netlink_interface *);
extern bool netlink_interface_isup(const struct netlink_interface *);
extern bool netlink_interface_isrunning(const struct netlink_interface *);

extern bool netlink_interface_isbroadcast(const struct netlink_interface *);
extern bool netlink_interface_isloopback(const struct netlink_interface *);
extern bool netlink_interface_ispointtopoint(const struct netlink_interface *);
extern bool netlink_interface_ismulticast(const struct netlink_interface *);

extern struct netlink_interface *netlink_interfacebyname(const char *);
extern struct netlink_interface *netlink_interfacebyindex(uint32_t);

extern void netlink_interface_free(struct netlink_interface *);

extern struct netlink_interface **netlink_interfaces(void);
extern void netlink_interfaces_free(struct netlink_interface **);

#ifdef __cplusplus
}
#endif

#endif
