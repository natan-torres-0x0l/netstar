#include "resolv-errors.h"

#include <network/sockets/sockets-errors.h>

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#if !defined __cplusplus
  #define thread_local _Thread_local
#endif


thread_local resolv_error_t resolv_error;


const char *
resolv_error_reason(resolv_error_t error) {
  switch (error) {
    case RESOLV_EFORMER:
      return "resource format error";

    case RESOLV_ESERVFAIL:
      return "server failed to complete the request";

    case RESOLV_ENXDOMAIN:
      return "non-existent requested domain";

    case RESOLV_ENOTIMP:
      return "server does not implement the type of query performed";

    case RESOLV_EREFUSED:
      return "search query refused";

    case RESOLV_EYXDOMAIN:
      return "name exists when it should not";

    case RESOLV_EYXRRSET:
      return "RR set exists when it should not";

    case RESOLV_ENXERSET:
      return "RR set that should exist does not";

    case RESOLV_ENOTAUTH:
      return "server not authoritative for zone";

    case RESOLV_ENOTZONE:
      return "name not contained in zone";

    case RESOLV_ENORECORD:
      return "record query not found";

    case RESOLV_ESOCKET:
      return socket_error_reason(socket_geterror());

    case RESOLV_ESYSCALL:
      return system_error_reason(system_geterror());
  }

  return "unknown error";
}

resolv_error_t
resolv_geterror(void) {
  return resolv_error;
}

void
resolv_seterror(resolv_error_t error) {
  resolv_error = error;
}
