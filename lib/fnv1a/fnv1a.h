#ifndef _FNV1A_FNV1A_H
#define _FNV1A_FNV1A_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define FNV1A_OFFSET 0xCBF29CE484222325
#define FNV1A_PRIME  0x100000001B3


extern uint64_t fnv1a_hash(const uint8_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
