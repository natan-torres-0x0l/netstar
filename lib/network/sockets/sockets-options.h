#ifndef _NETWORK_SOCKETS_SOCKETS_OPTIONS_H
#define _NETWORK_SOCKETS_SOCKETS_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "sockets-windows.h"

#else // elif defined __unix__ || defined __unix
  #include "sockets-unix.h"

#endif

#include <stdbool.h>


extern int socket_setbroadcast(socket_t, bool);
extern int socket_setreuseaddr(socket_t, bool);
extern int socket_setkeepalive(socket_t, bool);

extern int socket_setsndbuf(socket_t, int);
extern int socket_setrcvbuf(socket_t, int);

extern int socket_setlinger(socket_t, struct linger *);

extern int socket_setipttl(socket_t, int);
extern int socket_setiptos(socket_t, int);

extern int socket_setipv6only(socket_t, bool);

extern int socket_settcpnodelay(socket_t, bool);

#ifdef __cplusplus
}
#endif

#endif
