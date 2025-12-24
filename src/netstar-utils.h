#ifndef _NETSTAR_UTILS_H
#define _NETSTAR_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-dns-spoofing.h>

#include <netstar-redirect.h>

#include <netstar-vendors.h>
#include <netstar-hosts.h>

#include <netstar-monitor.h>
#include <netstar-filter.h>

#include <netstar-time.h>
#include <netstar.h>

#include <buffers/buffers.h>

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>


extern void netstar_utils_ranges_ipaddr4(const network_ipaddr4_t *, uint8_t, network_ipaddr4_t *, network_ipaddr4_t *);
extern void netstar_utils_ranges_ipaddr6(const network_ipaddr6_t *, uint8_t, network_ipaddr6_t *, network_ipaddr6_t *);


extern network_macaddr_t *netstar_utils_random_macieee802(void);
extern network_ipaddr4_t *netstar_utils_random_ipaddr4(const network_ipaddr4_t *, uint8_t);
extern network_ipaddr6_t *netstar_utils_random_ipaddr6(const network_ipaddr6_t *, uint8_t);


extern network_macaddr_t *netstar_utils_stealth_macieee802(const char *);


// extern bool netstar_utils_regex_match(const char *, const char *, char *, size_t);


extern int netstar_utils_string_vformat(buffer_t *, const char *restrict, va_list);
extern int netstar_utils_string_format(buffer_t *, const char *restrict, ...);

extern size_t netstar_utils_string_readline(const char *, size_t, char *, size_t);
extern long netstar_utils_string_number(const char *);

extern void netstar_utils_strings_free(char **);


extern int netstar_utils_filesystem_vendors(netstar_vendors_t, const char *);

extern int netstar_utils_filesystem_hosts4(netstar_hosts_t, const char *);
extern int netstar_utils_filesystem_hosts6(netstar_hosts_t, const char *);
extern int netstar_utils_filesystem_hosts(netstar_hosts_t, const char *);

extern int netstar_utils_filesystem_preroutes(netstar_redirect_preroutes_t, const char *);

extern int netstar_utils_filesystem_resources(netstar_dns_spoofing_resources_t, const char *);

extern buffer_t *netstar_utils_filesystem_read(const char *, const char *);


extern bool netstar_utils_args_macieee802(network_macaddr_t *, const char *);
extern bool netstar_utils_args_ipaddr4(network_ipaddr4_t *, const char *);
extern bool netstar_utils_args_ipaddr6(network_ipaddr6_t *, const char *);

extern bool netstar_utils_args_cidr4(network_ipaddr4_t *, network_ipaddr4_t *, const char *);
extern bool netstar_utils_args_range4(network_ipaddr4_t *, network_ipaddr4_t *, const char *);

extern bool netstar_utils_args_cidr6(network_ipaddr6_t *, network_ipaddr6_t *, const char *);
extern bool netstar_utils_args_range6(network_ipaddr6_t *, network_ipaddr6_t *, const char *);

extern bool netstar_utils_args_hosts4(netstar_hosts_t, const char *);
extern bool netstar_utils_args_hosts6(netstar_hosts_t, const char *);
extern bool netstar_utils_args_hosts(netstar_hosts_t, const char *);

extern bool netstar_utils_args_host4(netstar_host_t *, const char *);
extern bool netstar_utils_args_host6(netstar_host_t *, const char *);
extern bool netstar_utils_args_host(netstar_host_t *, const char *);

extern bool netstar_utils_args_time(netstar_time_t *, const char *);
extern bool netstar_utils_args_number(uint64_t *, const char *);

extern bool netstar_utils_args_payloadformat(netstar_monitor_payload_format_t *, const char *);

extern bool netstar_utils_args_preroutes(netstar_redirect_preroutes_t, const char *);

extern bool netstar_utils_args_filter(netstar_filter_t *, netstar_t *, const char *);

extern bool netstar_utils_args_resources(netstar_dns_spoofing_resources_t, const char *);

#ifdef __cplusplus
}
#endif

#endif
