#include "netstar-redirect.h"

#include <netstar-services.h>

#include <netstar-scanner.h>
#include <netstar-packet.h>

#include <netstar-forward.h>
#include <netstar-capture.h>

#include <netstar-utils.h>
#include <netstar-time.h>

#include <collections/map/map.h>
#include <random/random.h>

#include <stdlib.h>


int
netstar_redirect_preroutes_add(netstar_redirect_preroutes_t preroutes, struct netstar_redirect_preroute *preroute) {
  return hashset_insert(preroutes, hashset_clvalue(preroute, sizeof(struct netstar_redirect_preroute)));
}


struct netstar_redirect_nat_mapping {
  network_macaddr_t smac;
  network_ipaddr_t saddr, daddr;

  uint16_t sport, dport, nport;

  netstar_timer_t timer;
};


static bool netstar_redirect_started, netstar_redirect_initialized;

static map_t *netstar_redirect_nat_table;


static int
netstar_redirect_nat_table_compare(const void *value1, size_t value1_length, const void *value2, size_t value2_length) {
  if (*(uint16_t *)value1 < *(uint16_t *)value2)
    return -1;
  if (*(uint16_t *)value1 > *(uint16_t *)value2)
    return 1;

  return 0;
}

static inline int
netstar_redirect_nat_table_add(struct netstar_redirect_nat_mapping *mapping) {
  return map_insert(netstar_redirect_nat_table, map_clvalue(&mapping->nport, sizeof(uint16_t)), map_lvalue(mapping, sizeof(struct netstar_redirect_nat_mapping *)));
}

static inline int
netstar_redirect_nat_table_remove(struct netstar_redirect_nat_mapping *mapping) {
  return map_remove(netstar_redirect_nat_table, &mapping->nport, sizeof(uint16_t));
}

/*
static inline void
netstar_redirect_nat_table_fetch(void) {
  map_iterator_t mapping_iter;

  for (mapping_iter = map_begin(netstar_redirect_nat_table); mapping_iter; mapping_iter = map_next(mapping_iter)) {
    struct netstar_redirect_nat_mapping *mapping = (struct netstar_redirect_nat_mapping *)map_value(mapping_iter);
    netstar_time_t remaining_time = netstar_timer(&mapping->timer);

    if (!remaining_time) {
    	mapping_iter = map_next(mapping_iter);
      netstar_redirect_nat_table_remove(mapping);
    }
  }
}
*/

static bool
netstar_redirect_nat_mapping_find(const void *key, const void *value, va_list args) {
  struct netstar_redirect_nat_mapping *mapping = (struct netstar_redirect_nat_mapping *)value;
  struct netstar_capture_packet *packet = va_arg(args, struct netstar_capture_packet *);

  return mapping->sport == packet->layer4.sport;
}

static uint16_t
netstar_redirect_nat_mapping_port_new(void) {
  uint32_t port;

  do {
    port = (random_uint32()+1025) & 0xFFFF;
  } while (map_get(netstar_redirect_nat_table, &port, sizeof(uint16_t)));

  return (uint16_t)port;
}

static struct netstar_redirect_nat_mapping *
netstar_redirect_nat_mapping_reference(struct netstar_capture_packet *packet) {
  struct netstar_redirect_nat_mapping *mapping = NULL;

  if ((mapping = (struct netstar_redirect_nat_mapping *)map_findif(netstar_redirect_nat_table, netstar_redirect_nat_mapping_find, packet)))
    return mapping;

  if (!(mapping = (struct netstar_redirect_nat_mapping *)calloc(1, sizeof(struct netstar_redirect_nat_mapping))))
    return NULL;

  netstar_timer_start(&mapping->timer, 16 /* 16s | 30s */);

  mapping->smac  = packet->layer2.smac;
  mapping->saddr = packet->layer3.saddr;
  mapping->daddr = packet->layer3.daddr;
  mapping->sport = packet->layer4.sport;
  mapping->dport = packet->layer4.dport;
  mapping->nport = netstar_redirect_nat_mapping_port_new();

  netstar_redirect_nat_table_add(mapping);

  return mapping;
}

static bool
netstar_redirect_preroute_snat(const void *value, va_list args) {
  const struct netstar_redirect_preroute *preroute = (const struct netstar_redirect_preroute *)value;
  struct netstar_capture_packet *packet = va_arg(args, struct netstar_capture_packet *);

  return packet->layer3.protocol == preroute->protocol && packet->layer4.sport == preroute->rport;
}

