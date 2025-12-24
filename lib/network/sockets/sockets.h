#ifndef _NETWORK_SOCKETS_SOCKETS_H
#define _NETWORK_SOCKETS_SOCKETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/inet/inet.h>

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "sockets-windows.h"

#else // elif defined __unix__ || defined __unix
  #include "sockets-unix.h"

#endif

#include "sockets-handler.h"
#include "sockets-events.h"
#include "sockets-options.h"
#include "sockets-errors.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


extern ssize_t socket_recvfrom(socket_t, void *, size_t, int, struct sockaddr *, socklen_t *, const struct socket_timeout *);
extern ssize_t socket_sendto(socket_t, const void *, size_t, int, struct sockaddr *, socklen_t);

extern ssize_t socket_recv(socket_t, void *, size_t, int, const struct socket_timeout *);
extern ssize_t socket_send(socket_t, const void *, size_t, int);

extern int socket_connect(socket_t, struct sockaddr *, socklen_t, const struct socket_timeout *);

extern int socket_bind(socket_t, struct sockaddr *, socklen_t);
extern int socket_listen(socket_t, int);

extern socket_t socket_accept(socket_t, struct sockaddr *, socklen_t *, const struct socket_timeout *);

extern int socket_localaddr(socket_t, struct sockaddr *, socklen_t *);
extern int socket_remoteaddr(socket_t, struct sockaddr *, socklen_t *);

extern int socket_setoption(socket_t, int, int, const void *, socklen_t);
extern int socket_getoption(socket_t, int, int, void *, socklen_t *);

extern int socket_setblocking(socket_t, bool);
// extern bool socket_isnonblocking(socket_t);

extern socket_t socket_open(int, int, int);
extern socket_t socket_new(int, int, int);

extern int socket_shutdown(socket_t, int);
extern int socket_close(socket_t);

#ifdef __cplusplus
}
#endif

#endif
