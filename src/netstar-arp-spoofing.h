#ifndef _NETSTAR_ARP_SPOOFING_H
#define _NETSTAR_ARP_SPOOFING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-threads.h>
#include <netstar-time.h>

#include <netstar-hosts.h>

#include <netstar.h>


struct netstar_arp_broadcast_spoofing {
  netstar_hosts_t spoofed_hosts;

  netstar_time_t shockwave_time;
  size_t shockwaves;

  netstar_time_t persistent_time;

  netstar_thread_t *thread;

  netstar_t *netstar;
};


extern int netstar_arp_broadcast_spoofing_new(struct netstar_arp_broadcast_spoofing *, netstar_t *);
extern void netstar_arp_broadcast_spoofing_free(struct netstar_arp_broadcast_spoofing *);


struct netstar_arp_network_spoofing {
  netstar_host_t redirection_host;
  bool redirective;

  netstar_hosts_t protected_hosts,
                  spoofed_hosts;

  bool bidirectional;

  netstar_time_t shockwave_time;
  size_t shockwaves;

  netstar_time_t persistent_time;

  bool request;

  netstar_thread_t *thread;

  netstar_t *netstar;
};


extern int netstar_arp_network_spoofing_new(struct netstar_arp_network_spoofing *, netstar_t *);
extern void netstar_arp_network_spoofing_free(struct netstar_arp_network_spoofing *);


struct netstar_arp_spoofing {
  netstar_host_t redirection_host;
  bool redirective;

  netstar_hosts_t spoofed_hosts,
                  target_hosts;

  bool bidirectional;

  netstar_time_t shockwave_time;
  size_t shockwaves;

  netstar_time_t persistent_time;

  bool request;

  netstar_thread_t *thread;

  netstar_t *netstar;
};


extern int netstar_arp_spoofing_new(struct netstar_arp_spoofing *, netstar_t *);
extern void netstar_arp_spoofing_free(struct netstar_arp_spoofing *);

#ifdef __cplusplus
}
#endif

#endif