void
netstar_redirect_prerouting_snat(netstar_t *netstar, struct netstar_capture_packet *packet, void *args) {
  struct netstar_redirect *redirect = (struct netstar_redirect *)args;
  struct netstar_redirect_preroute *preroute = NULL;

  netstar_packet_t *redirect_packet = NULL;

  struct netstar_redirect_nat_mapping *mapping = NULL;

  if (!network_macaddr_compare(&packet->layer2.smac, &netstar->managed.iface->mac) || !network_ipaddr4_compare(&packet->layer3.saddr.v4, &netstar->managed.iface->addr))
    return;

  if (!(preroute = (struct netstar_redirect_preroute *)hashset_findif(redirect->preroutes, netstar_redirect_preroute_snat, packet)))
    return;

  if (!(mapping = map_get(netstar_redirect_nat_table, &packet->layer4.dport, sizeof(uint16_t))))
    return;

  if (!(redirect_packet = netstar_packet_new()))
    return;

  netstar_packet_buildethernet(
    redirect_packet,              /* @ packet buffer   */
    NULL,                         /* @ packet block    */
    &mapping->smac,               /* @ destination mac */
    &netstar->managed.iface->mac, /* @ source mac      */
    packet->layer2.protocol,      /* @ protocol        */
    NULL,                         /* @ payload         */
    0                             /* @ payload length  */
  );

  if (packet->type & NETSTAR_FORWARD_IPV4) {
    struct netstar_iphdr *iph = (struct netstar_iphdr *)packet->layer3.ptr;

    netstar_packet_buildipv4(
      redirect_packet,             /* @ packet buffer    */
      NULL,                        /* @ packet block     */
      iph->typeof_service,         /* @ type of service  */
      packet->layer3.length,       /* @ length           */
      ntohs(iph->id),              /* @ identifier       */
      ntohs(iph->fragment_offset), /* @ fragment offset  */
      iph->ttl,                    /* @ ttl              */
      packet->layer3.protocol,     /* @ protocol         */
      0,                           /* @ checksum         */
      &mapping->daddr.v4,          /* @ source addr      */
      &mapping->saddr.v4,          /* @ destination addr */
      NULL,                        /* @ payload          */
      0                            /* @ payload length   */
    );
  } else if (packet->type & NETSTAR_FORWARD_IPV6) {
    struct netstar_ipv6hdr *ipv6h = (struct netstar_ipv6hdr *)packet->layer3.ptr;

    netstar_packet_buildipv6(
      redirect_packet,       /* @ packet buffer    */
      NULL,                  /* @ packet block     */
      ipv6h->priority,       /* @ traffic class    */
      0, // ipv6h->flow_label, /* @ flow label       */
      packet->layer3.length, /* @ length           */
      ipv6h->nexthdr,        /* @ next header      */
      ipv6h->hop_limit,      /* @ hop limit        */
      &mapping->daddr.v6,    /* @ source addr      */
      &mapping->saddr.v6,    /* @ destination addr */
      NULL,                  /* @ payload          */
      0                      /* @ payload length   */
    );
  }

  if (packet->type & NETSTAR_FORWARD_TCP) {
    struct netstar_tcphdr *tcph = (struct netstar_tcphdr *)packet->layer4.ptr;

    if (tcph->flags & NETSTAR_TCP_FLAGS_FIN || tcph->flags & NETSTAR_TCP_FLAGS_RST)
      netstar_redirect_nat_table_remove(mapping);

    netstar_packet_buildtcp(
      redirect_packet,              /* @ packet buffer         */
      NULL,                         /* @ packet block          */
      mapping->dport,               /* @ source port           */
      mapping->sport,               /* @ destination port      */
      ntohl(tcph->sequence),        /* @ sequence              */
      ntohl(tcph->acknowledgment),  /* @ acknowledgment number */
      tcph->flags,                  /* @ flags                 */
      ntohs(tcph->window),          /* @ window                */
      ntohs(tcph->urgentptr),       /* @ urgent pointer        */
      0,                            /* @ checksum              */
      packet->layer4.payload,       /* @ payload               */
      packet->layer4.payload_length /* @ payload length        */
    );

    if ((tcph->data_offset*4) > NETSTAR_TCP_SIZE) {
      uint8_t *options = (uint8_t *)tcph+NETSTAR_TCP_SIZE;
      uint8_t options_length = (tcph->data_offset*4)-NETSTAR_TCP_SIZE;

      netstar_packet_buildtcpoptions(
        redirect_packet,
        NULL,
        options,
        options_length
      );
    }
  } else if (packet->type & NETSTAR_FORWARD_UDP) {
    netstar_packet_buildudp(
      redirect_packet,              /* @ packet buffer    */
      NULL,                         /* @ packet block     */
      mapping->dport,               /* @ source port      */
      mapping->sport,               /* @ destination port */
      packet->layer4.length,        /* @ length           */
      0,                            /* @ checksum         */
      packet->layer4.payload,       /* @ payload          */
      packet->layer4.payload_length /* @ payload length   */
    );
  }

  netstar_packet_coalesce(redirect_packet);

  netstar_send(&netstar->managed, netstar_packet_buffer(redirect_packet), netstar_packet_length(redirect_packet));

  netstar_packet_free(redirect_packet);

// netstar_redirect_nat_table_fetch();
}

