#include "network.h"

#include <string.h>


network_ipaddr4_t *
network_endpoint_addr4(struct network_endpoint *endpoint) {
  return (network_ipaddr4_t *)&endpoint->in4.sin_addr;
}

network_ipaddr6_t *
network_endpoint_addr6(struct network_endpoint *endpoint) {
  return (network_ipaddr6_t *)&endpoint->in6.sin6_addr;
}

uint16_t
network_endpoint_port(struct network_endpoint *endpoint) {
	return ntohs((endpoint->base.sa_family == AF_INET6) ? endpoint->in6.sin6_port : endpoint->in4.sin_port);
}

int
network_endpoint_new(struct network_endpoint *endpoint, const char *addr, uint16_t port) {
  memset(endpoint, 0, sizeof(struct network_endpoint));

  switch (network_ipaddr(addr)) {
    case NETWORK_IPADDR4:
      network_ipaddr4_parse((network_ipaddr4_t *)&endpoint->in4.sin_addr, addr);
      endpoint->in4.sin_port = htons(port);
      endpoint->in4.sin_family = AF_INET;

      endpoint->size = sizeof(struct sockaddr_in);

      return 1;

    case NETWORK_IPADDR6:
      network_ipaddr6_parse((network_ipaddr6_t *)&endpoint->in6.sin6_addr, addr);
      endpoint->in6.sin6_port = htons(port);
      endpoint->in6.sin6_family = AF_INET6;
      endpoint->in6.sin6_scope_id = 0;

      endpoint->size = sizeof(struct sockaddr_in6);

      return 1;
  }

  return -1;
}
