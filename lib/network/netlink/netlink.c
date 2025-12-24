#include "netlink.h"

#include <stdlib.h>


bool
netlink_interface_isreachable(const struct netlink_interface *iface) {
  return netlink_interface_isup(iface) && netlink_interface_isrunning(iface);
}

bool
netlink_interface_isup(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFUP;
}

bool
netlink_interface_isrunning(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFRUNNING;
}

bool
netlink_interface_isbroadcast(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFBROADCAST;
}

bool
netlink_interface_isloopback(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFLOOPBACK;
}

bool
netlink_interface_ispointtopoint(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFPOINTTOPOINT;
}

bool
netlink_interface_ismulticast(const struct netlink_interface *iface) {
  return iface->flags & NETLINK_IFMULTICAST;
}


void
netlink_interfaces_free(struct netlink_interface **ifaces) {
  size_t ifs;

  if (ifaces) {
    for (ifs = 0; ifaces[ifs]; ifs++)
      netlink_interface_free(ifaces[ifs]), ifaces[ifs] = NULL;

    free(ifaces);
  }
}
