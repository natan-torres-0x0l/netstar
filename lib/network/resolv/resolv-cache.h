#ifndef _NETWORK_RESOLV_CACHE_H
#define _NETWORK_RESOLV_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "resolv.h"

#include <time.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


struct resolv_cache_answer {
  uint8_t *response; size_t length;
  time_t ttl, timestamp;

  struct network_dns_question question;
  network_dns_opcode_t opcode;
};


extern struct resolv_cache_answer *resolv_cache_search(struct resolv_search *);
extern void resolv_cache_push(struct resolv_search *, uint8_t *, size_t);

extern bool resolv_cache_empty(void);
extern void resolv_cache_print(void);

extern void resolv_cache_free(void);

#ifdef __cplusplus
}
#endif

#endif