static bool
netstar_redirect_preroute_dnat(const void *value, va_list args) {
  const struct netstar_redirect_preroute *preroute = (const struct netstar_redirect_preroute *)value;
  struct netstar_capture_packet *packet = va_arg(args, struct netstar_capture_packet *);

  return (packet->layer3.protocol == preroute->protocol && (packet->layer4.sport == preroute->dport || packet->layer4.dport == preroute->dport));
}

static void
netstar_redirect_prerouting_dnat(netstar_t *netstar, struct netstar_capture_packet *packet, void *args) {
  struct netstar_redirect *redirect = (struct netstar_redirect *)args;
  struct netstar_redirect_preroute *preroute = NULL;

  netstar_packet_t *redirect_packet = NULL;

  struct netstar_redirect_nat_mapping *mapping = NULL;

#if defined __linux__ || defined __linux
  netstar_linkwave_t *loopback = ((packet->type & NETSTAR_FORWARD_IPV6) ? &netstar->loopback6 : &netstar->loopback4);
#else
  netstar_linkwave_t *loopback = &netstar->loopback;
#endif

  if (!(preroute = (struct netstar_redirect_preroute *)hashset_findif(redirect->preroutes, netstar_redirect_preroute_dnat, packet)))
    return;

  packet->flags &= ~(netstar_capture_packet_flags_t)NETSTAR_CAPTURE_PACKET_FORWARD;

  if (packet->layer4.sport == preroute->dport)
    return;

  if (!(redirect_packet = netstar_packet_new()))
    return;

  netstar_packet_buildethernet(
    redirect_packet,                /* @ packet buffer   */
    NULL,                           /* @ packet block    */
    &packet->layer2.dmac,           /* @ destination mac */
    &packet->layer2.smac,           /* @ source mac      */
    htons(packet->layer2.protocol), /* @ protocol        */
    NULL,                           /* @ payload         */
    0                               /* @ payload length  */
  );

  if (packet->type & NETSTAR_FORWARD_IPV4) {
    struct netstar_iphdr *iph = (struct netstar_iphdr *)packet->layer3.ptr;

    netstar_packet_buildipv4(
      redirect_packet,             /* @ packet buffer    */
      NULL,                        /* @ packet block     */
      iph->typeof_service,         /* @ type of service  */
      packet->layer3.length,       /* @ length           */
      ntohs(iph->id),              /* @ identifier       */
      ntohs(iph->fragment_offset), /* @ fragment offset  */
      iph->ttl,                    /* @ ttl              */
      packet->layer3.protocol,     /* @ protocol         */
      0,                           /* @ checksum         */
      &NETWORK_IPADDR4_LOCALHOST,  /* @ source addr      */
      &NETWORK_IPADDR4_LOCALHOST,  /* @ destination addr */
      NULL,                        /* @ payload          */
      0                            /* @ payload length   */
    );
  } else if (packet->type & NETSTAR_FORWARD_IPV6) {
    struct netstar_ipv6hdr *ipv6h = (struct netstar_ipv6hdr *)packet->layer3.ptr;

    netstar_packet_buildipv6(
      redirect_packet,            /* @ packet buffer    */
      NULL,                       /* @ packet block     */
      ipv6h->priority,            /* @ traffic class    */
      0, // ipv6h->flow_label,    /* @ flow label       */
      packet->layer3.length,      /* @ length           */
      ipv6h->nexthdr,             /* @ next header      */
      ipv6h->hop_limit,           /* @ hop limit        */
      &NETWORK_IPADDR6_LOCALHOST, /* @ source addr      */
      &NETWORK_IPADDR6_LOCALHOST, /* @ destination addr */
      NULL,                       /* @ payload          */
      0                           /* @ payload length   */
    );
  }

  if (!(mapping = netstar_redirect_nat_mapping_reference(packet)))
    return;

  if (packet->type & NETSTAR_FORWARD_TCP) {
    struct netstar_tcphdr *tcph = (struct netstar_tcphdr *)packet->layer4.ptr;

    if (tcph->flags & NETSTAR_TCP_FLAGS_FIN || tcph->flags & NETSTAR_TCP_FLAGS_RST)
      netstar_redirect_nat_table_remove(mapping);

    netstar_packet_buildtcp(
      redirect_packet,              /* @ packet buffer         */
      NULL,                         /* @ packet block          */
      mapping->nport,               /* @ source port           */
      preroute->rport,              /* @ destination port      */
      ntohl(tcph->sequence),        /* @ sequence              */
      ntohl(tcph->acknowledgment),  /* @ acknowledgment number */
      tcph->flags,                  /* @ flags                 */
      ntohs(tcph->window),          /* @ window                */
      ntohs(tcph->urgentptr),       /* @ urgent pointer        */
      0,                            /* @ checksum              */
      packet->layer4.payload,       /* @ payload               */
      packet->layer4.payload_length /* @ payload length        */
    );

    if ((tcph->data_offset*4) > NETSTAR_TCP_SIZE) {
      uint8_t *options = (uint8_t *)tcph+NETSTAR_TCP_SIZE;
      uint8_t options_length = (tcph->data_offset*4)-NETSTAR_TCP_SIZE;

      netstar_packet_buildtcpoptions(
        redirect_packet,
        NULL,
        options,
        options_length
      );
    }
  } else if (packet->type & NETSTAR_FORWARD_UDP) {
    netstar_packet_buildudp(
      redirect_packet,              /* @ packet buffer    */
      NULL,                         /* @ packet block     */
      mapping->nport,               /* @ source port      */
      preroute->rport,              /* @ destination port */
      packet->layer4.length,        /* @ length           */
      0,                            /* @ checksum         */
      packet->layer4.payload,       /* @ payload          */
      packet->layer4.payload_length /* @ payload length   */
    );
  }

  netstar_packet_coalesce(redirect_packet);

  netstar_send(loopback, netstar_packet_buffer(redirect_packet), netstar_packet_length(redirect_packet));

  netstar_packet_free(redirect_packet);

// netstar_redirect_nat_table_fetch();
}

