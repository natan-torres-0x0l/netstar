#include "sockets-epoll-reactor.h"
#include "sockets-handler.h"
#include "sockets-errors.h"

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stddef.h>

#include <unistd.h>

#define SOCKET_EPOLL_REACTOR_EVENTS 256

struct socket_epoll_reactor {
  socket_reactor_interrupter_t interrupter;

  struct epoll_event *events;
  int events_count;

  int epoll;
};


static int
socket_epoll_reactor_events(socket_epoll_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count) {
  int activated = 0, event;

  memset(socket_events, 0, sizeof(struct socket_event)*(size_t)socket_events_count);

  for (event = 0; event < socket_events_count; event++) {
    if ((socket_t)reactor->events[event].data.fd == reactor->interrupter[0])
      break;

    if (reactor->events[event].events & EPOLLOUT)
      socket_events[activated].events |= SOCKET_WRITE;

    if (reactor->events[event].events & EPOLLIN)
      socket_events[activated].events |= SOCKET_READ;

    if (reactor->events[event].events & (EPOLLERR|EPOLLHUP))
      socket_events[activated].events |= SOCKET_EXCEPT;

    if (socket_events[activated].events) {
      socket_events[activated].socket = (socket_t)reactor->events[event].data.fd;
      activated++;
    }
  }

  return activated;
}

int
socket_epoll_reactor_run(socket_epoll_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *timeout) {
  int timeval = (int)((timeout) ? (timeout->sec*1000)+(timeout->usec) : -1);
  int activated = -1;

  socket_events_count = ((socket_events_count > reactor->events_count) ? reactor->events_count : socket_events_count);

  activated = epoll_wait(reactor->epoll, reactor->events, socket_events_count, timeval);
  if (activated == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }
  if (activated == 0) {
    socket_seterror(SOCKET_ETIMEOUT);
    return 0;
  }

  socket_events_count = ((activated > socket_events_count) ? socket_events_count : activated);

  return socket_epoll_reactor_events(reactor, socket_events, socket_events_count);
}

void
socket_epoll_reactor_cancel(socket_epoll_reactor_t *reactor) {
  socket_reactor_interrupter_interrupt(reactor->interrupter);
}

static int
socket_epoll_reactor_ctl(int epollfd, int opcode, socket_t socket, struct epoll_event *event) {
  system_error_t syscall_error;

  if (epoll_ctl(epollfd, opcode, socket, event) != -1)
    return 1;

  syscall_error = system_geterror();
  switch (opcode) {
    case EPOLL_CTL_ADD:
      if (syscall_error == EEXIST) {
        if (epoll_ctl(epollfd, EPOLL_CTL_MOD, socket, event) == 0)
  				return 1;
      }

      break;

    case EPOLL_CTL_MOD:
      if (syscall_error == ENOENT) {
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket, event) == 0)
  				return 1;
      }

      break;

 // case EPOLL_CTL_DEL:
 //   break;
  }

  return -1;
}

int
socket_epoll_reactor_add(socket_epoll_reactor_t *reactor, socket_t socket, socket_event_t socket_events) {
  struct epoll_event event = { .data = { .fd = socket, }, };

  if (socket_events & SOCKET_READ)
    event.events |= EPOLLIN;

  if (socket_events & SOCKET_WRITE)
    event.events |= EPOLLOUT;

  if (socket_events & SOCKET_EXCEPT)
    event.events |= EPOLLERR|EPOLLHUP;

  return socket_epoll_reactor_ctl(reactor->epoll, EPOLL_CTL_ADD, socket, &event);
}

int
socket_epoll_reactor_remove(socket_epoll_reactor_t *reactor, socket_t socket) {
  struct epoll_event event = { .data = { .fd = socket, }, };

  return socket_epoll_reactor_ctl(reactor->epoll, EPOLL_CTL_DEL, socket, &event);
}

socket_epoll_reactor_t *
socket_epoll_reactor_new(void) {
  socket_epoll_reactor_t *reactor = NULL;

  if (!(reactor = (socket_epoll_reactor_t *)calloc(1, sizeof(socket_epoll_reactor_t)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if ((reactor->epoll = epoll_create1(0)) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (!(reactor->events = (struct epoll_event *)calloc(SOCKET_EPOLL_REACTOR_EVENTS, sizeof(struct epoll_event)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }
  reactor->events_count = SOCKET_EPOLL_REACTOR_EVENTS;

  if (socket_reactor_interrupter_new(reactor->interrupter) == -1)
    goto _return;

  socket_epoll_reactor_add(reactor, reactor->interrupter[0], SOCKET_READ);

  return reactor;

_return:
  socket_epoll_reactor_free(reactor);
  return NULL;
}

void
socket_epoll_reactor_free(socket_epoll_reactor_t *reactor) {
  if (reactor) {
    socket_reactor_interrupter_close(reactor->interrupter);

    if (reactor->events)
      free(reactor->events), reactor->events = NULL;

    if (reactor->epoll != -1)
      close(reactor->epoll), reactor->epoll = -1;

    free(reactor);
  }
}

#if defined SOCKET_HANDLER_EPOLL_REACTOR
struct socket_reactor_base socket_reactor_base = {
  socket_epoll_reactor_new,
  socket_epoll_reactor_free,
  socket_epoll_reactor_run,
  socket_epoll_reactor_cancel,
  socket_epoll_reactor_add,
  socket_epoll_reactor_remove
};

#endif
