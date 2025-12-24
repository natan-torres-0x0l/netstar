#ifndef _NETWORK_IANA_IANA_H
#define _NETWORK_IANA_IANA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NETWORK_IANA_PROTOCOLS_SIZE   0x176F // sizeof(network_iana_protocols)/sizeof(network_iana_protocols[0])
#define NETWORK_IANA_UNKNOWN_PROTOCOL &network_iana_unknown_protocol

typedef struct network_iana_protocol {
  const char *name; uint16_t port; /* const char *protocol; */
} network_iana_protocol_t;


extern const struct network_iana_protocol network_iana_unknown_protocol;
extern const struct network_iana_protocol network_iana_protocols[];


extern const struct network_iana_protocol *network_iana_protocolbyname(const char *);
extern const struct network_iana_protocol *network_iana_protocolbyport(uint16_t);

#ifdef __cplusplus
}
#endif

#endif
