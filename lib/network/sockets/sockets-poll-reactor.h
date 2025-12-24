#ifndef _NETWORK_SOCKETS_POLL_REACTOR_H
#define _NETWORK_SOCKETS_POLL_REACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sockets-reactor-interrupter.h"
#include "sockets-events.h"

typedef struct socket_poll_reactor socket_poll_reactor_t;


extern int socket_poll_reactor_add(socket_poll_reactor_t *, socket_t, socket_event_t);
extern int socket_poll_reactor_remove(socket_poll_reactor_t *, socket_t);

extern int socket_poll_reactor_run(socket_poll_reactor_t *, struct socket_event *, int, const struct socket_timeout *);
extern void socket_poll_reactor_cancel(socket_poll_reactor_t *);

extern socket_poll_reactor_t *socket_poll_reactor_new(void);
extern void socket_poll_reactor_free(socket_poll_reactor_t *);

#ifdef __cplusplus
}
#endif

#endif