int
netstar_redirect_new(struct netstar_redirect *redirect, netstar_t *netstar) {
  if (!(redirect->preexcluded_hosts = netstar_hosts_new()))
    goto _return;

  if (!(redirect->preroutes = hashset_new(free)))
    goto _return;

  netstar_utils_filesystem_preroutes(redirect->preroutes, "share/redirect/netstar-redirect-prerouting.rules");

  redirect->netstar = netstar;

  return 1;

_return:
  netstar_redirect_free(redirect);
  return -1;
}

void
netstar_redirect_free(struct netstar_redirect *redirect) {
  if (redirect) {
    netstar_hosts_free(redirect->preexcluded_hosts);
    hashset_free(redirect->preroutes);
  }
}

static void
netstar_redirect_start(void *context) {
  struct netstar_redirect *redirect = (struct netstar_redirect *)context;

  if (!netstar_redirect_initialized || netstar_redirect_started)
    return;

  netstar_forward_add(netstar_redirect_prerouting_dnat, NETSTAR_FORWARD_TCP|NETSTAR_FORWARD_UDP, redirect);
  netstar_forward_add(netstar_redirect_prerouting_snat, NETSTAR_FORWARD_LOOPBACK|NETSTAR_FORWARD_TCP|NETSTAR_FORWARD_UDP, redirect);

  netstar_redirect_started = true;

  netstar_log("#netstar-redirect started\r\n");
}

static void
netstar_redirect_stop(void *context) {
  if (!netstar_redirect_initialized || !netstar_redirect_started)
    return;

  netstar_log("#netstar-redirect stopping...\r\n");

  netstar_forward_remove(netstar_redirect_prerouting_dnat);
  netstar_forward_remove(netstar_redirect_prerouting_snat);

  netstar_redirect_started = false;
}

static void
netstar_redirect_initialize(netstar_t *netstar, void *context) {
  if (netstar_redirect_initialized)
    return;

  if (!(netstar_redirect_nat_table = map_new(netstar_redirect_nat_table_compare, free, free)))
    return;

  netstar_redirect_initialized = true;
}

static void
netstar_redirect_deinitialize(void *context) {
  struct netstar_redirect *redirect = (struct netstar_redirect *)context;

  if (!netstar_redirect_initialized)
    return;

  netstar_redirect_free(redirect);

  map_free(netstar_redirect_nat_table);
  netstar_redirect_nat_table = NULL;

  netstar_redirect_initialized = false;
}


static struct netstar_redirect netstar_redirect_context;

static struct netstar_service netstar_redirect_context_service = {
  netstar_redirect_initialize, netstar_redirect_deinitialize,
  netstar_redirect_start, netstar_redirect_stop,
  "netstar:redirect", &netstar_redirect_context,
};

struct netstar_service *
netstar_redirect_service(void) {
  return &netstar_redirect_context_service;
}

__attribute__((__constructor__))
static inline void
netstar_redirect_register_service(void) {
  netstar_services_register(netstar_redirect_service());
}
