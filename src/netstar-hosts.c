#include "netstar-hosts.h"

#include <stdlib.h>
#include <string.h>


bool
netstar_host_equals(netstar_host_t *host1, netstar_host_t *host2) {
  if (host1->inet != host2->inet)
    return false;

  return !memcmp(host1, host2, sizeof(netstar_host_t));
}

int
netstar_host_new(netstar_host_t *host, network_macaddr_t *mac, network_ipaddr_t *addr, int inet) {
  host->mac = *mac;
  host->addr = *addr;
  host->inet = inet;

  return 1;
}


static int
netstar_hosts_compare(const void *value1, const void *value2) {
  netstar_host_t *host1 = (netstar_host_t *)value1, *host2 = (netstar_host_t *)value2;
  uint16_t *addr1 = (uint16_t *)&host1->addr, *addr2 = (uint16_t *)&host2->addr;

  if (host1->inet < host2->inet)
    return -1;
  else if (host1->inet > host2->inet)
    return 1;

  for (; addr1 <= host1->addr.v6.u16+sizeof(network_ipaddr_t)/sizeof(uint16_t); addr1++, addr2++)
    if (*addr1 < *addr2)
      return -1;
    else if (*addr1 > *addr2)
      return 1;

  return network_macaddr_compare(&host1->mac, &host2->mac);
}

netstar_hosts_iterator_t
netstar_hosts_begin(netstar_hosts_t hosts) {
  return set_begin(hosts);
}

netstar_hosts_iterator_t
netstar_hosts_end(netstar_hosts_t hosts) {
  return set_end(hosts);
}

netstar_hosts_iterator_t
netstar_hosts_next(netstar_hosts_iterator_t hosts) {
  return set_next(hosts);
}

netstar_hosts_iterator_t
netstar_hosts_prev(netstar_hosts_iterator_t hosts) {
  return set_prev(hosts);
}

netstar_host_t *
netstar_hosts_value(netstar_hosts_iterator_t host) {
  return (netstar_host_t *)set_value(host);
}

size_t
netstar_hosts_size(netstar_hosts_t hosts) {
  return set_size(hosts);
}

bool
netstar_hosts_empty(netstar_hosts_t hosts) {
  return set_empty(hosts);
}

netstar_host_t *
netstar_hosts_find(netstar_hosts_t hosts, netstar_host_t *host) {
  return (netstar_host_t *)set_find(hosts, host, sizeof(netstar_host_t));
}

netstar_host_t *
netstar_hosts_findmac(netstar_hosts_t hosts, const network_macaddr_t *mac) {
  netstar_hosts_iterator_t hosts_iter = NULL;

  for (hosts_iter = netstar_hosts_begin(hosts); hosts_iter; hosts_iter = netstar_hosts_next(hosts_iter)) {
    netstar_host_t *host = netstar_hosts_value(hosts_iter);

    if (!network_macaddr_compare(&host->mac, mac))
      return host;
  }

  return NULL;
}

netstar_host_t *
netstar_hosts_findipaddr4(netstar_hosts_t hosts, const network_ipaddr4_t *addr) {
  netstar_hosts_iterator_t hosts_iter = NULL;

  for (hosts_iter = netstar_hosts_begin(hosts); hosts_iter; hosts_iter = netstar_hosts_next(hosts_iter)) {
    netstar_host_t *host = netstar_hosts_value(hosts_iter);

    if (host->inet == NETWORK_IPADDR4 && !memcmp(&host->addr.v4, addr, NETWORK_IPADDR4_SIZE))
      return host;
  }

  return NULL;
}

netstar_host_t *
netstar_hosts_findipaddr6(netstar_hosts_t hosts, const network_ipaddr6_t *addr) {
  netstar_hosts_iterator_t hosts_iter = NULL;

  for (hosts_iter = netstar_hosts_begin(hosts); hosts_iter; hosts_iter = netstar_hosts_next(hosts_iter)) {
    netstar_host_t *host = netstar_hosts_value(hosts_iter);

    if (host->inet == NETWORK_IPADDR6 && !memcmp(&host->addr.v6, addr, NETWORK_IPADDR6_SIZE))
      return host;
  }

  return NULL;
}

int
netstar_hosts_add(netstar_hosts_t hosts, netstar_host_t *host) {
  return set_insert(hosts, set_clvalue(host, sizeof(netstar_host_t)));
}

int
netstar_hosts_remove(netstar_hosts_t hosts, netstar_host_t *host) {
  return set_remove(hosts, host, sizeof(netstar_host_t));
}

netstar_hosts_t
netstar_hosts_new(void) {
  netstar_hosts_t hosts = NULL;

  if (!(hosts = set_new(netstar_hosts_compare, free)))
    return NULL;

  return hosts;
}

void
netstar_hosts_free(netstar_hosts_t hosts) {
  set_free(hosts);
}
