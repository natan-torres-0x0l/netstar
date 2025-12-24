#ifndef _NETWORK_SOCKETS_SOCKETS_EVENTS_H
#define _NETWORK_SOCKETS_SOCKETS_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#define SOCKET_EVENTS_WAIT_SERVICE_POLL

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "sockets-windows.h"

#else // elif defined __unix__ || defined __unix
  #include "sockets-unix.h"

#endif

#include <stddef.h>


typedef enum {
  SOCKET_READ = 0x01, SOCKET_ACCEPT = SOCKET_READ, SOCKET_WRITE = 0x04, SOCKET_CONNECT = SOCKET_WRITE, SOCKET_EXCEPT = 0x08,
} socket_event_t;

struct socket_event {
  socket_event_t events;
	socket_t socket;
};

#define SOCKET_NONBLOCKING socket_nonblocking

struct socket_timeout {
  long sec, usec;
};


extern const struct socket_timeout socket_nonblocking;


extern int socket_select(int, fd_set *, fd_set *, fd_set *, const struct socket_timeout *);

extern int socket_poll(struct pollfd *pollfds, size_t, const struct socket_timeout *);

extern int socket_wait(socket_t, socket_event_t, const struct socket_timeout *);

#ifdef __cplusplus
}
#endif

#endif
