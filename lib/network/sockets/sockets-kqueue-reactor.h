#ifndef _NETWORK_SOCKETS_SOCKETS_KQUEUE_REACTOR_H
#define _NETWORK_SOCKETS_SOCKETS_KQUEUE_REACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sockets-reactor-interrupter.h"
#include "sockets-events.h"

typedef struct socket_kqueue_reactor socket_kqueue_reactor_t;


extern int socket_kqueue_reactor_add(socket_kqueue_reactor_t *, socket_t, socket_event_t);
extern int socket_kqueue_reactor_remove(socket_kqueue_reactor_t *, socket_t);

extern int socket_kqueue_reactor_run(socket_kqueue_reactor_t *, struct socket_event *, int, const struct socket_timeout *);
extern void socket_kqueue_reactor_cancel(socket_kqueue_reactor_t *);

extern socket_kqueue_reactor_t *socket_kqueue_reactor_new(void);
extern void socket_kqueue_reactor_free(socket_kqueue_reactor_t *);

#ifdef __cplusplus
}
#endif

#endif
