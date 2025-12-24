#ifndef _NETSTAR_HOSTS_H
#define _NETSTAR_HOSTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/set/set.h>

#include <netstar.h>


typedef struct netstar_host {
	network_macaddr_t mac;
  network_ipaddr_t addr;
  int inet;
} netstar_host_t;


extern bool netstar_host_equals(netstar_host_t *, netstar_host_t *);

extern int netstar_host_new(netstar_host_t *, network_macaddr_t *, network_ipaddr_t *, int);


typedef set_iterator_t netstar_hosts_iterator_t;
typedef set_t * netstar_hosts_t;


extern netstar_hosts_iterator_t netstar_hosts_next(netstar_hosts_iterator_t);
extern netstar_hosts_iterator_t netstar_hosts_prev(netstar_hosts_iterator_t);

extern netstar_hosts_iterator_t netstar_hosts_begin(netstar_hosts_t);
extern netstar_hosts_iterator_t netstar_hosts_end(netstar_hosts_t);

extern netstar_host_t *netstar_hosts_value(netstar_hosts_iterator_t);

extern int netstar_hosts_add(netstar_hosts_t, netstar_host_t *);
extern int netstar_hosts_remove(netstar_hosts_t, netstar_host_t *);

extern netstar_host_t *netstar_hosts_findmac(netstar_hosts_t, const network_macaddr_t *);

extern netstar_host_t *netstar_hosts_findipaddr4(netstar_hosts_t, const network_ipaddr4_t *);
extern netstar_host_t *netstar_hosts_findipaddr6(netstar_hosts_t, const network_ipaddr6_t *);

extern netstar_host_t *netstar_hosts_find(netstar_hosts_t, netstar_host_t *);

extern size_t netstar_hosts_size(netstar_hosts_t);
extern bool netstar_hosts_empty(netstar_hosts_t);

extern netstar_hosts_t netstar_hosts_new(void);
extern void netstar_hosts_free(netstar_hosts_t);

#ifdef __cplusplus
}
#endif

#endif
