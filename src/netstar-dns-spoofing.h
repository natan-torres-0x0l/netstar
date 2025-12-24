#ifndef _NETSTAR_DNS_SPOOFING_H
#define _NETSTAR_DNS_SPOOFING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/hashset/hashset.h>
#include <network/dns/dns.h>

#include <netstar-threads.h>
#include <netstar-time.h>

#include <netstar-hosts.h>

#include <netstar.h>

struct netstar_dns_spoofing_resource {
  char name[NETWORK_DNS_DOMAIN_LENGTH];
  network_dns_type_t rtype;
  network_dns_class_t rclass;
  char result[NETWORK_DNS_DOMAIN_LENGTH];
};

typedef hashset_t * netstar_dns_spoofing_resources_t;


extern int netstar_dns_spoofing_resources_add(netstar_dns_spoofing_resources_t, struct netstar_dns_spoofing_resource *);


struct netstar_dns_spoofing {
  netstar_dns_spoofing_resources_t resources;

  netstar_hosts_t protected_hosts;
  netstar_hosts_t target_hosts;

  bool status;

  netstar_t *netstar;
};


extern int netstar_dns_spoofing_new(struct netstar_dns_spoofing *, netstar_t *);
extern void netstar_dns_spoofing_free(struct netstar_dns_spoofing *);

#ifdef __cplusplus
}
#endif

#endif
