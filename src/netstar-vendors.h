#ifndef _NETSTAR_VENDORS_H
#define _NETSTAR_VENDORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/hashset/hashset.h>

#include <netstar.h>

#define NETSTAR_VENDORS_FILENAME "share/vendors/netstar-vendors-mac.list"

#define NETSTAR_VENDOR_STRLENGTH 256

struct netstar_vendor {
  char mac[NETWORK_MACADDR_STRLENGTH], name[NETSTAR_VENDOR_STRLENGTH];
};

typedef hashset_t * netstar_vendors_t;


extern int netstar_vendors_add(netstar_vendors_t, struct netstar_vendor *);

extern char *netstar_vendors_namebymac(const network_macaddr_t *);

extern void netstar_vendors_initialize(void);
extern void netstar_vendors_deinitialize(void);

#ifdef __cplusplus
}
#endif

#endif
