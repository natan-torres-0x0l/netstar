#ifndef _NETWORK_NETLINK_NETLINK_WINDOWS_H
#define _NETWORK_NETLINK_NETLINK_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/network.h>

#include <stddef.h>
#include <stdint.h>


struct netlink_interface {
  network_ipaddr4_t network, gateway, broadcast, netmask, addr;
  network_ipaddr6_t network6, gateway6, netmask6, addr6;

  network_macaddr_t mac;

  char *name, *adapter_name;

  uint32_t index, flags;
  size_t mtu;

  uint8_t prefix, prefix6;
};

#ifdef __cplusplus
}
#endif

#endif
