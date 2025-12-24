#ifndef _NETWORK_RESOLV_RESOLV_H
#define _NETWORK_RESOLV_RESOLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "resolv-errors.h"

#include <network/network.h>
#include <network/dns/dns.h>

/*
#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include <basetsd.h>

#else // elif defined __unix__ || defined __unix
  #include <sys/types.h>

#endif
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


struct resolv_search {
  struct network_dns_question question;
  network_dns_opcode_t opcode;
};

struct resolv_timeout {
  long sec, usec;
};


extern ssize_t resolv_search(struct resolv_search *, const char *, uint8_t *, size_t, struct resolv_timeout *);


struct resolv_hostentry {
  char *hostname, **names, *name, **addrs, *addr;
  size_t names_count, addrs_count;
};


extern struct resolv_hostentry *resolv_host4byname(const char *);
#define resolv_hostbyname resolv_host4byname
extern struct resolv_hostentry *resolv_host6byname(const char *);

extern struct resolv_hostentry *resolv_hostbyaddr(const char *);
extern void resolv_hostentry_free(struct resolv_hostentry *);


struct resolv_cname {
  char *hostname, **names;
  size_t nnames;
};


extern struct resolv_cname *resolv_cnamebyname(const char *);
extern void resolv_cname_free(struct resolv_cname *);


struct resolv_mx {
  char *hostname;

  struct {
    char *exchange;
    uint16_t preference;
  } *mx;

  size_t mx_count;
};


extern struct resolv_mx *resolv_mxbyname(const char *);
extern void resolv_mx_free(struct resolv_mx *);


struct resolv_ns {
  char *hostname, **nsnames;
  size_t nsnames_count;
};


extern struct resolv_ns *resolv_nsbyname(const char *);
extern void resolv_ns_free(struct resolv_ns *);


struct resolv_ptr {
  char *ptr;
};


extern struct resolv_ptr *resolv_ptrbyaddr(const char *);
extern void resolv_ptr_free(struct resolv_ptr *);


struct resolv_soa {
  char *hostname, *mname, *rname;
  uint32_t serial, refresh, retry, expire, minimum;
};


extern struct resolv_soa *resolv_soabyname(const char *);
extern void resolv_soa_free(struct resolv_soa *);


struct resolv_txt {
  char *hostname, **txts;
  size_t txts_count;
};


extern struct resolv_txt *resolv_txtbyname(const char *);
extern void resolv_txt_free(struct resolv_txt *);

#ifdef __cplusplus
}
#endif

#endif
