#ifndef _NETWORK_NETLINK_NETLINK_ERRORS_H
#define _NETWORK_NETLINK_NETLINK_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <system/system.h>

typedef enum {
  NETLINK_EUNKNOWN = -5, NETLINK_ESYSCALL, NETLINK_ENOSUPPORT, NETLINK_ENOINTERFACE,
#if defined __linux__ || defined __linux
  NETLINK_ESOCKET,
#endif
} netlink_error_t;


extern const char *netlink_error_reason(netlink_error_t);

extern netlink_error_t netlink_geterror(void);
extern void netlink_seterror(netlink_error_t);

#ifdef __cplusplus
}
#endif

#endif

