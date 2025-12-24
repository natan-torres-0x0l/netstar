#include "netpacket.h"

#include "netpacket.h"

#include <network/sockets/sockets.h>

#include <net/bpf.h>
#include <net/if.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>

#include <string.h>

#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NETPACKET_BPF_DEVICE_LAST_IDENTIFIER 99
#define NETPACKET_BPF_DEVICE_NAME_LENGTH     sizeof("/dev/bpf00")

struct netpacket_live {
  char bpf_device_name[NETPACKET_BPF_DEVICE_NAME_LENGTH];
  int bpf_device;

  int write_buffer_length;
  uint8_t *write_buffer;

  int read_buffer_length;
  uint8_t *read_buffer;

  socket_t socket;

  netpacket_protocol_t protocol;
  netpacket_layer_t layer;

  netpacket_live_mode_t mode;

  u_int loopback_datalink;
  bool loopback;
};


static inline int
netpacket_live_set_promiscuos(netpacket_live_t *live, const struct netlink_interface *iface, netpacket_live_mode_t mode) {
  int promiscuos = ((mode == NETPACKET_LIVE_MODE_PROMISCUOS) ? 1 : 0);

  if (netlink_interface_isloopback(iface) && mode == NETPACKET_LIVE_MODE_PROMISCUOS)
    return 1;

  if (live->layer != NETPACKET_LAYER_LAYER2)
    return 1;

  if (ioctl(live->bpf_device, BIOCPROMISC, &promiscuos) == -1) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  return 1;
}

static inline int
netpacket_live_bpf_device_open(netpacket_live_t *live) {
  uint8_t id;

  for (id = 0; id < NETPACKET_BPF_DEVICE_LAST_IDENTIFIER; id++) {
    snprintf(live->bpf_device_name, sizeof(live->bpf_device_name), "/dev/bpf%d", id);

    if ((live->bpf_device = open(live->bpf_device_name, O_RDWR)) != -1)
      return 1;
    if (system_geterror() != ENOENT)
      return -1;
  }

  return -1;
}

static inline int
netpacket_live_bpf_device_set_if(netpacket_live_t *live, const struct netlink_interface *iface) {
  u_int immediate = 1, buffer_length = 0;
  struct ifreq ifreq = {0};

  strncpy(ifreq.ifr_name, iface->name, sizeof(ifreq.ifr_name));

  if (ioctl(live->bpf_device, BIOCSETIF, &ifreq) == -1) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  live->loopback = netlink_interface_isloopback(iface);

  if (live->loopback && ioctl(live->bpf_device, BIOCGDLT, &live->loopback_datalink) == -1) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  if (ioctl(live->bpf_device, BIOCIMMEDIATE, &immediate) == -1) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  if (ioctl(live->bpf_device, BIOCGBLEN, &buffer_length) == -1) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  live->write_buffer_length = buffer_length; // + sizeof(struct bpf_hdr);
  if (!(live->write_buffer = (uint8_t *)calloc(1, (size_t)live->write_buffer_length))) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  live->read_buffer_length = buffer_length; // + sizeof(struct bpf_hdr);
  if (!(live->read_buffer = (uint8_t *)calloc(1, (size_t)live->read_buffer_length))) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  return 1;
}

