#ifndef _NETWORK_SOCKETS_SOCKETS_EPOLL_REACTOR_H
#define _NETWORK_SOCKETS_SOCKETS_EPOLL_REACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sockets-reactor-interrupter.h"
#include "sockets-events.h"

typedef struct socket_epoll_reactor socket_epoll_reactor_t;


extern int socket_epoll_reactor_add(socket_epoll_reactor_t *, socket_t, socket_event_t);
extern int socket_epoll_reactor_remove(socket_epoll_reactor_t *, socket_t);

extern int socket_epoll_reactor_run(socket_epoll_reactor_t *, struct socket_event *, int, const struct socket_timeout *);
extern void socket_epoll_reactor_cancel(socket_epoll_reactor_t *);

extern socket_epoll_reactor_t *socket_epoll_reactor_new(void);
extern void socket_epoll_reactor_free(socket_epoll_reactor_t *);

#ifdef __cplusplus
}
#endif

#endif
