#ifndef _NETWORK_NETWORK_ENDPOINT_H
#define _NETWORK_NETWORK_ENDPOINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "network.h"

typedef socklen_t network_endpoint_size_t;

struct network_endpoint {
  union {
    struct sockaddr_storage storage;
    struct sockaddr base;
    struct sockaddr_in in4;
    struct sockaddr_in6 in6;
  };

  network_endpoint_size_t size;
};


extern network_ipaddr4_t *network_endpoint_addr4(struct network_endpoint *);
extern network_ipaddr6_t *network_endpoint_addr6(struct network_endpoint *);

extern uint16_t network_endpoint_port(struct network_endpoint *);

extern int network_endpoint_new(struct network_endpoint *, const char *, uint16_t);

#ifdef __cplusplus
}
#endif

#endif
