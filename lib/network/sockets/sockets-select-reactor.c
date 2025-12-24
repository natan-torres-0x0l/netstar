#include "sockets-select-reactor.h"
#include "sockets-handler.h"
#include "sockets-errors.h"

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stddef.h>

struct socket_select_reactor {
  socket_reactor_interrupter_t interrupter;

  fd_set readset_events;

  fd_set writeset_events;

  fd_set exceptset_events;

  int select_descriptor;
};


static int
socket_select_reactor_events(socket_select_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count) {
  int socket_descriptor = -1, activated = 0;

  memset(socket_events, 0, sizeof(struct socket_event)*(size_t)socket_events_count);

  for (socket_descriptor = 0; socket_descriptor <= reactor->select_descriptor && activated < socket_events_count; socket_descriptor++) {
    if (FD_ISSET(socket_descriptor, &reactor->readset_events)) {
   // if (socket_descriptor == reactor->interrupter[0])
   //   break;

      socket_events[activated].events |= SOCKET_READ;
    }

    if (FD_ISSET(socket_descriptor, &reactor->writeset_events))
      socket_events[activated].events |= SOCKET_WRITE;

    if (FD_ISSET(socket_descriptor, &reactor->exceptset_events))
      socket_events[activated].events |= SOCKET_EXCEPT;

    if (socket_events[activated].events) {
      socket_events[activated].socket = (socket_t)socket_descriptor;
      activated++;
    }
  }

  return activated;
}

int
socket_select_reactor_run(socket_select_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *socket_timeout) {
  fd_set readset_events = reactor->readset_events, writeset_events = reactor->writeset_events, exceptset_events = reactor->exceptset_events;
  int activated = -1;

  activated = socket_select(reactor->select_descriptor+1, &reactor->readset_events, NULL /*&reactor->writeset_events*/, NULL/*&reactor->exceptset_events*/, socket_timeout);
  if (activated == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }
  if (activated == 0) {
    socket_seterror(SOCKET_ETIMEOUT);
    goto _return;
  }

  activated = socket_select_reactor_events(reactor, socket_events, socket_events_count);

_return:
  reactor->readset_events = readset_events;
  reactor->writeset_events = writeset_events;
  reactor->exceptset_events = exceptset_events;

  return activated;
}

void
socket_select_reactor_cancel(socket_select_reactor_t *reactor) {
  socket_reactor_interrupter_interrupt(reactor->interrupter);
}

int
socket_select_reactor_add(socket_select_reactor_t *reactor, socket_t socket, socket_event_t socket_events) {
  if (socket_events & SOCKET_READ) {
    FD_SET(socket, &reactor->readset_events);
  }

  if (socket_events & SOCKET_WRITE) {
    FD_SET(socket, &reactor->writeset_events);
  }

  if (socket_events & SOCKET_EXCEPT) {
    FD_SET(socket, &reactor->exceptset_events);
  }

  if (reactor->select_descriptor < socket) {
    reactor->select_descriptor = (int)socket;
  }

  return 1;
}

static inline bool
socket_select_reactor_select_descriptor_active(socket_select_reactor_t *reactor, int socket_descriptor) {
  return FD_ISSET(socket_descriptor, &reactor->readset_events) || FD_ISSET(socket_descriptor, &reactor->writeset_events) || FD_ISSET(socket_descriptor, &reactor->exceptset_events);
}

static void
socket_select_reactor_select_descriptor(socket_select_reactor_t *reactor) {
  int socket_descriptor, select_descriptor = -1;

  for (socket_descriptor = reactor->select_descriptor-1; socket_descriptor >= 0; socket_descriptor--)
    if (socket_select_reactor_select_descriptor_active(reactor, socket_descriptor)) {
      select_descriptor = socket_descriptor;
      break;
    }

  reactor->select_descriptor = select_descriptor;
}

int
socket_select_reactor_remove(socket_select_reactor_t *reactor, socket_t socket) {
  if (socket > reactor->select_descriptor || socket > FD_SETSIZE || socket == SOCKET_INVALID)
    return -1;

  FD_CLR(socket, &reactor->readset_events);

  FD_CLR(socket, &reactor->writeset_events);

  FD_CLR(socket, &reactor->exceptset_events);

  if (socket == reactor->select_descriptor)
    socket_select_reactor_select_descriptor(reactor);

  return 1;
}

socket_select_reactor_t *
socket_select_reactor_new(void) {
  socket_select_reactor_t *reactor = NULL;

  if (!(reactor = (socket_select_reactor_t *)calloc(1, sizeof(socket_select_reactor_t)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  reactor->select_descriptor = -1;

  if (socket_reactor_interrupter_new(reactor->interrupter) == -1)
    goto _return;

  socket_select_reactor_add(reactor, reactor->interrupter[0], SOCKET_READ);

  return reactor;

_return:
  socket_select_reactor_free(reactor);
  return NULL;
}

void
socket_select_reactor_free(socket_select_reactor_t *reactor) {
  if (reactor) {
    socket_reactor_interrupter_close(reactor->interrupter);

    free(reactor);
  }
}

#if defined SOCKET_HANDLER_SELECT_REACTOR
struct socket_reactor_base socket_reactor_base = {
  socket_select_reactor_new,
  socket_select_reactor_free,
  socket_select_reactor_run,
  socket_select_reactor_cancel,
  socket_select_reactor_add,
  socket_select_reactor_remove
};

#endif
