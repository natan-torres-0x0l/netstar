#include "sockets-poll-reactor.h"
#include "sockets-handler.h"
#include "sockets-errors.h"

#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stddef.h>

#define SOCKET_POLL_REACTOR_EVENTS 256

struct socket_poll_reactor {
  socket_reactor_interrupter_t interrupter;

  struct pollfd *pollfds;
  int pollfds_count;
  size_t pollfds_size;
};


static int
socket_poll_reactor_events(socket_poll_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count) {
  int pollfd, activated = 0;

  memset(socket_events, 0, sizeof(struct socket_event)*(size_t)socket_events_count);

  for (pollfd = 0; pollfd < reactor->pollfds_count && activated < socket_events_count; pollfd++) {
    if (reactor->pollfds[pollfd].revents & POLLIN) {
      if ((socket_t)reactor->pollfds[pollfd].fd == reactor->interrupter[0])
        break;

      socket_events[activated].events |= SOCKET_READ;
    }

    if (reactor->pollfds[pollfd].revents & POLLOUT)
      socket_events[activated].events |= SOCKET_WRITE;

    if (reactor->pollfds[pollfd].revents & (POLLERR|POLLHUP))
      socket_events[activated].events |= SOCKET_EXCEPT;

    if (socket_events[activated].events) {
      socket_events[activated].socket = (socket_t)reactor->pollfds[pollfd].fd;
      activated++;
    }
  }

  return activated;
}

int
socket_poll_reactor_run(socket_poll_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *socket_timeout) {
  int activated = -1;

  activated = socket_poll(reactor->pollfds, (size_t)reactor->pollfds_count, socket_timeout);
  if (activated < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }
  if (activated == 0) {
    socket_seterror(SOCKET_ETIMEOUT);
    return 0;
  }

  socket_events_count = ((activated > socket_events_count) ? socket_events_count : activated);

  return socket_poll_reactor_events(reactor, socket_events, socket_events_count);
}

void
socket_poll_reactor_cancel(socket_poll_reactor_t *reactor) {
  socket_reactor_interrupter_interrupt(reactor->interrupter);
}

static struct pollfd *
socket_poll_reactor_pollfd(socket_poll_reactor_t *reactor, int socket_descriptor) {
  int pollfd;

  for (pollfd = 0; pollfd < reactor->pollfds_count; pollfd++)
    if (reactor->pollfds[pollfd].fd == socket_descriptor)
      return &reactor->pollfds[pollfd];

  return &reactor->pollfds[reactor->pollfds_count];
}

int
socket_poll_reactor_add(socket_poll_reactor_t *reactor, socket_t socket, socket_event_t socket_events) {
  struct pollfd *pollfd = NULL;

// if (socket_poll_reactor_requires_reallocation(reactor) && socket_poll_reactor_capacity_reallocatr(reactor) == -1)
//   return;

  if (reactor->pollfds_count >= reactor->pollfds_size)
    return -1;
  if (socket == SOCKET_INVALID)
    return -1;

  pollfd = socket_poll_reactor_pollfd(reactor, (int)socket);
  memset(pollfd, 0, sizeof(struct pollfd));

  if (socket_events & SOCKET_READ)
    pollfd->events |= POLLIN;

  if (socket_events & SOCKET_WRITE)
    pollfd->events |= POLLOUT;

  if (socket_events & SOCKET_EXCEPT)
    pollfd->events |= POLLERR|POLLHUP;

  pollfd->fd = socket;

  reactor->pollfds_count++;
  return 1;
}

int
socket_poll_reactor_remove(socket_poll_reactor_t *reactor, socket_t socket) {
  int pollfd, shift;

  for (pollfd = 0; pollfd < reactor->pollfds_count; pollfd++)
    if (reactor->pollfds[pollfd].fd == (int)socket) {
      if ((shift = reactor->pollfds_count-pollfd-1) > 0)
        memmove(&reactor->pollfds[pollfd], &reactor->pollfds[pollfd+1], (size_t)shift*sizeof(struct pollfd));

      reactor->pollfds_count--;

      reactor->pollfds[reactor->pollfds_count].fd = -1;
      reactor->pollfds[reactor->pollfds_count].events = 0;
      reactor->pollfds[reactor->pollfds_count].revents = 0;

      return 1;
    }

  return -1;
}

socket_poll_reactor_t *
socket_poll_reactor_new(void) {
  socket_poll_reactor_t *reactor = NULL;

  if (!(reactor = (socket_poll_reactor_t *)calloc(1, sizeof(socket_poll_reactor_t)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (!(reactor->pollfds = (struct pollfd *)calloc(SOCKET_POLL_REACTOR_EVENTS, sizeof(struct pollfd)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  reactor->pollfds_size = SOCKET_POLL_REACTOR_EVENTS;

  if (socket_reactor_interrupter_new(reactor->interrupter) == -1)
    goto _return;

  socket_poll_reactor_add(reactor, reactor->interrupter[0], SOCKET_READ);

  return reactor;

_return:
  socket_poll_reactor_free(reactor);
  return NULL;
}

void
socket_poll_reactor_free(socket_poll_reactor_t *reactor) {
  if (reactor) {
    socket_reactor_interrupter_close(reactor->interrupter);

    if (reactor->pollfds)
      free(reactor->pollfds), reactor->pollfds = NULL;

    free(reactor);
  }
}

#if defined SOCKET_HANDLER_POLL_REACTOR
struct socket_reactor_base socket_reactor_base = {
  socket_poll_reactor_new,
  socket_poll_reactor_free,
  socket_poll_reactor_run,
  socket_poll_reactor_cancel,
  socket_poll_reactor_add,
  socket_poll_reactor_remove
};

#endif
