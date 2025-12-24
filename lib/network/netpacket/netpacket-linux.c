#include "netpacket.h"

#include <network/sockets/sockets.h>

#include <linux/if_packet.h>
#include <linux/if_arp.h>

#include <linux/if_ether.h>
#include <linux/if.h>

#include <sys/ioctl.h>

#include <strings/strings.h>
#include <string.h>

#include <stdarg.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef PACKET_IGNORE_OUTGOING
#define PACKET_IGNORE_OUTGOING 23
#endif

#define NETPACKET_PROTOCOL_IPV4_SIZE 20
#define NETPACKET_PROTOCOL_IPV6_SIZE 40

struct netpacket_live {
  struct sockaddr_ll network_link_addr;
  socket_t socket;

  netpacket_protocol_t protocol;
  netpacket_layer_t layer;

  netpacket_live_mode_t mode;
};


static inline int
netpacket_live_set_membership_promiscuos(netpacket_live_t *live, const struct netlink_interface *iface, netpacket_live_mode_t mode) {
  struct packet_mreq packet_mreq = { .mr_ifindex = (int)iface->index, .mr_type = PACKET_MR_PROMISC };
  int packet_request = ((mode == NETPACKET_LIVE_MODE_PROMISCUOS) ? PACKET_ADD_MEMBERSHIP : PACKET_DROP_MEMBERSHIP);

  if (netlink_interface_isloopback(iface))
    return 1;

  if (socket_setoption(live->socket, SOL_PACKET, packet_request, &packet_mreq, sizeof(packet_mreq)) < 0) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  return 1;
}

static inline int
netpacket_live_set_promiscuos(netpacket_live_t *live, const struct netlink_interface *iface, netpacket_live_mode_t mode) {
  struct ifreq ifreq = {0};

  if (netlink_interface_isloopback(iface))
    return 1;

  strncpy(ifreq.ifr_name, iface->name, IFNAMSIZ-1);

  if (ioctl(live->socket, SIOCGIFFLAGS, &ifreq) < 0) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  if (mode == NETPACKET_LIVE_MODE_PROMISCUOS)
    ifreq.ifr_flags |= IFF_PROMISC;
  else
    ifreq.ifr_flags &= ~IFF_PROMISC;

  if (ioctl(live->socket, SIOCSIFFLAGS, &ifreq) < 0) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  return 1;
}

// static inline int
// netpacket_live_set_loopback(netpacket_live_t *live, bool activated) {
//   int ignore_outgoing = ((activated) ? 0 : 1);
//
//   if (socket_setoption(live->socket, SOL_PACKET, PACKET_IGNORE_OUTGOING, &ignore_outgoing, sizeof(ignore_outgoing)) == -1) {
//     netpacket_seterror(NETPACKET_ESOCKET);
//     return -1;
//   }
//
//   return 1;
// }

