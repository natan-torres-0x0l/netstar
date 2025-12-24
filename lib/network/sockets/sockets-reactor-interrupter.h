#ifndef _NETWORK_SOCKETS_SOCKETS_REACTOR_INTERRUPTER_H
#define _NETWORK_SOCKETS_SOCKETS_REACTOR_INTERRUPTER_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "sockets-windows.h"

#else // elif defined __unix__ || defined __unix
  #include "sockets-unix.h"

#endif

typedef socket_t socket_reactor_interrupter_t[2];


extern int socket_reactor_interrupter_interrupt(socket_reactor_interrupter_t);

extern int socket_reactor_interrupter_new(socket_reactor_interrupter_t);
extern void socket_reactor_interrupter_close(socket_reactor_interrupter_t);

#ifdef __cplusplus
}
#endif

#endif
