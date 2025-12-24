#include "sockets-handler.h"


extern struct socket_reactor_base socket_reactor_base;


int
socket_reactor_add(socket_reactor_t *reactor, socket_t socket, socket_event_t socket_event) {
  return socket_reactor_base.add(reactor, socket, socket_event);
}

int
socket_reactor_remove(socket_reactor_t *reactor, socket_t socket) {
  return socket_reactor_base.remove(reactor, socket);
}

int
socket_reactor_poll(socket_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *timeout) {
  return socket_reactor_base.run(reactor, socket_events, socket_events_count, timeout);
}

void
socket_reactor_cancel(socket_reactor_t *reactor) {
  return socket_reactor_base.cancel(reactor);
}

socket_reactor_t *
socket_reactor_new(void) {
  return socket_reactor_base.initialize();
}

void
socket_reactor_free(socket_reactor_t *reactor) {
  socket_reactor_base.free(reactor);
}
