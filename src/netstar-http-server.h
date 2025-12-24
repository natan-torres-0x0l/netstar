#ifndef _NETSTAR_HTTP_SERVER_H
#define _NETSTAR_HTTP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/hashset/hashset.h>
#include <network/sockets/sockets.h>

#include <netstar-threads.h>
#include <netstar.h>


struct netstar_http_server {
  struct network_endpoint addr;
  socket_t socket;

  int backlog;

  hashset_t *handlers;

  netstar_thread_t *thread;
};


extern int netstar_http_server_new(struct netstar_http_server *);
extern void netstar_http_server_free(struct netstar_http_server *);

#ifdef __cplusplus
}
#endif

#endif
