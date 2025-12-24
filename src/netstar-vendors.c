#include "netstar-vendors.h"

#include <netstar-utils.h>

#include <strings/strings.h>
#include <string.h>

#include <stdlib.h>


static netstar_thread_mutex_t netstar_vendors_mutex;
static bool netstar_vendors_initialized;

static netstar_vendors_t netstar_vendors;

static struct netstar_vendor netstar_vendor_unknown;


int
netstar_vendors_add(netstar_vendors_t vendors, struct netstar_vendor *vendor) {
  return hashset_insert(vendors, hashset_clvalue(vendor, sizeof(struct netstar_vendor)));
}

static bool
netstar_vendors_vendor_by_mac(const void *value, va_list args) {
  const struct netstar_vendor *vendor = (const struct netstar_vendor *)value;
  const char *vendor_mac = va_arg(args, const char *);

  return !memcmp(vendor->mac, vendor_mac, string_length(vendor->mac));
}

static char *
netstar_vendors_name_by_mac(const network_macaddr_t *mac) {
  char vendor_mac[NETWORK_MACADDR_STRLENGTH] = {0};
  struct netstar_vendor *vendor = NULL;

  network_macaddr_format(mac, vendor_mac, sizeof(vendor_mac));

  if (!(vendor = (struct netstar_vendor *)hashset_findif(netstar_vendors, netstar_vendors_vendor_by_mac, vendor_mac)))
    vendor = &netstar_vendor_unknown;

  return vendor->name;
}

char *
netstar_vendors_namebymac(const network_macaddr_t *mac) {
  char *vendor_name = netstar_vendor_unknown.name;

  netstar_thread_mutex_lock(&netstar_vendors_mutex);

  if (!netstar_vendors_initialized)
    goto _return;

  vendor_name = netstar_vendors_name_by_mac(mac);

_return:
  netstar_thread_mutex_unlock(&netstar_vendors_mutex);
  return vendor_name;
}

void
netstar_vendors_initialize(void) {
  netstar_thread_mutex_lock(&netstar_vendors_mutex);

  if (netstar_vendors_initialized)
    goto _return;

// netstar_log("\b \b[ vendors ] reading vendors information base \"%s\"\r\n", NETSTAR_VENDORS_FILENAME);

  if (!(netstar_vendors = hashset_new(free))) {
 // netstar_critical("\b \b[ vendors ] %s base not found\r\n", NETSTAR_VENDORS_FILENAME);
    goto _return;
  }

  if (netstar_utils_filesystem_vendors(netstar_vendors, NETSTAR_VENDORS_FILENAME) == -1) {
    netstar_warning("\b \b[ vendors ] %s base not found\r\n", NETSTAR_VENDORS_FILENAME);
    goto _return;
  }

  netstar_vendors_initialized = true;

// netstar_log("\b \b[ vendors ] read %zu vendors\r\n", hashset_size(netstar_vendors));

_return:
  netstar_thread_mutex_unlock(&netstar_vendors_mutex);
}

void
netstar_vendors_deinitialize(void) {
  netstar_thread_mutex_lock(&netstar_vendors_mutex);

  if (!netstar_vendors_initialized)
    return;

  hashset_free(netstar_vendors);
  netstar_vendors = NULL;

  netstar_vendors_initialized = false;

  netstar_thread_mutex_unlock(&netstar_vendors_mutex);
}
