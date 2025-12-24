#include "netlink-errors.h"

#if defined __linux__ || defined __linux
  #include <network/sockets/sockets-errors.h>
#endif

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#if !defined __cplusplus
  #define thread_local _Thread_local
#endif


thread_local netlink_error_t netlink_error;


const char *
netlink_error_reason(netlink_error_t error) {
  switch (error) {
    case NETLINK_ENOINTERFACE:
      return "no such network interface";

    case NETLINK_ENOSUPPORT:
      return "there is no support/implementation for the system";

#if defined __linux__ || defined __linux
    case NETLINK_ESOCKET:
      return socket_error_reason(socket_geterror());
#endif

    case NETLINK_ESYSCALL:
      return system_error_reason(system_geterror());
  }

  return "unknown error";
}

netlink_error_t
netlink_geterror(void) {
  return netlink_error;
}

void
netlink_seterror(netlink_error_t error) {
  netlink_error = error;
}
