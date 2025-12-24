#include "sockets-errors.h"

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#if !defined __cplusplus
  #define thread_local _Thread_local
#endif


thread_local socket_error_t socket_error;


const char *
socket_error_reason(socket_error_t error) {
  switch (error) {
    case SOCKET_ETIMEOUT:
      return "socket operation timed out";

    case SOCKET_ECLOSED:
      return "socket is currently closed";

    case SOCKET_ESYSCALL:
      return system_error_reason(system_geterror());
  }

  return "unknown error";
}

socket_error_t
socket_geterror(void) {
  return socket_error;
}

void
socket_seterror(socket_error_t error) {
  socket_error = error;
}
