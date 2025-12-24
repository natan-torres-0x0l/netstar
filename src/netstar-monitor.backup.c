#include "netstar-monitor.h"

/*
#include <netstar-protocols/netstar-telnet.h>
#include <netstar-protocols/netstar-http.h>

#include <netstar-protocols/netstar-mdns.h>
#include <netstar-protocols/netstar-dns.h>

#include <netstar-protocols/netstar-tftp.h>
#include <netstar-protocols/netstar-ftp.h>

#include <netstar-protocols/netstar-icmpv4.h>
#include <netstar-protocols/netstar-icmpv6.h>

#include <netstar-protocols/netstar-igmp.h>

#include <netstar-protocols/netstar-arp.h>
#include <netstar-protocols/netstar-ndp.h>
*/

#include <netstar-capture.h>
#include <netstar-forward.h>

#include <netstar-filter.h>

#include <netstar-utils.h>
#include <netstar-time.h>

#include <netstar-services.h>
#include <netstar-threads.h>

#include <netstar.h>

#include <network/dns/dns.h>

#include <buffers/buffers.h>
#include <strings/strings.h>

#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>
#include <ctype.h>


typedef void (*netstar_monitor_dissector_routine_t)(struct netstar_capture_packet *);

struct netstar_monitor_dissector {
  netstar_monitor_dissector_routine_t routine;
  uint16_t protocol;
};


static bool netstar_monitor_initialized, netstar_monitor_started;

static buffer_t *netstar_monitor_buffer = NULL;


static void
netstar_monitor_write(const char *restrict fmt, ...) {
  va_list args;

  size_t capacity, resize = buffer_length(netstar_monitor_buffer);
  char *buffer = NULL;

  int length = -1;

  va_start(args, fmt);
  if ((length = vsnprintf(NULL, 0, fmt, args)) == -1 || (resize+length > buffer_capacity(netstar_monitor_buffer) && buffer_reserve(netstar_monitor_buffer, (resize+length)*2) == -1))
    return;

  buffer = (char *)(buffer_ptr(netstar_monitor_buffer)+resize);
  capacity = buffer_capacity(netstar_monitor_buffer);

  va_start(args, fmt);
  if (vsnprintf(buffer, capacity, fmt, args) == -1)
    return;

  resize += length;
  buffer_resize(netstar_monitor_buffer, resize);

  va_end(args);
}

static void
netstar_monitor_layer2_arp(struct netstar_capture_packet *packet) {
  struct netstar_arphdr *arph = (struct netstar_arphdr *)packet->layer2.payload;

  switch (ntohs(arph->opcode)) {
    case NETSTAR_ARP_OPCODE_REQUEST: {
      char saddr[NETWORK_IPADDR4_STRLENGTH] = {0}, daddr[NETWORK_IPADDR4_STRLENGTH] = {0};
      netstar_monitor_write("Request who-has %s? Tell %s", network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)),
                                                           network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)));
      break;
    }

    case NETSTAR_ARP_OPCODE_REPLY: {
      char saddr[NETWORK_IPADDR4_STRLENGTH] = {0}, smac[NETWORK_MACADDR_STRLENGTH] = {0};
      netstar_monitor_write("Reply %s is-at %s", network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)),
                                                 network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)));
      break;
    }

    default: {
      char smac[NETWORK_MACADDR_STRLENGTH] = {0}, dmac[NETWORK_MACADDR_STRLENGTH] = {0};
      netstar_monitor_write("%s %04" PRIX16 " %s", network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)),
                                                   network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));
      break;
    }
  }
}


static struct netstar_monitor_dissector netstar_monitor_layer2_dissectors[] = {
  { netstar_monitor_layer2_arp, NETSTAR_ETHERNET_PROTOCOL_ARP },
  { NULL                      , 0x00                          },
};

static inline void
netstar_monitor_layer2_protocol(struct netstar_capture_packet *packet) {
  struct netstar_monitor_dissector *dissectors = NULL;

  for (dissectors = netstar_monitor_layer2_dissectors; dissectors->routine; dissectors++)
    if (packet->layer2.protocol == dissectors->protocol) {
      dissectors->routine(packet);
      return;
    }
}

