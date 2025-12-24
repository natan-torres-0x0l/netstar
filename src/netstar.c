#include <netstar-threads.h>
#include <netstar-time.h>

#include <netstar-packet.h>

#include <netstar.h>

#include <random/random.h>

#include <string.h>
#include <stdio.h>


static inline struct netlink_interface *
netstar_network_interface_by_addr(const network_ipaddr4_t *addr, netlink_interface_flags_t flags) {
  struct netlink_interface **ifaces = NULL, *iface = NULL;
  size_t ifs;

  if (!(ifaces = netlink_interfaces()))
    return NULL;

  for (ifs = 0; ifaces[ifs]; ifs++)
    if (!network_ipaddr4_compare(&ifaces[ifs]->addr, addr) && (ifaces[ifs]->flags & flags))
      break;

  iface = netlink_interfacebyname(ifaces[ifs]->name);

  netlink_interfaces_free(ifaces);
  return iface;
}

static inline struct netlink_interface *
netstar_managed_interface(void) {
  static const netlink_interface_flags_t iface_flags = NETLINK_INTERFACE_FLAGS_UP|NETLINK_INTERFACE_FLAGS_RUNNING;
  struct netlink_interface *iface = NULL;
  network_ipaddr4_t *iface_addr = NULL;

  static struct network_endpoint network_addr = {0};
  socket_t socket = SOCKET_INVALID;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    netstar_critical("netstar-managed-interface: socket-error: %s\r\n", socket_error_reason(socket_geterror()));

  network_endpoint_new(&network_addr, "8.8.8.8", 53);

  socket_connect(socket, &network_addr.base, network_addr.size, NULL);

  if (socket_send(socket, "z", 1, 0) == -1)
    netstar_critical("netstar-managed-interface: socket-error: %s\r\n", socket_error_reason(socket_geterror()));

  if (socket_localaddr(socket, &network_addr.base, &network_addr.size) == -1)
    netstar_critical("netstar-managed-interface: socket-error: %s\r\n", socket_error_reason(socket_geterror()));

  iface_addr = network_endpoint_addr4(&network_addr);
  iface = netstar_network_interface_by_addr(iface_addr, iface_flags);

  socket_close(socket);
  return iface;
}

void
netstar_managed_interface_set(netstar_t *netstar, const char *iface_name) {
  if ((iface_name && !(netstar->managed.iface = netlink_interfacebyname(iface_name))) || (!iface_name && !(netstar->managed.iface = netstar_managed_interface())))
    netstar_critical("netstar-managed-interface: netlink-error: %s\r\n", netlink_error_reason(netlink_geterror()));

  if (!netlink_interface_isreachable(netstar->managed.iface))
    netstar_critical("netstar-managed-interface: netstar-error: the interface \"%s\" is not available (up/active or running)\r\n", netstar->managed.iface->name);

  if (!(netstar->managed.live = netpacket_live_bind(netstar->managed.iface, netpacket_layer2(), NETPACKET_LIVE_PROMISCUOS)))
    netstar_critical("netstar-managed-interface: netpacket-error: %s\r\n", netpacket_error_reason(netpacket_geterror()));
}

static struct netlink_interface *
netstar_loopback_interface(void) {
  static const netlink_interface_flags_t iface_flags = NETLINK_INTERFACE_FLAGS_LOOPBACK;
  static const network_ipaddr4_t iface_addr = NETWORK_IPADDR4_ILOOPBACK;

  return netstar_network_interface_by_addr(&iface_addr, iface_flags);
}

void
netstar_loopback_interface_set(netstar_t *netstar, const char *iface_name) {
  if (!(netstar->loopback.iface = netstar_loopback_interface()))
    netstar_critical("netstar-loopback-interface: netlink-error: %s\r\n", netlink_error_reason(netlink_geterror()));

  if (!netlink_interface_isreachable(netstar->loopback.iface))
    netstar_critical("netstar-loopback-interface: netstar-error: the interface \"%s\" is not available (up/active or running)\r\n", netstar->loopback.iface->name);

#if defined __linux__ || defined __linux
  if (!(netstar->loopback4.live = netpacket_live_bind(netstar->loopback.iface, netpacket_layer3(NETPACKET_PROTOCOL_IPV4), NETPACKET_LIVE_NORMAL)))
    netstar_critical("netstar-loopback4-interface: netpacket-error: %s\r\n", netpacket_error_reason(netpacket_geterror()));

  if (!(netstar->loopback6.live = netpacket_live_bind(netstar->loopback.iface, netpacket_layer3(NETPACKET_PROTOCOL_IPV6), NETPACKET_LIVE_NORMAL)))
    netstar_critical("netstar-loopback6-interface: netpacket-error: %s\r\n", netpacket_error_reason(netpacket_geterror()));
#endif
  if (!(netstar->loopback.live = netpacket_live_bind(netstar->loopback.iface, netpacket_layer2(), NETPACKET_LIVE_NORMAL)))
    netstar_critical("netstar-loopback-interface: netpacket-error: %s\r\n", netpacket_error_reason(netpacket_geterror()));
}

