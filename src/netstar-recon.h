#ifndef _NETSTAR_RECON_H
#define _NETSTAR_RECON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-hosts.h>
#include <netstar-time.h>

#include <netstar.h>


extern void netstar_recon_hosts4(netstar_t *, netstar_hosts_t);
extern void netstar_recon_host4(netstar_t *, netstar_host_t *);

extern void netstar_recon_hosts6(netstar_t *, netstar_hosts_t);
extern void netstar_recon_host6(netstar_t *, netstar_host_t *);

#ifdef __cplusplus
}
#endif

#endif