static inline void
netstar_monitor_layer2(struct netstar_capture_packet *packet) {
  char smac[NETWORK_MACADDR_STRLENGTH] = {0}, dmac[NETWORK_MACADDR_STRLENGTH] = {0};
  struct netstar_ethernet_type *ethertype = NULL;

  netstar_monitor_write("%s - %s  ", network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)),
                                     network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));

  if ((ethertype = netstar_ethernet_type(packet->layer2.protocol)))
    netstar_monitor_write("%s ", ethertype->name);

  netstar_monitor_write("(0x%04" PRIX16 ")  ",  packet->layer2.protocol);

  netstar_monitor_layer2_protocol(packet);
}

static void
netstar_monitor_layer3_icmpv4(struct netstar_capture_packet *packet) {
  struct netstar_icmphdr *icmph = (struct netstar_icmphdr *)(packet->layer3.payload);

  switch (icmph->type) {
    case NETSTAR_ICMP_TYPE_ECHO_REPLY:
      netstar_monitor_write("Echo reply");
      break;

    case NETSTAR_ICMP_TYPE_ECHO:
      netstar_monitor_write("Echo request");
      break;

    case NETSTAR_ICMP_TYPE_DESTINATION_UNREACHABLE:
      netstar_monitor_write("Destination unreachable");
      break;

    case NETSTAR_ICMP_TYPE_TIME_EXCEEDED:
      netstar_monitor_write("Time exceeded");
      break;

    case NETSTAR_ICMP_TYPE_PARAMETER_PROBLEM:
      netstar_monitor_write("Parameter problem");
      break;

    case NETSTAR_ICMP_TYPE_REDIRECT:
      netstar_monitor_write("Redirect");
      break;

    case NETSTAR_ICMP_TYPE_ROUTER_ADVERT:
      netstar_monitor_write("Router advertisement");
      break;

    case NETSTAR_ICMP_TYPE_ROUTER_SOLICIT:
      netstar_monitor_write("Router solicitation");
      break;

    case NETSTAR_ICMP_TYPE_TIMESTAMP_REQUEST:
      netstar_monitor_write("Timestamp request");
      break;

    case NETSTAR_ICMP_TYPE_TIMESTAMP_REPLY:
      netstar_monitor_write("Timestamp reply");
      break;

    case NETSTAR_ICMP_TYPE_UNFORMATION_REQUEST:
      netstar_monitor_write("Unformation request");
      break;

    case NETSTAR_ICMP_TYPE_UNFORMATION_REPLY:
      netstar_monitor_write("Unformation reply");
      break;

    case NETSTAR_ICMP_TYPE_NETMASK_REQUEST:
      netstar_monitor_write("Address mask request");
      break;

    case NETSTAR_ICMP_TYPE_NETMASK_REPLY:
      netstar_monitor_write("Address mask reply");
      break;

    default:
      netstar_monitor_write("Message type 0x%02" PRIX8, icmph->type);
      break;
  }
}

static void
netstar_monitor_layer3_ndp(struct netstar_capture_packet *packet) {
  struct netstar_ndphdr *ndph = (struct netstar_ndphdr *)(packet->layer3.payload);

  switch (ndph->type) {
    case NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT: {
      char dmac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};
      netstar_monitor_write("Neighbor solicitation from %s to %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
                                                                   network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));
      break;
    }

    case NETSTAR_NDP_TYPE_ROUTER_SOLICIT: {
      char dmac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};
      netstar_monitor_write("Router solicitation from %s to %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
                                                                 network_macaddr_format(&packet->layer2.dmac, dmac, sizeof(dmac)));
      break;
    }

    case NETSTAR_NDP_TYPE_NEIGHBOR_ADVERT: {
      char smac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};
      netstar_monitor_write("Neighbor advertisement %s is at %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
                                                                  network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)));
      break;
    }

    case NETSTAR_NDP_TYPE_ROUTER_ADVERT: {
      char smac[NETWORK_MACADDR_STRLENGTH] = {0}, saddr[NETWORK_IPADDR6_STRLENGTH] = {0};
      netstar_monitor_write("Router advertisement %s is at %s", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
                                                                network_macaddr_format(&packet->layer2.smac, smac, sizeof(smac)));
      break;
    }

    case NETSTAR_NDP_TYPE_REDIRECT: {
      netstar_monitor_write("Redirect");
      break;
    }

    default: {
      netstar_monitor_write("Message type 0x%02" PRIX8, ndph->type);
      break;
    }
  }
}