void
netstar_forward_set(netstar_t *netstar, bool forward) {
  netstar->forward = forward;
}

ssize_t
netstar_recv(netstar_linkwave_t *netstar, void *buffer, size_t buffer_length, struct netpacket_timeout *timeout) {
  netpacket_live_t *live = netstar->live;
  ssize_t received = -1;

  netstar_thread_mutex_lock(&netstar->mutex.recv);
  received = netpacket_live_recv(live, buffer, buffer_length, timeout);
  netstar_thread_mutex_unlock(&netstar->mutex.recv);

  return received;
}

ssize_t
netstar_send(netstar_linkwave_t *netstar, void *buffer, size_t buffer_length) {
  netpacket_live_t *live = netstar->live;
  ssize_t sent = -1;

  if (netpacket_live_layer(live) == NETPACKET_LAYER_LAYER3) {
    buffer += NETSTAR_ETHERNET_SIZE;
    buffer_length -= NETSTAR_ETHERNET_SIZE;
  }

  netstar_thread_mutex_lock(&netstar->mutex.send);
  sent = netpacket_live_send(live, buffer, buffer_length);
  netstar_thread_mutex_unlock(&netstar->mutex.send);

  return sent;
}

ssize_t
netstar_sendethernet(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint16_t type, uint8_t *payload, uint16_t payload_length) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,        /* @ packet buffer   */
    NULL,          /* @ packet block    */
    dmac,          /* @ destination mac */
    smac,          /* @ source mac      */
    type,          /* @ protocol        */
    payload,       /* @ payload         */
    payload_length /* @ payload length  */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendarp(netstar_linkwave_t *netstar, const network_macaddr_t *shardware, const network_macaddr_t *dhardware, uint16_t opcode, const network_macaddr_t *smac, const network_ipaddr4_t *saddr, const network_macaddr_t *dmac, const network_ipaddr4_t *daddr) {
  netstar_packet_t *packet = NULL;
  ssize_t sent = -1;

  network_macaddr_t target_mac = {0};

  if (netstar->iface->flags & NETLINK_INTERFACE_FLAGS_LOOPBACK)
    return -1;

  if (!(packet = netstar_packet_new()))
    return -1;
 
  netstar_packet_buildethernet(
    packet,                        /* @ packet buffer   */
    NULL,                          /* @ packet block    */
    dhardware,                     /* @ destination mac */
    shardware,                     /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_ARP, /* @ protocol        */
    NULL,                          /* @ payload         */
    0                              /* @ payload length  */
  );

  if (!(opcode == NETSTAR_ARP_OPCODE_REQUEST && !network_macaddr_compare(dmac, &MACIEEE802_BROADCAST)))
    target_mac = *dmac;

  netstar_packet_buildarp(
    packet,                        /* @ packet buffer    */
    NULL,                          /* @ packet block     */
    NETSTAR_ARP_HARDWARE_ETHERNET, /* @ hardware type    */
    NETSTAR_ARP_PROTOCOL_IPV4,     /* @ protocol         */
    NETWORK_MACADDR_SIZE,          /* @ hardware size    */
    NETWORK_IPADDR4_SIZE,          /* @ protocol size    */
    opcode,                        /* @ operation code   */
    smac,                          /* @ source mac       */
    saddr,                         /* @ source addr      */
    &target_mac,                   /* @ destination mac  */
    daddr,                         /* @ destination addr */
    NULL,                          /* @ payload          */
    0                              /* @ payload length   */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendicmpecho(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint8_t type, const network_ipaddr4_t *saddr, const network_ipaddr4_t *daddr) {
  netstar_packet_t *packet = NULL;

  static uint8_t sequence = 1;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                         /* @ packet buffer   */
    NULL,                           /* @ packet block    */
    dmac,                           /* @ destination mac */
    smac,                           /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV4, /* @ protocol        */
    NULL,                           /* @ payload         */
    0                               /* @ payload length  */
  );

  netstar_packet_buildipv4(
    packet,                     /* @ packet buffer    */
    NULL,                       /* @ packet block     */
    0,                          /* @ type of service  */
    NETSTAR_IP_SIZE+NETSTAR_ICMP_SIZE+NETSTAR_ICMP_ECHO_SIZE, /* @ length          */
    random_uint16(),            /* @ identifier       */
    0,                          /* @ fragment offset  */
    64,                         /* @ ttl              */
    NETSTAR_IP_PROTOCOL_ICMPV4, /* @ protocol         */
    0,                          /* @ checksum         */
    saddr,                      /* @ source addr      */
    daddr,                      /* @ destination addr */
    NULL,                       /* @ payload          */
    0                           /* @ payload length   */
  );

  netstar_packet_buildicmpecho(
    packet,          /* @ packet buffer   */
    NULL,            /* @ packet block    */
    type,            /* @ message type    */
    0,               /* @ message code    */
    0,               /* @ checksum        */
    random_uint16(), /* @ identifier      */
    sequence++,      /* @ sequence        */
    NULL,            /* @ payload         */
    0                /* @ payload length  */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendicmpredirect(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint8_t code, const network_ipaddr4_t *gateway, const network_ipaddr4_t *saddr, const network_ipaddr4_t *daddr, struct netstar_iphdr *iph) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (netstar->iface->flags & NETLINK_INTERFACE_FLAGS_LOOPBACK)
    return -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                         /* @ packet buffer   */
    NULL,                           /* @ packet block    */
    dmac,                           /* @ destination mac */
    smac,                           /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV4, /* @ protocol        */
    NULL,                           /* @ payload         */
    0                               /* @ payload length  */
  );

  netstar_packet_buildipv4(
    packet,                     /* @ packet buffer    */
    NULL,                       /* @ packet block     */
    0,                          /* @ type of service  */
    NETSTAR_IP_SIZE+NETSTAR_ICMP_SIZE+NETSTAR_ICMP_REDIRECT_SIZE+ntohs(iph->length), /* @ length          */
    random_uint16(),            /* @ identifier       */
    0,                          /* @ fragment offset  */
    64,                         /* @ ttl              */
    NETSTAR_IP_PROTOCOL_ICMPV4, /* @ protocol         */
    0,                          /* @ checksum         */
    saddr,                      /* @ source addr      */
    daddr,                      /* @ destination addr */
    NULL,                       /* @ payload          */
    0                           /* @ payload length   */
  );

  netstar_packet_buildicmpredirect(
    packet,                     /* @ packet buffer   */
    NULL,                       /* @ packet block    */
    NETSTAR_ICMP_TYPE_REDIRECT, /* @ message type    */
    code,                       /* @ message code    */
    gateway,                    /* @ gateway         */
    0,                          /* @ checksum        */
    (uint8_t *)iph,             /* @ payload         */
    ntohs(iph->length)          /* @ payload length  */
  );

#if false
  netstar_packet_buildipv4(
    packet,                            /* @ packet buffer    */
    NULL,                              /* @ packet block     */
    iph->tos,                          /* @ type of service  */
    ntohs(iph->length),                /* @ length           */
    ntohs(iph->id),                    /* @ identifier       */
    ntohs(iph->fragoff),               /* @ fragment offset  */
    iph->ttl,                          /* @ ttl              */
    iph->protocol,                     /* @ protocol         */
    iph->checksum,                     /* @ checksum         */
    &iph->saddr,                       /* @ source addr      */
    &iph->daddr,                       /* @ destination addr */
    ((uint8_t *)iph+NETSTAR_IP_SIZE),  /* @ payload          */
    ntohs(iph->length)-NETSTAR_IP_SIZE /* @ payload length   */
  );
#endif

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendicmpv6echo(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint8_t type, const network_ipaddr6_t *saddr, const network_ipaddr6_t *daddr) {
  netstar_packet_t *packet = NULL;
 // static thread_local uint8_t sequence = 1;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                               /* @ packet buffer   */
    NULL,                                 /* @ packet block    */
    dmac,                                 /* @ destination mac */
    smac,                                 /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV6,       /* @ protocol        */ 
    NULL,                                 /* @ payload         */
    0                                     /* @ payload length  */
  );

  netstar_packet_buildipv6(
    packet,                               /* @ packet buffer    */
    NULL,                                 /* @ packet block     */
    0,                                    /* @ traffic class    */
    0,                                    /* @ flow label       */
    NETSTAR_ICMPV6_SIZE+NETSTAR_ICMPV6_ECHO_SIZE, /* @ length          */
    NETSTAR_IP_PROTOCOL_ICMPV6,           /* @ next header      */
    64,                                   /* @ hop limit        */
    saddr,                                /* @ source addr      */
    daddr,                                /* @ destination addr */
    NULL,                                 /* @ payload          */
    0                                     /* @ payload length   */
  );

  netstar_packet_buildicmpv6echo(
    packet,                               /* @ packet buffer  */
    NULL,                                 /* @ packet block   */
    NETSTAR_ICMPV6_TYPE_ECHO,             /* @ message type   */
    0,                                    /* @ message code   */
    0,                                    /* @ checksum       */
    random_uint16(),                      /* @ identifier     */
    1,                                    /* @ sequence       */
    NULL,                                 /* @ payload        */
    0                                     /* @ payload length */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendndpsolicit(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint8_t type, const network_ipaddr6_t *saddr, const network_ipaddr6_t *daddr, const network_macaddr_t *target_mac) {
  netstar_packet_t *packet = NULL;
  ssize_t sent = -1;

  char multicast_addr[NETWORK_IPADDR6_STRLENGTH] = {0};
  network_ipaddr6_t multicast_daddr = {0};

  network_ipaddr6_t *target_addr = (network_ipaddr6_t *)daddr;
  network_macaddr_t mac = *dmac;

  if (netstar->iface->flags & NETLINK_INTERFACE_FLAGS_LOOPBACK)
    return -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  if (type == NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT && !network_macaddr_compare(dmac, &MACIEEE802_V6ALLNODES)) {
    memcpy(&mac.u8[3], &daddr->u8[13], 3);
    mac.u8[2] = 0xFF;

    snprintf(multicast_addr, sizeof(multicast_addr), "ff02::1:ff%x:%x%x", daddr->u8[13], daddr->u8[14], daddr->u8[15]);
    network_ipaddr6_parse(&multicast_daddr, multicast_addr);

    target_addr = &multicast_daddr;
  }

  netstar_packet_buildethernet(
    packet,                               /* @ packet buffer   */
    NULL,                                 /* @ packet block    */
    &mac,                                /* @ destination mac */
    smac,                                 /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV6,       /* @ protocol        */ 
    NULL,                                 /* @ payload         */
    0                                     /* @ payload length  */
  );

  netstar_packet_buildipv6(
    packet,                               /* @ packet buffer    */
    NULL,                                 /* @ packet block     */
    0,                                    /* @ traffic class    */
    0,                                    /* @ flow label       */
    NETSTAR_ICMPV6_SIZE+NETSTAR_NDP_SOLICIT_SIZE+NETSTAR_NDP_OPTION_SIZE+NETWORK_MACADDR_SIZE,                  /* @ length          */
    NETSTAR_IP_PROTOCOL_ICMPV6,           /* @ next header      */
    255,                                  /* @ hop limit        */
    saddr,                                /* @ source addr      */
    target_addr,                          /* @ destination addr */
    NULL,                                 /* @ payload          */
    0                                     /* @ payload length   */
  );

  netstar_packet_buildndpsolicit(
    packet,                               /* @ packet buffer  */
    NULL,                                 /* @ packet block   */
    type,                                 /* @ message type   */
    0,                                    /* @ message code   */
    0,                                    /* @ checksum       */
    daddr,                                /* @ target addr    */
    NULL,                                 /* @ payload        */
    0                                     /* @ payload length */
  );

  if (target_mac == NULL)
    target_mac = smac;

  netstar_packet_buildndpoption(
    packet,                               /* @ packet buffer */
    NULL,                                 /* @ packet block  */
    NETSTAR_NDP_OPTION_SOURCE_LINKADDR,   /* @ option type   */
    target_mac,                           /* @ option value  */
    NETWORK_MACADDR_SIZE                  /* @ option length */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendndpadvert(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, uint8_t type, uint32_t flags, const network_ipaddr6_t *saddr, const network_ipaddr6_t *daddr, const network_macaddr_t *target_mac) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (netstar->iface->flags & NETLINK_INTERFACE_FLAGS_LOOPBACK)
    return -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                               /* @ packet buffer   */
    NULL,                                 /* @ packet block    */
    dmac,                                 /* @ destination mac */
    smac,                                 /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV6,       /* @ protocol        */ 
    NULL,                                 /* @ payload         */
    0                                     /* @ payload length  */
  );

  netstar_packet_buildipv6(
    packet,                               /* @ packet buffer    */
    NULL,                                 /* @ packet block     */
    0,                                    /* @ traffic class    */
    0,                                    /* @ flow label       */
    NETSTAR_ICMPV6_SIZE+NETSTAR_NDP_ADVERT_SIZE+NETSTAR_NDP_OPTION_SIZE+NETWORK_MACADDR_SIZE,                  /* @ length          */
    NETSTAR_IP_PROTOCOL_ICMPV6,           /* @ next header      */
    255,                                  /* @ hop limit        */
    saddr,                                /* @ source addr      */
    daddr,                                /* @ destination addr */
    NULL,                                 /* @ payload          */
    0                                     /* @ payload length   */
  );

  netstar_packet_buildndpadvert(
    packet,                               /* @ packet buffer   */
    NULL,                                 /* @ packet block    */
    type,                                 /* @ message type    */
    0,                                    /* @ message code    */
    0,                                    /* @ checksum        */
    flags,                                /* @ flags           */
    saddr,                                /* @ source addr     */
    NULL,                                 /* @ payload         */
    0                                     /* @ payload length  */
  );

  if (target_mac == NULL)
    target_mac = smac;

  netstar_packet_buildndpoption(
    packet,                               /* @ packet buffer   */
    NULL,                                 /* @ packet block    */
    NETSTAR_NDP_OPTION_TARGET_LINKADDR,   /* @ option type     */
    target_mac,                           /* @ option value    */
    NETWORK_MACADDR_SIZE                  /* @ option length   */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendtcp(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, const network_ipaddr4_t *saddr, const network_ipaddr4_t *daddr, uint16_t sport, uint16_t dport, uint32_t sequence, uint32_t acknowledgment, uint8_t flags, uint8_t *payload, uint16_t payload_length) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                         /* @ packet buffer   */
    NULL,                           /* @ packet block    */
    dmac,                           /* @ destination mac */
    smac,                           /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV4, /* @ protocol        */
    NULL,                           /* @ payload         */
    0                               /* @ payload length  */
  );

  netstar_packet_buildipv4(
    packet,                         /* @ packet buffer    */
    NULL,                           /* @ packet block     */
    0,                              /* @ type of service  */
    NETSTAR_IP_SIZE+NETSTAR_TCP_SIZE+payload_length, /* @ length          */
    random_uint16(),                /* @ identifier       */
    0,                              /* @ fragment offset  */
    64,                             /* @ ttl              */
    NETSTAR_IP_PROTOCOL_TCP,        /* @ protocol         */
    0,                              /* @ checksum         */
    saddr,                          /* @ source addr      */
    daddr,                          /* @ destination addr */
    NULL,                           /* @ payload          */
    0                               /* @ payload length   */
  );

  netstar_packet_buildtcp(
    packet,                         /* @ packet buffer         */
    NULL,                           /* @ packet block          */
    sport,                          /* @ source port           */
    dport,                          /* @ destination port      */
    sequence,                       /* @ sequence              */
    acknowledgment,                 /* @ acknowledgment number */
    flags,                          /* @ flags                 */
    64240,                          /* @ window                */
    0,                              /* @ urgent pointer        */
    0,                              /* @ checksum              */
    payload,                        /* @ payload               */
    payload_length                  /* @ payload length        */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendtcp6(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, const network_ipaddr6_t *saddr, const network_ipaddr6_t *daddr, uint16_t sport, uint16_t dport, uint32_t sequence, uint32_t acknowledgment, uint8_t flags, uint8_t *payload, uint16_t payload_length) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                          /* @ packet buffer   */
    NULL,                            /* @ packet block    */
    dmac,                            /* @ destination mac */
    smac,                            /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV6,  /* @ protocol        */
    NULL,                            /* @ payload         */
    0                                /* @ payload length  */
  );

  netstar_packet_buildipv6(
    packet,                          /* @ packet buffer    */
    NULL,                            /* @ packet block     */
    0,                               /* @ traffic class    */
    0,                               /* @ flow label       */
    NETSTAR_TCP_SIZE+payload_length, /* @ length           */
    NETSTAR_IP_PROTOCOL_TCP,         /* @ next header      */
    255,                             /* @ hop limit        */
    saddr,                           /* @ source addr      */
    daddr,                           /* @ destination addr */
    NULL,                            /* @ payload          */
    0                                /* @ payload length   */
  );

  netstar_packet_buildtcp(
    packet,                          /* @ packet buffer         */
    NULL,                            /* @ packet block          */
    sport,                           /* @ source port           */
    dport,                           /* @ destination port      */
    sequence,                        /* @ sequence              */
    acknowledgment,                  /* @ acknowledgment number */
    flags,                           /* @ flags                 */
    64240,                           /* @ window                */
    0,                               /* @ urgent pointer        */
    0,                               /* @ checksum              */
    payload,                         /* @ payload               */
    payload_length                   /* @ payload length        */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendudp(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, const network_ipaddr4_t *saddr, const network_ipaddr4_t *daddr, uint16_t sport, uint16_t dport, uint8_t *payload, uint16_t payload_length) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                              /* @ packet buffer   */
    NULL,                                /* @ packet block    */
    dmac,                                /* @ destination mac */
    smac,                                /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV4,      /* @ protocol        */
    NULL,                                /* @ payload         */
    0                                    /* @ payload length  */
  );

  netstar_packet_buildipv4(
    packet,                               /* @ packet buffer    */
    NULL,                                 /* @ packet block     */
    0x80,                                 /* @ type of service  */
    NETSTAR_IP_SIZE+NETSTAR_UDP_SIZE+payload_length, /* @ length          */
    random_uint16(),                      /* @ identifier       */
    0,                                    /* @ fragment offset  */
    64,                                   /* @ ttl              */
    NETSTAR_IP_PROTOCOL_UDP,              /* @ protocol         */
    0,                                    /* @ checksum         */
    saddr,                                /* @ source addr      */
    daddr,                                /* @ destination addr */
    NULL,                                 /* @ payload          */
    0                                     /* @ payload length   */
  );

  netstar_packet_buildudp(
    packet,                              /* @ packet buffer    */
    NULL,                                /* @ packet block     */
    sport,                               /* @ source port      */
    dport,                               /* @ destination port */
    NETSTAR_UDP_SIZE+payload_length,     /* @ length           */
    0,                                   /* @ checksum         */
    payload,                             /* @ payload          */
    payload_length                       /* @ payload length   */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}

ssize_t
netstar_sendudp6(netstar_linkwave_t *netstar, const network_macaddr_t *smac, const network_macaddr_t *dmac, const network_ipaddr6_t *saddr, const network_ipaddr6_t *daddr, uint16_t sport, uint16_t dport, uint8_t *payload, uint16_t payload_length) {
  netstar_packet_t *packet = NULL;

  ssize_t sent = -1;

  if (!(packet = netstar_packet_new()))
    return -1;

  netstar_packet_buildethernet(
    packet,                              /* @ packet buffer   */
    NULL,                                /* @ packet block    */
    dmac,                                /* @ destination mac */
    smac,                                /* @ source mac      */
    NETSTAR_ETHERNET_PROTOCOL_IPV6,      /* @ protocol        */
    NULL,                                /* @ payload         */
    0                                    /* @ payload length  */
  );

  netstar_packet_buildipv6(
    packet,                               /* @ packet buffer    */
    NULL,                                 /* @ packet block     */
    0,                                    /* @ traffic class    */
    0,                                    /* @ flow label       */
    NETSTAR_UDP_SIZE+payload_length,      /* @ length           */
    NETSTAR_IP_PROTOCOL_UDP,              /* @ next header      */
    255,                                  /* @ hop limit        */
    saddr,                                /* @ source addr      */
    daddr,                                /* @ destination addr */
    NULL,                                 /* @ payload          */
    0                                     /* @ payload length   */
  );

  netstar_packet_buildudp(
    packet,                              /* @ packet buffer    */
    NULL,                                /* @ packet block     */
    sport,                               /* @ source port      */
    dport,                               /* @ destination port */
    NETSTAR_UDP_SIZE+payload_length,     /* @ length           */
    0,                                   /* @ checksum         */
    payload,                             /* @ payload          */
    payload_length                       /* @ payload length   */
  );

  netstar_packet_coalesce(packet);

  sent = netstar_send(netstar, netstar_packet_buffer(packet), netstar_packet_length(packet));

  netstar_packet_free(packet);

  return sent;
}
