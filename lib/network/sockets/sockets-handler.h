#ifndef _NETWORK_SOCKETS_SOCKETS_HANDLER_H
#define _NETWORK_SOCKETS_SOCKETS_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sockets-events.h"

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
// #include "sockets-iocp-service.h"
  #include "sockets-afd-reactor.h"

#elif (defined __APPLE__ && defined __MACH__) || (defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD)
  #include "sockets-kqueue-reactor.h"

#elif defined __linux__ || defined __linux
  #include "sockets-epoll-reactor.h"

#endif

#include "sockets-select-reactor.h"
#include "sockets-poll-reactor.h"

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #define SOCKET_HANDLER_AFD_REACTOR

#elif (defined __APPLE__ && defined __MACH__) || (defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD)
  #define SOCKET_HANDLER_KQUEUE_REACTOR

#elif defined __linux__ || defined __linux
  #define SOCKET_HANDLER_EPOLL_REACTOR

#else // defined __unix__ || defined __unix
  #define SOCKET_HANDLER_SELECT_REACTOR
// #define SOCKET_HANDLER_POLL_REACTOR

#endif

#if defined SOCKET_HANDLER_AFD_REACTOR
typedef socket_afd_reactor_t socket_reactor_t;

#elif defined SOCKET_HANDLER_KQUEUE_REACTOR
typedef socket_kqueue_reactor_t socket_reactor_t;

#elif defined SOCKET_HANDLER_EPOLL_REACTOR
typedef socket_epoll_reactor_t socket_reactor_t;

#elif defined SOCKET_HANDLER_SELECT_REACTOR
typedef socket_select_reactor_t socket_reactor_t;

#elif defined SOCKET_HANDLER_POLL_REACTOR
typedef socket_poll_reactor_t socket_reactor_t;

#endif

typedef socket_reactor_t *(*socket_reactor_initialize_t)(void);
typedef void (*socket_reactor_free_t)(socket_reactor_t *);
typedef int (*socket_reactor_run_t)(socket_reactor_t *, struct socket_event *, int, const struct socket_timeout *);
typedef int (*socket_reactor_add_t)(socket_reactor_t *, socket_t, socket_event_t);
typedef int (*socket_reactor_remove_t)(socket_reactor_t *, socket_t);
typedef void (*socket_reactor_cancel_t)(socket_reactor_t *);

struct socket_reactor_base {
  socket_reactor_initialize_t initialize;
  socket_reactor_free_t free;
  socket_reactor_run_t run;
  socket_reactor_cancel_t cancel;
  socket_reactor_add_t add;
  socket_reactor_remove_t remove;
};


extern int socket_reactor_add(socket_reactor_t *, socket_t, socket_event_t);
extern int socket_reactor_remove(socket_reactor_t *, socket_t);

extern int socket_reactor_poll(socket_reactor_t *, struct socket_event *, int, const struct socket_timeout *);

extern void socket_reactor_cancel(socket_reactor_t *);

extern socket_reactor_t *socket_reactor_new(void);
extern void socket_reactor_free(socket_reactor_t *);

#ifdef __cplusplus
}
#endif

#endif
