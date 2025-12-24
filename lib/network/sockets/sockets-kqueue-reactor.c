#include "sockets-kqueue-reactor.h"
#include "sockets-handler.h"
#include "sockets-errors.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_KQUEUE_REACTOR_EVENTS 256

struct socket_kqueue_reactor {
  socket_reactor_interrupter_t interrupter;

  struct kevent *events;
  int events_count;

  int kqueue;
};


static int
socket_kqueue_reactor_events(socket_kqueue_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count) {
  int activated = 0, event;

  memset(socket_events, 0, sizeof(struct socket_event)*(size_t)socket_events_count);

  for (event = 0; event < socket_events_count; event++) {
    if ((socket_t)reactor->events[event].ident == reactor->interrupter[0])
      break;

    if (reactor->events[event].filter == EVFILT_READ)
      socket_events[activated].events |= SOCKET_READ;

    if (reactor->events[event].filter == EVFILT_WRITE)
      socket_events[activated].events |= SOCKET_WRITE;

    if (reactor->events[event].udata && (reactor->events[event].flags & EV_ERROR || reactor->events[event].flags & EV_EOF))
      socket_events[activated].events |= SOCKET_EXCEPT;

    if (socket_events[activated].events) {
      socket_events[activated].socket = (socket_t)reactor->events[event].ident;
      activated++;
    }
  }

  return activated;
}

static inline struct timespec *
socket_kqueue_reactor_timespec_new(struct timespec *timespec, const struct socket_timeout *timeout) {
  timespec->tv_sec = timeout->sec;
  timespec->tv_nsec = timeout->usec*1000000;

  return timespec;
}

int
socket_kqueue_reactor_run(socket_kqueue_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *timeout) {
  struct timespec timespec = {0}, *timespecptr = ((timeout) ? socket_kqueue_reactor_timespec_new(&timespec, timeout) : NULL);
  int activated = -1;

  socket_events_count = ((socket_events_count > reactor->events_count) ? reactor->events_count : socket_events_count);
  memset(reactor->events, 0, sizeof(struct kevent)*(size_t)reactor->events_count);

  activated = kevent(reactor->kqueue, NULL, 0, reactor->events, socket_events_count, timespecptr);
  if (activated == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }
  if (activated == 0) {
    socket_seterror(SOCKET_ETIMEOUT);
    return 0;
  }

  socket_events_count = ((activated > socket_events_count) ? socket_events_count : activated);

  return socket_kqueue_reactor_events(reactor, socket_events, socket_events_count);
}

void
socket_kqueue_reactor_cancel(socket_kqueue_reactor_t *reactor) {
  socket_reactor_interrupter_interrupt(reactor->interrupter);
}

int
socket_kqueue_reactor_add(socket_kqueue_reactor_t *reactor, socket_t socket, socket_event_t socket_events) {
  struct kevent events[2] = {0};
  int events_count = 0;

  socket_kqueue_reactor_remove(reactor, socket);

  if (socket_events & SOCKET_READ || socket_events & SOCKET_EXCEPT) {
    EV_SET(&events[events_count], (int)socket, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, ((socket_events & SOCKET_EXCEPT) ? reactor : NULL));
    events_count++;
  }

  if (socket_events & SOCKET_WRITE) {
    EV_SET(&events[events_count], (int)socket, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, NULL);
    events_count++;
  }

  if (kevent(reactor->kqueue, events, events_count, NULL, 0, NULL) == -1)
    return -1;

  return 1;
}

int
socket_kqueue_reactor_remove(socket_kqueue_reactor_t *reactor, socket_t socket) {
  struct kevent events[2] = {0};
  int events_count = 0;

  int status = -1;

  EV_SET(&events[events_count++], (int)socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  EV_SET(&events[events_count++], (int)socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

  status = kevent(reactor->kqueue, events, events_count, NULL, 0, NULL);
  if (status == -1 && system_geterror() != ENOENT)
    return -1;

  return 1;
}

socket_kqueue_reactor_t *
socket_kqueue_reactor_new(void) {
  socket_kqueue_reactor_t *reactor = NULL;

  if (!(reactor = (socket_kqueue_reactor_t *)calloc(1, sizeof(socket_kqueue_reactor_t)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if ((reactor->kqueue = kqueue()) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (!(reactor->events = (struct kevent *)calloc(SOCKET_KQUEUE_REACTOR_EVENTS, sizeof(struct kevent)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }
  reactor->events_count = SOCKET_KQUEUE_REACTOR_EVENTS;

  if (socket_reactor_interrupter_new(reactor->interrupter) == -1)
    goto _return;

  socket_kqueue_reactor_add(reactor, reactor->interrupter[0], SOCKET_READ);

  return reactor;

_return:
  socket_kqueue_reactor_free(reactor);
  return NULL;
}

void
socket_kqueue_reactor_free(socket_kqueue_reactor_t *reactor) {
  if (reactor) {
    socket_reactor_interrupter_close(reactor->interrupter);

    if (reactor->events)
      free(reactor->events), reactor->events = NULL;

    if (reactor->kqueue != -1)
      close(reactor->kqueue), reactor->kqueue = -1;

    free(reactor);
  }
}

#if defined SOCKET_HANDLER_KQUEUE_REACTOR
struct socket_reactor_base socket_reactor_base = {
  socket_kqueue_reactor_new,
  socket_kqueue_reactor_free,
  socket_kqueue_reactor_run,
  socket_kqueue_reactor_cancel,
  socket_kqueue_reactor_add,
  socket_kqueue_reactor_remove
};

#endif
