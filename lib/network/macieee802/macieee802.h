#ifndef _NETWORK_MACIEEE802_MACIEEE802_H
#define _NETWORK_MACIEEE802_MACIEEE802_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define MACIEEE802_IBROADCAST    { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } }
#define MACIEEE802_IV6ALLNODES   { { 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 } }
#define MACIEEE802_IV6ALLROUTERS { { 0x33, 0x33, 0x00, 0x00, 0x00, 0x02 } }
#define MACIEEE802_IV4ALLNODES   { { 0x01, 0x00, 0x5E, 0x00, 0x00, 0x01 } }
#define MACIEEE802_IV4ALLROUTERS { { 0x01, 0x00, 0x5E, 0x00, 0x00, 0x02 } }

#define MACIEEE802_BROADCAST     macieee802_broadcast
#define MACIEEE802_V6ALLNODES    macieee802_v6allnodes
#define MACIEEE802_V6ALLROUTERS  macieee802_v6allrouters
#define MACIEEE802_V4ALLNODES    macieee802_v4allnodes
#define MACIEEE802_V4ALLROUTERS  macieee802_v4allrouters

#define MACIEEE802_SIZE          0x06 // 06 bytes
#define MACIEEE802_STRLENGTH     0x12 // 18 bytes


typedef struct macieee802 {
  uint8_t u8[MACIEEE802_SIZE];
} macieee802_t;


extern const struct macieee802 macieee802_broadcast;
extern const struct macieee802 macieee802_v6allnodes;
extern const struct macieee802 macieee802_v6allrouters;
extern const struct macieee802 macieee802_v4allnodes;
extern const struct macieee802 macieee802_v4allrouters;

extern int macieee802_compare(const struct macieee802 *, const struct macieee802 *);

extern char *macieee802_format(const struct macieee802 *, char *, size_t);
extern int macieee802_parse(struct macieee802 *, const char *);

#ifdef __cplusplus
}
#endif

#endif
