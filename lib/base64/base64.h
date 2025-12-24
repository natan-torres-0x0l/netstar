#ifndef _BASE64_BASE64_H
#define _BASE64_BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


extern char *base64_encode(const uint8_t *, size_t);
extern uint8_t *base64_decode(const char *, size_t *);

#ifdef __cplusplus
}
#endif

#endif