static void
netstar_monitor_layer3_icmpv6(struct netstar_capture_packet *packet) {
  struct netstar_icmpv6hdr *icmp6h = (struct netstar_icmpv6hdr *)(packet->layer3.payload);

  switch (icmp6h->type) {
    case NETSTAR_ICMPV6_TYPE_ECHO_REPLY:
      netstar_monitor_write("Echo reply");
      break;

    case NETSTAR_ICMPV6_TYPE_ECHO:
      netstar_monitor_write("Echo request");
      break;

    case NETSTAR_ICMPV6_TYPE_DESTINATION_UNREACHABLE:
      netstar_monitor_write("Destination unreachable");
      break;

    case NETSTAR_ICMPV6_TYPE_PACKET_TOO_BIG:
      netstar_monitor_write("Packet too big");
      break;

    case NETSTAR_ICMPV6_TYPE_TIME_EXCEEDED:
      netstar_monitor_write("Time exceeded");
      break;

    case NETSTAR_ICMPV6_TYPE_PARAMETER_PROBLEM:
      netstar_monitor_write("Parameter problem");
      break;

    default:
      netstar_monitor_layer3_ndp(packet);
      break;
  }
}

static void
netstar_monitor_layer3_igmp(struct netstar_capture_packet *packet) {
  struct netstar_igmphdr *igmph = (struct netstar_igmphdr *)(packet->layer3.payload);

  switch (igmph->type) {
    case NETSTAR_IGMP_TYPE_MEMBERSHIP_QUERY: {
      netstar_monitor_write("Membership query");
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V1: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};
      netstar_monitor_write("Membership report v1 %s", network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V2: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};
      netstar_monitor_write("Membership report v2 %s", network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_LEAVE_GROUP: {
      char group[NETWORK_IPADDR4_STRLENGTH] = {0};
      netstar_monitor_write("Leave group %s", network_ipaddr4_format((network_ipaddr4_t *)igmph->group, group, sizeof(group)));
      break;
    }

    case NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V3: {
      struct netstar_igmpv3hdr *igmpv3h = (struct netstar_igmpv3hdr *)igmph;
      netstar_monitor_write("Membership report v3 %" PRIu16, igmpv3h->groups_count);
      break;
    }

    default:
      netstar_monitor_write("Message type 0x%02" PRIX8, igmph->type);
      break;
  }
}

static void
netstar_monitor_layer3_gre(struct netstar_capture_packet *packet) {
  struct netstar_grehdr *greh = (struct netstar_grehdr *)(packet->layer3.payload);
  struct netstar_ethernet_type *ethertype = NULL;

  netstar_monitor_write("Protocol type ");

  if ((ethertype = netstar_ethernet_type(greh->protocol)))
    netstar_monitor_write("%s ", ethertype->name);

  netstar_monitor_write("(0x%04" PRIX16 ")", greh->protocol);
}


static struct netstar_monitor_dissector netstar_monitor_layer3_dissectors[] = {
  { netstar_monitor_layer3_icmpv4, NETSTAR_IP_PROTOCOL_ICMPV4 },
  { netstar_monitor_layer3_icmpv6, NETSTAR_IP_PROTOCOL_ICMPV6 },
  { netstar_monitor_layer3_igmp  , NETSTAR_IP_PROTOCOL_IGMP   },
  { netstar_monitor_layer3_gre   , NETSTAR_IP_PROTOCOL_GRE    },
  { NULL                         , 0x00                       },
};

static inline void
netstar_monitor_layer3_protocol(struct netstar_capture_packet *packet) {
  struct netstar_monitor_dissector *dissectors = NULL;

  for (dissectors = netstar_monitor_layer3_dissectors; dissectors->routine; dissectors++)
    if (packet->layer3.protocol == dissectors->protocol) {
      dissectors->routine(packet);
      return;
    }
}

static void
netstar_monitor_layer3(struct netstar_capture_packet *packet) {
  char saddr[NETWORK_IPADDR6_STRLENGTH] = {0}, daddr[NETWORK_IPADDR6_STRLENGTH] = {0};
  struct netstar_ip_protocol *protocol = NULL;

  if (packet->layer2.protocol == NETSTAR_ETHERNET_PROTOCOL_IPV4)
    netstar_monitor_write("%s - %s  ", network_ipaddr4_format(&packet->layer3.saddr.v4, saddr, sizeof(saddr)),
                                       network_ipaddr4_format(&packet->layer3.daddr.v4, daddr, sizeof(daddr)));
  else if (packet->layer2.protocol == NETSTAR_ETHERNET_PROTOCOL_IPV6)
    netstar_monitor_write("%s - %s  ", network_ipaddr6_format(&packet->layer3.saddr.v6, saddr, sizeof(saddr)),
                                       network_ipaddr6_format(&packet->layer3.daddr.v6, daddr, sizeof(daddr)));

  if ((protocol = netstar_ip_protocol(packet->layer3.protocol)))
    netstar_monitor_write("%s ", protocol->name);

  netstar_monitor_write("(0x%02" PRIX8 ")  ", packet->layer3.protocol);

  netstar_monitor_layer3_protocol(packet);
}

static void
netstar_monitor_layer4_dns(struct netstar_capture_packet *packet) {
}

static void
netstar_monitor_layer4_mdns(struct netstar_capture_packet *packet) {
}

static void
netstar_monitor_layer4_ftp(struct netstar_capture_packet *packet) {
}

static void
netstar_monitor_layer4_http(struct netstar_capture_packet *packet) {
}

static void
netstar_monitor_layer4_telnet(struct netstar_capture_packet *packet) {
}

static inline void
netstar_monitor_layer4_protocol(struct netstar_capture_packet *packet) {
  netstar_dissectors(packet->layer, packet->layer4.protocol->port);
}

static inline void
netstar_monitor_layer4(struct netstar_capture_packet *packet) {
  if (packet->layer4.protocol->port >= 1 && packet->layer4.protocol->port <= 1025)
    netstar_monitor_write("%s  ", packet->layer4.protocol->name);

  netstar_monitor_write("%" PRIu16 " - %" PRIu16 "  ", packet->layer4.sport,
                                                       packet->layer4.dport);

// netstar_monitor_layer4_protocol(packet);
}

static inline uint8_t *
netstar_monitor_packet_payload(struct netstar_capture_packet *packet, size_t *length) {
  if (packet->layer4.payload && packet->layer4.payload_length) {
    *length = packet->layer4.payload_length;
    return packet->layer4.payload;
  }

  if (packet->layer3.payload && packet->layer3.payload_length) {
    *length = packet->layer3.payload_length;
    return packet->layer3.payload;
  }

  if (packet->layer2.payload && packet->layer2.payload_length) {
    *length = packet->layer2.payload_length;
    return packet->layer2.payload;
  }

  return NULL;
}

static void
netstar_monitor_packet_data(struct netstar_capture_packet *packet, netstar_monitor_payload_format_t payload_format) {
  size_t payload_length = 0, byte;
  uint8_t *payload = NULL;

  if (!(payload = netstar_monitor_packet_payload(packet, &payload_length)))
    return;

  netstar_monitor_write("\r\n");

  for (byte = 0; byte < payload_length; byte++)
    switch (payload_format) {
      case NETSTAR_MONITOR_PAYLOAD_DATA_HEXADECIMAL: {
        netstar_monitor_write("%02X ", payload[byte]);
        break;
      }

      case NETSTAR_MONITOR_PAYLOAD_DATA_OCTAL: {
        netstar_monitor_write("%02o ", payload[byte]);
        break;
      }

      case NETSTAR_MONITOR_PAYLOAD_DATA_DECIMAL: {
        netstar_monitor_write("%02d ", payload[byte]);
        break;
      }

      case NETSTAR_MONITOR_PAYLOAD_DATA_ASCII: {
        char ascii = (isgraph((char)payload[byte]) && !isspace((char)payload[byte]) ? payload[byte] : '.');
        netstar_monitor_write("%c", ascii);
        break;
      }
    }
}

static bool
netstar_monitor_build(void) {
  if (!buffer_length(netstar_monitor_buffer))
    return false;

  buffer_append(netstar_monitor_buffer, "\r\n\0", 3);
  return true;
}

static void
netstar_monitor(netstar_t *netstar, struct netstar_capture_packet *packet, void *args) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)args;

  if (monitor->filter.state.source && !netstar_filter_compile(&monitor->filter, packet))
    return;

  buffer_truncate(netstar_monitor_buffer, 0);

  if (monitor->mode & NETSTAR_MONITOR_MODE_LAYER2 && packet->layer & NETSTAR_FORWARD_LAYER2)
    netstar_monitor_layer2(packet);

  if (monitor->mode & NETSTAR_MONITOR_MODE_LAYER3 && packet->layer & NETSTAR_FORWARD_LAYER3)
    netstar_monitor_layer3(packet);

  if (monitor->mode & NETSTAR_MONITOR_MODE_LAYER4 && packet->layer & NETSTAR_FORWARD_LAYER4)
    netstar_monitor_layer4(packet);

  if (monitor->payload_format)
    netstar_monitor_packet_data(packet, monitor->payload_format);

  if (netstar_monitor_build())
    netstar_log("\b \b[ monitor ] %s", (char *)buffer_ptr(netstar_monitor_buffer));
}

