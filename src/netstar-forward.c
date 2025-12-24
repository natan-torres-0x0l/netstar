#include "netstar-forward.h"

#include <netstar-scanner.h>
#include <netstar-hosts.h>

#include <netstar.h>

#include <collections/map/map.h>

#include <stdlib.h>


static netstar_thread_mutex_t netstar_forward_mutex;

static bool netstar_forward_initialized;


struct netstar_forward {
  netstar_forward_routine_t routine;
  void *args;
  netstar_forward_protocol_t protocol;
};


static map_t *netstar_forward_routines;


static int
netstar_forward_routines_compare(const void *key1, size_t key1_size, const void *key2, size_t key2_size) {
  if ((uintptr_t)key1 < (uintptr_t)key2)
    return -1;
  else if ((uintptr_t)key1 > (uintptr_t)key2)
    return 1;

  return 0;
}

static void
netstar_forward_packet(netstar_t *netstar, struct netstar_capture_packet *packet) {
  if (!netstar_scanner_scanned_hosts4 || !netstar_scanner_scanned_hosts6)
    return;
  if (!(packet->type & NETSTAR_FORWARD_IP))
    return;

  if (!network_macaddr_compare(&netstar->managed.iface->mac, &packet->layer2.dmac)) {
    netstar_host_t *host = NULL;

    if ((packet->type & NETSTAR_FORWARD_IPV4) && !(host = netstar_hosts_findipaddr4(netstar_scanner_scanned_hosts4, &packet->layer3.daddr.v4)) && !(host = netstar_hosts_findipaddr4(netstar_scanner_scanned_hosts4, &netstar->managed.iface->gateway)))
      return;

    if ((packet->type & NETSTAR_FORWARD_IPV6) && !(host = netstar_hosts_findipaddr6(netstar_scanner_scanned_hosts6, &packet->layer3.daddr.v6)) && !(host = netstar_hosts_findipaddr6(netstar_scanner_scanned_hosts6, &netstar->managed.iface->gateway6)))
      return;

    netstar_sendethernet(&netstar->managed, &netstar->managed.iface->mac, &host->mac, packet->layer2.protocol, packet->layer3.ptr, (uint16_t)packet->layer3.length);
  }
}

void
netstar_forward(netstar_t *netstar, struct netstar_capture_packet *packet) {
  map_iterator_t iterator = NULL;

  netstar_thread_mutex_lock(&netstar_forward_mutex);

  if (!netstar_forward_initialized)
    goto _return;

  for (iterator = map_begin(netstar_forward_routines); iterator; iterator = map_next(iterator)) {
    struct netstar_forward *forward = (struct netstar_forward *)map_value(iterator);

    if ((!forward->protocol && !(packet->type & NETSTAR_FORWARD_LOOPBACK)) || (packet->type & forward->protocol)) {
      forward->routine(netstar, packet, forward->args);
    }
  }

  if ((packet->flags & NETSTAR_CAPTURE_PACKET_FORWARD))
    netstar_forward_packet(netstar, packet);

_return:
  netstar_thread_mutex_unlock(&netstar_forward_mutex);
}

void
netstar_forward_add(netstar_forward_routine_t routine, netstar_forward_protocol_t protocol, void *args) {
  struct netstar_forward forward = {
    .routine = routine, .args = args, .protocol = protocol,
  };

  netstar_thread_mutex_lock(&netstar_forward_mutex);

  if (!netstar_forward_initialized)
    goto _return;

  map_insert(
    netstar_forward_routines,
    map_rvalue(routine, sizeof(netstar_forward_routine_t)),
    map_clvalue(&forward, sizeof(struct netstar_forward))
  );

_return:
  netstar_thread_mutex_unlock(&netstar_forward_mutex);
}

void
netstar_forward_remove(netstar_forward_routine_t routine) {
  netstar_thread_mutex_lock(&netstar_forward_mutex);

  if (!netstar_forward_initialized)
    goto _return;

  map_remove(netstar_forward_routines, routine, sizeof(netstar_forward_routine_t));

_return:
  netstar_thread_mutex_unlock(&netstar_forward_mutex);
}

void
netstar_forward_initialize(void) {
  netstar_thread_mutex_lock(&netstar_forward_mutex);

  if (netstar_forward_initialized)
    goto _return;

  if (!(netstar_forward_routines = map_new(netstar_forward_routines_compare, NULL, free)))
    goto _return;

  netstar_forward_initialized = true;

_return:
  netstar_thread_mutex_unlock(&netstar_forward_mutex);
}

void
netstar_forward_deinitialize(void) {
  netstar_thread_mutex_lock(&netstar_forward_mutex);

  if (!netstar_forward_initialized)
    goto _return;

  map_free(netstar_forward_routines);
  netstar_forward_routines = NULL;

  netstar_forward_initialized = false;

_return:
  netstar_thread_mutex_unlock(&netstar_forward_mutex);
}
