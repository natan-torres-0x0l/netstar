#include "netpacket-errors.h"

#include <network/sockets/sockets-errors.h>

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#if !defined __cplusplus
  #define thread_local _Thread_local
#endif


thread_local netpacket_error_t netpacket_error;


const char *
netpacket_error_reason(netpacket_error_t error) {
  switch (error) {
    case NETPACKET_EPROTOCOL:
      return "network layer/protocol is not supported";

    case NETPACKET_ESOCKET:
      return socket_error_reason(socket_geterror());

    case NETPACKET_ETIMEOUT:
      return "operation timed out";

    case NETPACKET_ENOSUPPORT:
      return "there is no support/implementation for the system";

    case NETPACKET_ESYSCALL:
      return system_error_reason(system_geterror());
  }

  return "unknown error";
}

netpacket_error_t
netpacket_geterror(void) {
  return netpacket_error;
}

void
netpacket_seterror(netpacket_error_t error) {
  netpacket_error = error;
}
