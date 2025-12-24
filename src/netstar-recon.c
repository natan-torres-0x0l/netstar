#include "netstar-recon.h"

#include <netstar-scanner.h>
#include <netstar-packet.h>

#include <string.h>
#include <stdio.h>


static int
netstar_recon_host4_reconnaissance(netstar_t *netstar, network_macaddr_t *mac, const network_ipaddr4_t *addr, ssize_t requests, netstar_time_t secs) {
  netstar_timer_t timer = NETSTAR_TIMER_INITIALIZER;

  char smac[NETWORK_MACADDR_STRLENGTH] = {0};
  char daddr[NETWORK_IPADDR4_STRLENGTH] = {0};

  int status = -1;

  network_ipaddr4_format(addr, daddr, sizeof(daddr));

  for (; !netstar->interrupted && requests;) {
    netstar_time_t remaining_time = netstar_timer(&timer);
    netstar_host_t *host = NULL;

    if (!(host = netstar_hosts_findipaddr4(netstar_scanner_scanned_hosts4, addr)) && remaining_time) {
      netstar_time_sleep(netstar_time_secstomillisecs(remaining_time)/2);
      continue;
    }

    if (!host && !remaining_time) {
      netstar_log("\b \b[ recon@host4 ] request for %s\r\n", daddr);

      netstar_sendarp(&netstar->managed, &netstar->managed.iface->mac, &MACIEEE802_BROADCAST, NETSTAR_ARP_OPCODE_REQUEST, &netstar->managed.iface->mac, &netstar->managed.iface->addr, &MACIEEE802_BROADCAST, addr);

      if (requests > 0)
        requests--;

      netstar_timer_start(&timer, secs);
      continue;
    }

    memcpy(mac, &host->mac, NETWORK_MACADDR_SIZE);
    network_macaddr_format(mac, smac, sizeof(smac));

    netstar_log("\b \b[ recon@host4 ] reply from %s %s\r\n", smac, daddr);

    status = 1;
    break;
  }

  return status;
}

void
netstar_recon_host4(netstar_t *netstar, netstar_host_t *host) {
  netstar_recon_host4_reconnaissance(netstar, &host->mac, &host->addr.v4, -1, 4);
}

void
netstar_recon_hosts4(netstar_t *netstar, netstar_hosts_t hosts) {
  netstar_hosts_iterator_t hosts_iter = NULL;
  netstar_host_t *host = NULL;

  for (hosts_iter = netstar_hosts_begin(hosts); hosts_iter; hosts_iter = netstar_hosts_next(hosts_iter)) {
    host = netstar_hosts_value(hosts_iter);

    if (host->inet == NETWORK_IPADDR4 && !memcmp(&host->mac, "\x00\x00\x00\x00\x00\x00", NETWORK_MACADDR_SIZE))
      netstar_recon_host4(netstar, host);
  }
}

static int
netstar_recon_host6_reconnaissance(netstar_t *netstar, network_macaddr_t *mac, const network_ipaddr6_t *addr, ssize_t requests, netstar_time_t secs) {
  netstar_timer_t timer = NETSTAR_TIMER_INITIALIZER;

  char smac[NETWORK_MACADDR_STRLENGTH] = {0};
  char daddr[NETWORK_IPADDR6_STRLENGTH] = {0};

  int status = -1;

  network_ipaddr6_format(addr, daddr, sizeof(daddr));

  for (; !netstar->interrupted && requests;) {
    netstar_time_t remaining_time = netstar_timer(&timer);
    netstar_host_t *host = NULL;

    if (!(host = netstar_hosts_findipaddr6(netstar_scanner_scanned_hosts6, addr)) && remaining_time) {
      netstar_time_sleep(netstar_time_secstomillisecs(remaining_time)/2);
      continue;
    }

    if (!host && !remaining_time) {
      netstar_log("\b \b[ recon@host6 ] neighbor solicitation for %s\r\n", daddr);

      netstar_sendndpsolicit(&netstar->managed, &netstar->managed.iface->mac, &MACIEEE802_V6ALLNODES, NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT, &netstar->managed.iface->addr6, addr, NULL);

      if (requests > 0)
        requests--;

      netstar_timer_start(&timer, secs);
      continue;
    }

    memcpy(mac, &host->mac, NETWORK_MACADDR_SIZE); 
    network_macaddr_format(mac, smac, sizeof(smac));

    netstar_log("\b \b[ recon@host6 ] neighbor advertisement for %s %s\r\n", smac, daddr);

    status = 1;
    break;
  }

  return status;
}

void
netstar_recon_host6(netstar_t *netstar, netstar_host_t *host) {
  netstar_recon_host6_reconnaissance(netstar, &host->mac, &host->addr.v6, -1, 4);
}

void
netstar_recon_hosts6(netstar_t *netstar, netstar_hosts_t hosts) {
  netstar_hosts_iterator_t hosts_iter = NULL;
  netstar_host_t *host = NULL;

  for (hosts_iter = netstar_hosts_begin(hosts); hosts_iter; hosts_iter = netstar_hosts_next(hosts_iter)) {
    host = netstar_hosts_value(hosts_iter);

    if (host->inet == NETWORK_IPADDR6 && !memcmp(&host->mac, "\x00\x00\x00\x00\x00\x00", NETWORK_MACADDR_SIZE))
      netstar_recon_host6(netstar, host);
  }
}
