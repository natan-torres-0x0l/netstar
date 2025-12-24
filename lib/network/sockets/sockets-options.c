#include "sockets-options.h"
#include "sockets.h"


int
socket_setreuseaddr(socket_t socket, bool reuseaddr) {
  int value = ((reuseaddr) ? 1 : 0);
  return socket_setoption(socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
}

int
socket_setkeepalive(socket_t socket, bool keepalive) {
  int value = ((keepalive) ? 1 : 0);
  return socket_setoption(socket, SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value));
}

int
socket_setsndbuf(socket_t socket, int buffer) {
  return socket_setoption(socket, SOL_SOCKET, SO_SNDBUF, &buffer, sizeof(buffer));
}

int
socket_setrcvbuf(socket_t socket, int buffer) {
  return socket_setoption(socket, SOL_SOCKET, SO_RCVBUF, &buffer, sizeof(buffer));
}

int
socket_setlinger(socket_t socket, struct linger *linger) {
  return socket_setoption(socket, SOL_SOCKET, SO_LINGER, linger, sizeof(struct linger));
}

int
socket_setbroadcast(socket_t socket, bool broadcast) {
  int value = ((broadcast) ? 1 : 0);
  return socket_setoption(socket, SOL_SOCKET, SO_BROADCAST, &value, sizeof(value));
}

int
socket_setipttl(socket_t socket, int ttl) {
  return socket_setoption(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
}

int
socket_setiptos(socket_t socket, int tos) {
  return socket_setoption(socket, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
}

int
socket_setipv6only(socket_t socket, bool ipv6only) {
  int value = ((ipv6only) ? 1 : 0);
  return socket_setoption(socket, IPPROTO_IPV6, IPV6_V6ONLY, &value, sizeof(value));
}

int
socket_settcpnodelay(socket_t socket, bool nodelay) {
  int value = ((nodelay) ? 1 : 0);
  return socket_setoption(socket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}
