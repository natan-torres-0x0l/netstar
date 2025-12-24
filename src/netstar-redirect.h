#ifndef _NETSTAR_REDIRECT_H
#define _NETSTAR_REDIRECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/hashset/hashset.h>

#include <netstar-hosts.h>
#include <netstar.h>

typedef enum {
  NETSTAR_REDIRECT_PROTOCOL_TCP = NETSTAR_IP_PROTOCOL_TCP,  NETSTAR_REDIRECT_PROTOCOL_UDP = NETSTAR_IP_PROTOCOL_UDP, NETSTAR_REDIRECT_PROTOCOL_UNKNOWN = 0
} netstar_redirect_protocol_t;

struct netstar_redirect_preroute {
  netstar_redirect_protocol_t protocol;
  uint16_t dport, rport;
};

typedef hashset_t * netstar_redirect_preroutes_t;


extern int netstar_redirect_preroutes_add(netstar_redirect_preroutes_t, struct netstar_redirect_preroute *);


struct netstar_redirect {
  netstar_redirect_preroutes_t preroutes;
  netstar_hosts_t preexcluded_hosts;

  netstar_t *netstar;
};


extern int netstar_redirect_new(struct netstar_redirect *, netstar_t *);
extern void netstar_redirect_free(struct netstar_redirect *);

#ifdef __cplusplus
}
#endif

#endif
