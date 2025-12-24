#ifndef _NETWORK_SOCKETS_SOCKETS_SELECT_REACTOR_H
#define _NETWORK_SOCKETS_SOCKETS_SELECT_REACTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sockets-reactor-interrupter.h"
#include "sockets-events.h"

typedef struct socket_select_reactor socket_select_reactor_t;


extern int socket_select_reactor_add(socket_select_reactor_t *, socket_t, socket_event_t);
extern int socket_select_reactor_remove(socket_select_reactor_t *, socket_t);

extern int socket_select_reactor_run(socket_select_reactor_t *, struct socket_event *, int, const struct socket_timeout *);
extern void socket_select_reactor_cancel(socket_select_reactor_t *);

extern int socket_select_reactor_initialize(socket_select_reactor_t *);
extern void socket_select_reactor_free(socket_select_reactor_t *);

#ifdef __cplusplus
}
#endif

#endif
