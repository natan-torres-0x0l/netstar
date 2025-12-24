#ifndef _FNV_FNV_H
#define _FNV_FNV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define FNV1_OFFSET 0xCBF29CE484222325
#define FNV1_PRIME  0x100000001B3


extern uint64_t fnv1_hash(const uint8_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
