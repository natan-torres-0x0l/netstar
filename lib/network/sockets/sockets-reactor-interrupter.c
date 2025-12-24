#include "sockets-reactor-interrupter.h"
#include "sockets.h"


int
socket_reactor_interrupter_interrupt(socket_reactor_interrupter_t interrupter) {
  return socket_send(interrupter[1], "c", 1, 0);
}

static inline void
socket_reactor_interrupter_initialize(socket_reactor_interrupter_t interrupter) {
  interrupter[0] = SOCKET_INVALID;
  interrupter[1] = SOCKET_INVALID;
}

int
socket_reactor_interrupter_new(socket_reactor_interrupter_t interrupter) {
  struct sockaddr_in server_addr = {0}, remote_addr = {0};
  socklen_t remote_addr_length = sizeof(struct sockaddr_in);

  socket_t socket_listener = SOCKET_INVALID;
  int reuseaddr, nodelay;

  socket_reactor_interrupter_initialize(interrupter);

  if ((socket_listener = socket_new(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_INVALID)
    goto _return;

  reuseaddr = 1;
  if (socket_setoption(socket_listener, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1)
    goto _return;

  inetv4_parse((inetv4_t *)&server_addr.sin_addr.s_addr, "127.0.0.1");
  server_addr.sin_port = 0;
  server_addr.sin_family = AF_INET;

  if (socket_bind(socket_listener, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    goto _return;

  if (socket_localaddr(socket_listener, (struct sockaddr *)&remote_addr, &remote_addr_length) == -1)
    goto _return;

  if (socket_listen(socket_listener, 1) == -1)
    goto _return;

  if ((interrupter[1] = socket_new(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_INVALID)
    goto _return;

  socket_connect(interrupter[1], (struct sockaddr *)&remote_addr, sizeof(struct sockaddr_in), NULL);

  nodelay = 1;
  if (socket_setoption(interrupter[1], IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) == -1)
    goto _return;

  if ((interrupter[0] = socket_accept(socket_listener, NULL, NULL, NULL)) == SOCKET_INVALID)
    goto _return;

  nodelay = 1;
  if (socket_setoption(interrupter[0], IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) == -1)
    goto _return;

  socket_close(socket_listener);
  return 1;

_return:
  socket_reactor_interrupter_close(interrupter);
  socket_close(socket_listener);

  return -1;
}

void
socket_reactor_interrupter_close(socket_reactor_interrupter_t interrupter) {
  if (interrupter) {
    if (interrupter[0] != SOCKET_INVALID) {
      socket_close(interrupter[0]);
      interrupter[0] = SOCKET_INVALID;
    }
    if (interrupter[1] != SOCKET_INVALID) {
      socket_close(interrupter[1]);
      interrupter[1] = SOCKET_INVALID;
    }
  }
}