static inline int
netpacket_live_set_send_buffer(netpacket_live_t *live, int buffer_length) {
  if (socket_setoption(live->socket, SOL_SOCKET, SO_SNDBUF, &buffer_length, sizeof(buffer_length)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  return 1;
}

static inline int
netpacket_live_set_recv_buffer(netpacket_live_t *live, int buffer_length) {
  if (socket_setoption(live->socket, SOL_SOCKET, SO_RCVBUF, &buffer_length, sizeof(buffer_length)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  return 1;
}

static inline int
netpacket_live_bind_to_link_layer(netpacket_live_t *live, const struct netlink_interface *iface) {
  memcpy(&live->network_link_addr.sll_addr, &iface->mac, MACIEEE802_SIZE);
  live->network_link_addr.sll_halen    = MACIEEE802_SIZE;
  live->network_link_addr.sll_ifindex  = (int)iface->index;
  live->network_link_addr.sll_family   = AF_PACKET;
  live->network_link_addr.sll_protocol = htons(ETH_P_ALL);

  if (socket_bind(live->socket, (struct sockaddr *)&live->network_link_addr, sizeof(live->network_link_addr)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  return 1;
}

static int
netpacket_live_bind_to_protocol(netpacket_live_t *live, const struct netlink_interface *iface) {
  int hdrincl = 1, broadcast = 1;

  switch (live->protocol) {
    case NETPACKET_PROTOCOL_IPV4: {
      if (socket_setoption(live->socket, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) == -1) {
        netpacket_seterror(NETPACKET_ESOCKET);
        return -1;
      }
      
      break;
    }

    case NETPACKET_PROTOCOL_IPV6: {
// #ifdef IPV6_HDRINCL
//    if (socket_setoption(live->socket, IPPROTO_IPV6, IPV6_HDRINCL, &hdrincl, sizeof(hdrincl)) == -1) {
//      netpacket_seterror(NETPACKET_ESOCKET);
//      return -1;
//    }
// #endif
      break;
    }
  }

  if (socket_setoption(live->socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

#ifdef SO_BINDTODEVICE
  if (socket_setoption(live->socket, SOL_SOCKET, SO_BINDTODEVICE, iface->name, (socklen_t)string_length(iface->name)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }
#endif

  return 1;
}

static inline int
netpacket_family_by_protocol(netpacket_protocol_t protocol) {
  return ((protocol == NETPACKET_PROTOCOL_IPV6) ? AF_INET6 : AF_INET);
}

static int
netpacket_live_bind_to_interface(netpacket_live_t *live, const struct netlink_interface *iface) {
  switch (live->layer) {
    case NETPACKET_LAYER_LAYER3: {
      int socket_family = netpacket_family_by_protocol(live->protocol);

      if ((live->socket = socket_new(socket_family, SOCK_RAW, IPPROTO_RAW)) == SOCKET_INVALID) {
        netpacket_seterror(NETPACKET_ESOCKET);
        return -1;
      }

      if (netpacket_live_bind_to_protocol(live, iface) == -1)
        return -1;

      break;
    }

    case NETPACKET_LAYER_LAYER2: {
      if ((live->socket = socket_new(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == SOCKET_INVALID) {
        netpacket_seterror(NETPACKET_ESOCKET);
        return -1;
      }

      if (netpacket_live_bind_to_link_layer(live, iface) == -1)
        return -1;

      break;
    }
  }

  return 1;
}

static inline bool
netpacket_live_layer_is_supported(netpacket_live_t *live, netpacket_layer_t layer, netpacket_protocol_t protocol) {
  if ((live->layer = layer) == NETPACKET_LAYER_LAYER3 && (live->protocol = protocol) != NETPACKET_PROTOCOL_IPV4 && live->protocol != NETPACKET_PROTOCOL_IPV6) {
    netpacket_seterror(NETPACKET_EPROTOCOL);
    return false;
  }
  if (live->layer == NETPACKET_LAYER_LAYER2 && live->protocol != NETPACKET_PROTOCOL_ETHERNET) {
    netpacket_seterror(NETPACKET_EPROTOCOL);
    return false;
  }
  if (live->layer != NETPACKET_LAYER_LAYER2 && live->layer != NETPACKET_LAYER_LAYER3) {
    netpacket_seterror(NETPACKET_EPROTOCOL);
    return false;
  }

  return true;
}

static inline int
netpacket_live_initialize(netpacket_live_t *live, const struct netlink_interface *iface, netpacket_layer_t layer, netpacket_protocol_t protocol) {
  if (!netpacket_live_layer_is_supported(live, layer, protocol))
    return -1;

  if (netpacket_live_bind_to_interface(live, iface) == -1)
    return -1;

  if (netpacket_live_set_send_buffer(live, 65535) == -1)
    return -1;

  if (netpacket_live_set_recv_buffer(live, 65535) == -1)
    return -1;

  return 1;
}

netpacket_live_t *
netpacket_live_bind(const struct netlink_interface *iface, netpacket_layer_t layer, netpacket_protocol_t protocol, netpacket_live_mode_t mode) {
  netpacket_live_t *live = NULL;

  if (!(live = (netpacket_live_t *)calloc(1, sizeof(netpacket_live_t)))) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    goto _return;
  }

  if (netpacket_live_initialize(live, iface, layer, protocol) == -1)
    goto _return;

// if (netpacket_live_set_loopback(live, false) == -1)
//   goto _return;

  if (netpacket_live_set_promiscuos(live, iface, mode) == -1)
    goto _return;

  if (netpacket_live_set_membership_promiscuos(live, iface, mode) == -1)
    goto _return;

  live->mode = mode;

  return live;

_return:
  netpacket_live_close(live);
  return NULL;
}

netpacket_protocol_t
netpacket_live_protocol(netpacket_live_t *live) {
  return live->protocol;
}

netpacket_layer_t
netpacket_live_layer(netpacket_live_t *live) {
  return live->layer;
}

ssize_t
netpacket_live_recv(netpacket_live_t *live, void *buffer, size_t length, const struct netpacket_timeout *netpacket_timeout) {
  ssize_t received = -1;

  if ((received = socket_recvfrom(live->socket, buffer, length, 0, NULL, NULL, (const struct socket_timeout *)netpacket_timeout)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  if (netpacket_timeout && received == 0) {
    netpacket_seterror(NETPACKET_ETIMEOUT);
    return 0;
  }

  return received;
}

static inline ssize_t
netpacket_live_send_to(netpacket_live_t *live, const void *buffer, size_t buffer_length, struct sockaddr *network_addr, socklen_t network_addr_length) {
  ssize_t sent = -1;

  if ((sent = socket_sendto(live->socket, buffer, buffer_length, 0, network_addr, network_addr_length)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  return sent;
}

static inline ssize_t
netpacket_live_link_send(netpacket_live_t *live, const void *buffer, size_t buffer_length) {
  return netpacket_live_send_to(live, buffer, buffer_length, (struct sockaddr *)&live->network_link_addr, sizeof(live->network_link_addr));
}

static int
netpacket_live_network_addr_to_sockaddr(netpacket_live_t *live, const void *buffer, size_t buffer_length, struct sockaddr *network_addr, socklen_t *network_addr_length) {
  netpacket_protocol_t protocol = (netpacket_protocol_t)((((const uint8_t *)buffer)[0] >> 4) & 0x0F);

  switch (protocol) {
    case NETPACKET_PROTOCOL_IPV4:
      if (live->protocol != NETPACKET_PROTOCOL_IPV4 || buffer_length < NETPACKET_PROTOCOL_IPV4_SIZE) {
        netpacket_seterror(NETPACKET_EPROTOCOL);
        return -1;
      }

      memcpy(&((struct sockaddr_in *)network_addr)->sin_addr, &((const uint8_t *)buffer)[16], NETWORK_IPADDR4_SIZE);
      network_addr->sa_family = AF_INET;

      *network_addr_length = sizeof(struct sockaddr_in);
      break;

    case NETPACKET_PROTOCOL_IPV6:
      if (live->protocol != NETPACKET_PROTOCOL_IPV6 || buffer_length < NETPACKET_PROTOCOL_IPV6_SIZE) {
        netpacket_seterror(NETPACKET_EPROTOCOL);
        return -1;
      }

      memcpy(&((struct sockaddr_in6 *)network_addr)->sin6_addr, &((const uint8_t *)buffer)[24], NETWORK_IPADDR6_SIZE);
      network_addr->sa_family = AF_INET6;

      *network_addr_length = sizeof(struct sockaddr_in6);
      break;

    default:
      netpacket_seterror(NETPACKET_EPROTOCOL);
      return -1;
  }

  return 1;
}

static inline ssize_t
netpacket_live_network4_send(netpacket_live_t *live, const void *buffer, size_t buffer_length) {
  struct sockaddr_in network_addr = {0};
  socklen_t network_addr_length;

  if (netpacket_live_network_addr_to_sockaddr(live, buffer, buffer_length, (struct sockaddr *)&network_addr, &network_addr_length) == -1)
    return -1;

  return netpacket_live_send_to(live, buffer, buffer_length, (struct sockaddr *)&network_addr, network_addr_length);
}

static inline ssize_t
netpacket_live_network6_send(netpacket_live_t *live, const void *buffer, size_t buffer_length) {
  struct sockaddr_in6 network_addr = {0};
  socklen_t network_addr_length;

  if (netpacket_live_network_addr_to_sockaddr(live, buffer, buffer_length, (struct sockaddr *)&network_addr, &network_addr_length) == -1)
    return -1;

  return netpacket_live_send_to(live, buffer, buffer_length, (struct sockaddr *)&network_addr, network_addr_length);
}

ssize_t
netpacket_live_send(netpacket_live_t *live, const void *buffer, size_t buffer_length) {
  switch (live->protocol) {
    case NETPACKET_PROTOCOL_IPV4:
      return netpacket_live_network4_send(live, buffer, buffer_length);

    case NETPACKET_PROTOCOL_IPV6:
      return netpacket_live_network6_send(live, buffer, buffer_length);
  }

  return netpacket_live_link_send(live, buffer, buffer_length);
}

void
netpacket_live_close(netpacket_live_t *live) {
  if (live) {
    if (live->socket != SOCKET_INVALID)
      socket_close(live->socket), live->socket = SOCKET_INVALID;

    free(live);
  }
}