static void
netstar_monitor_initialize(netstar_t *netstar, void *context) {
  if (netstar_monitor_initialized)
    return;

  if (!(netstar_monitor_buffer = buffer_make(0, 1024)))
    return;

  netstar_monitor_initialized = true;
}

static void
netstar_monitor_deinitialize(void *context) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)context;

  if (!netstar_monitor_initialized)
    return;

  buffer_free(netstar_monitor_buffer);
  netstar_monitor_buffer = NULL;

  netstar_monitor_free(monitor);

  netstar_monitor_initialized = false;
}

static void
netstar_monitor_start(void *context) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)context;

  if (netstar_monitor_started)
    return;

  netstar_forward_add(netstar_monitor, NETSTAR_FORWARD_ANY, monitor);

  netstar_monitor_started = true;
  netstar_log("#netstar-monitor started\r\n");
}

static void
netstar_monitor_stop(void *context) {
  if (!netstar_monitor_started)
    return;

  netstar_log("#netstar-monitor stopping...\r\n");

  netstar_forward_remove(netstar_monitor);

  netstar_monitor_started = false;
}

int
netstar_monitor_new(struct netstar_monitor *monitor, netstar_t *netstar) {
  monitor->mode = NETSTAR_MONITOR_MODE_NORMAL;
  monitor->netstar = netstar;

  return 1;
}

void
netstar_monitor_free(struct netstar_monitor *monitor) {
  if (monitor) {
    netstar_filter_free(&monitor->filter);
  }
}

static struct netstar_monitor netstar_monitor_context;

static struct netstar_service netstar_monitor_context_service = {
  netstar_monitor_initialize, netstar_monitor_deinitialize,
  netstar_monitor_start, netstar_monitor_stop,
  "netstar:monitor", &netstar_monitor_context,
};

struct netstar_service *
netstar_monitor_service(void) {
  return &netstar_monitor_context_service;
}

__attribute__((__constructor__))
static inline void
netstar_monitor_register_service(void) {
  netstar_services_register(netstar_monitor_service());
}
