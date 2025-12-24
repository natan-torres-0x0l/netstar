#ifndef _NETWORK_SOCKETS_SOCKETS_ERRORS_H
#define _NETWORK_SOCKETS_SOCKETS_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "sockets-windows.h"

#elif defined __unix__ || defined __unix
  #include "sockets-unix.h"

#endif

#include <system/system.h>

typedef enum {
  SOCKET_ESYSCALL = -3, SOCKET_ECLOSED, SOCKET_ETIMEOUT,
} socket_error_t;


extern const char *socket_error_reason(socket_error_t);

extern socket_error_t socket_geterror(void);
extern void socket_seterror(socket_error_t);

#ifdef __cplusplus
}
#endif

#endif