static inline int
netpacket_live_bind_to_interface_addr(netpacket_live_t *live, const struct netlink_interface *iface) {
  struct sockaddr_storage network_addr = {0};
  socklen_t network_addr_length = 0;

  int broadcast = 1, hdrincl = 1;

  switch (live->protocol) {
    case NETPACKET_PROTOCOL_IPV4: {
#ifdef IP_HDRINCL
      if (socket_setoption(live->socket, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) == -1) {
        netpacket_seterror(NETPACKET_ESOCKET);
        return -1;
      }
#endif
      memcpy(&((struct sockaddr_in *)&network_addr)->sin_addr, &iface->addr, NETWORK_IPADDR4_SIZE);
      network_addr.ss_family = AF_INET;
      network_addr_length = sizeof(struct sockaddr_in);

      break;
    }

    case NETPACKET_PROTOCOL_IPV6: {
      memcpy(&((struct sockaddr_in6 *)&network_addr)->sin6_addr, &iface->addr6, NETWORK_IPADDR6_SIZE);
      network_addr.ss_family = AF_INET6;
      network_addr_length = sizeof(struct sockaddr_in6);

      break;
    }
  }

  if (socket_setoption(live->socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  if (socket_bind(live->socket, (struct sockaddr *)&network_addr, network_addr_length) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

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

      if (netpacket_live_bind_to_interface_addr(live, iface) == -1)
        return -1;

      break;
    }

    case NETPACKET_LAYER_LAYER2: {
      if (netpacket_live_bpf_device_open(live) == -1) {
        netpacket_seterror(NETPACKET_ESYSCALL);
        return -1;
      }

      if (netpacket_live_bpf_device_set_if(live, iface) == -1)
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

  if (netpacket_live_set_promiscuos(live, iface, mode) == -1)
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

#define NETPACKET_ETHERNET_SIZE          14

#define NETPACKET_ETHERNET_PROTOCOL_IPV4 0x0800
#define NETPACKET_ETHERNET_PROTOCOL_IPV6 0x08DD

static inline uint16_t
netpacket_ethernet_protocol_by_family(int family) {
  return ((family == AF_INET6) ? htons(NETPACKET_ETHERNET_PROTOCOL_IPV6) : htons(NETPACKET_ETHERNET_PROTOCOL_IPV4));
}

static inline ssize_t
netpacket_live_link_bpf_loopback_in_ethernet(struct bpf_hdr *bpfh, size_t received, void *buffer, size_t buffer_length) {
  const int family = *(const int *)&(*((uint8_t *)bpfh+bpfh->bh_hdrlen));
  uint16_t protocol = netpacket_ethernet_protocol_by_family(family);

  if (buffer_length < NETPACKET_ETHERNET_SIZE)
    return buffer_length;

  memcpy(buffer+NETPACKET_ETHERNET_SIZE-sizeof(protocol), &protocol, sizeof(protocol));
  buffer_length -= NETPACKET_ETHERNET_SIZE;

  if (buffer_length < received-sizeof(family))
    return buffer_length;

  memcpy(buffer+NETPACKET_ETHERNET_SIZE, (uint8_t *)bpfh+bpfh->bh_hdrlen+sizeof(family), (size_t)received-sizeof(family));
  buffer_length -= NETPACKET_ETHERNET_SIZE;

  received = NETPACKET_ETHERNET_SIZE+(received-sizeof(family));

  return received;
}

ssize_t
netpacket_live_link_recv(netpacket_live_t *live, void *buffer, size_t buffer_length, const struct netpacket_timeout *netpacket_timeout) {
  struct timeval timeval = { .tv_sec = ((netpacket_timeout) ? netpacket_timeout->sec+(netpacket_timeout->usec/1000) : 0), };
  struct timeval *timevalptr = ((netpacket_timeout) ? &timeval : NULL);

  struct bpf_hdr *bpfh = (struct bpf_hdr *)live->read_buffer;

  ssize_t received = -1;

  memset(live->read_buffer, 0, live->read_buffer_length);

  if (ioctl(live->bpf_device, BIOCSRTIMEOUT, timevalptr) < 0) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }
  if ((received = (ssize_t)read(live->bpf_device, live->read_buffer, (int)live->read_buffer_length)) < 0) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }
  if (received == 0) {
    netpacket_seterror(NETPACKET_ETIMEOUT);
    return 0;
  }

  if ((received = (ssize_t)bpfh->bh_caplen) > (ssize_t)buffer_length)
    received = (ssize_t)buffer_length;
  if (received == -1)
    return -1;

  if (live->loopback)
    return netpacket_live_link_bpf_loopback_in_ethernet(bpfh, received, buffer, buffer_length);

  memcpy(buffer, (uint8_t *)bpfh+bpfh->bh_hdrlen, (size_t)received);
  return received;
}

ssize_t
netpacket_live_network_recv(netpacket_live_t *live, void *buffer, size_t buffer_length, const struct netpacket_timeout *netpacket_timeout) {
  ssize_t received = -1;

  if ((received = socket_recvfrom(live->socket, buffer, buffer_length, 0, NULL, NULL, (const struct socket_timeout *)netpacket_timeout)) == -1) {
    netpacket_seterror(NETPACKET_ESOCKET);
    return -1;
  }

  if (netpacket_timeout && received == 0) {
    netpacket_seterror(NETPACKET_ETIMEOUT);
    return 0;
  }

  return received;
}

ssize_t
netpacket_live_recv(netpacket_live_t *live, void *buffer, size_t buffer_length, const struct netpacket_timeout *netpacket_timeout) {
  if (live->layer == NETPACKET_LAYER_LAYER3)
    return netpacket_live_network_recv(live, buffer, buffer_length, netpacket_timeout);

  return netpacket_live_link_recv(live, buffer, buffer_length, netpacket_timeout);
}

static inline void
netpacket_live_link_bpf_ethernet_in_loopback(netpacket_live_t *live, const void **buffer, size_t *buffer_length) {
  int family = netpacket_family_by_protocol(live->protocol);

  *buffer_length = ((*buffer_length > live->write_buffer_length) ? live->write_buffer_length : *buffer_length);

  if (*buffer_length < NETPACKET_ETHERNET_SIZE)
    return;

  memcpy(live->write_buffer, &family, sizeof(family));
  memcpy(live->write_buffer+sizeof(family), (uint8_t *)*buffer+NETPACKET_ETHERNET_SIZE, *buffer_length-NETPACKET_ETHERNET_SIZE);

  *buffer_length -= (NETPACKET_ETHERNET_SIZE-sizeof(family));
  *buffer = (const void *)live->write_buffer;
}

static inline ssize_t
netpacket_live_link_send(netpacket_live_t *live, const void *buffer, size_t buffer_length) {
  ssize_t sent = -1;

  if (live->loopback)
    netpacket_live_link_bpf_ethernet_in_loopback(live, (const void **)&buffer, &buffer_length);

  if ((sent = (ssize_t)write(live->bpf_device, buffer, buffer_length)) < 0) {
    netpacket_seterror(NETPACKET_ESYSCALL);
    return -1;
  }

  return sent;
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

#define NETPACKET_PROTOCOL_IPV4_SIZE 20
#define NETPACKET_PROTOCOL_IPV6_SIZE 40

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
    if (live->bpf_device != -1)
      close(live->bpf_device), live->bpf_device = -1;

    if (live->write_buffer)
      free(live->write_buffer), live->write_buffer = NULL;

    if (live->read_buffer)
      free(live->read_buffer), live->read_buffer = NULL;

    free(live);
  }
}
