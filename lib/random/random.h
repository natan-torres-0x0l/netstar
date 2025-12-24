#ifndef _RANDOM_RANDOM_H
#define _RANDOM_RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define random_uint8()  (uint8_t)(random_uint64() & (uint8_t)0xFF)
#define random_int8()   (int8_t)(random_uint64())

#define random_uint16() (uint16_t)(random_uint64() & (uint16_t)0xFFFF)
#define random_int16()  (int16_t)(random_uint64())

#define random_uint32() (uint32_t)(random_uint64() & (uint32_t)0xFFFFFFF)
#define random_int32()  (int32_t)(random_uint64())

#define random_int64()  (int64_t)(random_uint64())

extern uint64_t random_uint64(void);

#ifdef __cplusplus
}
#endif

#endif
