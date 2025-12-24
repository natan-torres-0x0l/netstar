#ifndef _NETWORK_NETWORK_H
#define _NETWORK_NETWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/macieee802/macieee802.h>
#include <network/inet/inet.h>

#include <network/sockets/sockets.h>

#define NETWORK_MACADDR_IBROADCAST    {{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}
#define NETWORK_MACADDR_IV6ALLNODES   {{ 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 }}
#define NETWORK_MACADDR_IV6ALLROUTERS {{ 0x33, 0x33, 0x00, 0x00, 0x00, 0x02 }}
#define NETWORK_MACADDR_IV4ALLNODES   {{ 0x01, 0x00, 0x5E, 0x00, 0x00, 0x01 }}
#define NETWORK_MACADDR_IV4ALLROUTERS {{ 0x01, 0x00, 0x5E, 0x00, 0x00, 0x02 }}

#define NETWORK_MACADDR_BROADCAST     MACIEEE802_BROADCAST
#define NETWORK_MACADDR_V6ALLNODES    MACIEEE802_V6ALLNODES
#define NETWORK_MACADDR_V6ALLROUTERS  MACIEEE802_V6ALLROUTERS
#define NETWORK_MACADDR_V4ALLNODES    MACIEEE802_V4ALLNODES
#define NETWORK_MACADDR_V4ALLROUTERS  MACIEEE802_V4ALLROUTERS

#define NETWORK_MACADDR_STRLENGTH     MACIEEE802_STRLENGTH
#define NETWORK_MACADDR_SIZE          MACIEEE802_SIZE

typedef macieee802_t network_macaddr_t;


extern int network_macaddr_compare(const network_macaddr_t *, const network_macaddr_t *);

extern char *network_macaddr_format(const network_macaddr_t *, char *, size_t);
extern int network_macaddr_parse(network_macaddr_t *, const char *);


#define NETWORK_IPADDR4_IBROADCAST  {{{ 0xFF, 0xFF, 0xFF, 0xFF }}}
#define NETWORK_IPADDR4_ILOOPBACK   {{{ 0x7F, 0x00, 0x00, 0x01 }}}
#define NETWORK_IPADDR4_ILOCALHOST  NETWORK_IPADDR4_ILOOPBACK
#define NETWORK_IPADDR4_IALL        {{{ 0x00, 0x00, 0x00, 0x00 }}}
#define NETWORK_IPADDR4_IANY        NETWORK_IPADDR4_IALL
#define NETWORK_IPADDR4_IALLNODES   {{{ 0xE0, 0x00, 0x00, 0x01 }}}
#define NETWORK_IPADDR4_IALLROUTERS {{{ 0xE0, 0x00, 0x00, 0x02 }}}

#define NETWORK_IPADDR4_UBROADCAST  0xFFFFFFFF
#define NETWORK_IPADDR4_ULOOPBACK   0x7F000001
#define NETWORK_IPADDR4_ULOCALHOST  NETWORK_IPADDR4_ULOOPBACK
#define NETWORK_IPADDR4_UALL        0x00000000
#define NETWORK_IPADDR4_UANY        NETWORK_IPADDR4_UALL
#define NETWORK_IPADDR4_UALLNODES   0xE0000001
#define NETWORK_IPADDR4_UALLROUTERS 0xE0000002

#define NETWORK_IPADDR4_BROADCAST   INETV4_BROADCAST
#define NETWORK_IPADDR4_LOOPBACK    INETV4_LOOPBACK
#define NETWORK_IPADDR4_LOCALHOST   INETV4_LOCALHOST
#define NETWORK_IPADDR4_ALL         INETV4_ALL
#define NETWORK_IPADDR4_ANY         INETV4_ANY
#define NETWORK_IPADDR4_ALLNODES    INETV4_ALLNODES
#define NETWORK_IPADDR4_ALLROUTERS  INETV4_ALLROUTERS

#define NETWORK_IPADDR4_STRLENGTH   INETV4_STRLENGTH
#define NETWORK_IPADDR4_SIZE        INETV4_SIZE
#define NETWORK_IPADDR4             INETV4_ADDRESS

typedef inetv4_t network_ipaddr4_t;


extern bool network_ipaddr4_ismulticast(const network_ipaddr4_t *);
extern bool network_ipaddr4_isloopback(const network_ipaddr4_t *);

extern bool network_ipaddr4_isclassa(const network_ipaddr4_t *);
extern bool network_ipaddr4_isclassb(const network_ipaddr4_t *);
extern bool network_ipaddr4_isclassc(const network_ipaddr4_t *);

extern int network_ipaddr4_compare(const network_ipaddr4_t *, const network_ipaddr4_t *);

extern char *network_ipaddr4_format(const network_ipaddr4_t *, char *, size_t);

extern int network_ipaddr4_cidr(const char *, uint8_t *, network_ipaddr4_t *, network_ipaddr4_t *);
extern int network_ipaddr4_parse(network_ipaddr4_t *, const char *);


#define NETWORK_IPADDR6_ILOOPBACK   {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define NETWORK_IPADDR6_ILOCALHOST  NETWORK_IPADDR6_ILOOPBACK
#define NETWORK_IPADDR6_IALL        {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define NETWORK_IPADDR6_IANY        NETWORK_IPADDR6_IALL
#define NETWORK_IPADDR6_IALLNODES   {{{ 0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define NETWORK_IPADDR6_IALLROUTERS {{{ 0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}}

#define NETWORK_IPADDR6_LOOPBACK    INETV6_LOOPBACK
#define NETWORK_IPADDR6_LOCALHOST   INETV6_LOCALHOST
#define NETWORK_IPADDR6_ALL         INETV6_ALL
#define NETWORK_IPADDR6_ANY         INETV6_ALL
#define NETWORK_IPADDR6_ALLNODES    INETV6_ALLNODES
#define NETWORK_IPADDR6_ALLROUTERS  INETV6_ALLROUTERS

#define NETWORK_IPADDR6_STRLENGTH   INETV6_STRLENGTH
#define NETWORK_IPADDR6_SIZE        INETV6_SIZE
#define NETWORK_IPADDR6             INETV6_ADDRESS

typedef inetv6_t network_ipaddr6_t;


extern bool network_ipaddr6_isunspecified(const network_ipaddr6_t *);

extern bool network_ipaddr6_isloopback(const network_ipaddr6_t *);
extern bool network_ipaddr6_ismulticast(const network_ipaddr6_t *);

extern bool network_ipaddr6_islinklocal(const network_ipaddr6_t *);
extern bool network_ipaddr6_issitelocal(const network_ipaddr6_t *);
extern bool network_ipaddr6_isglobal(const network_ipaddr6_t *);

extern bool network_ipaddr6_isv4mapped(const network_ipaddr6_t *);

extern int network_ipaddr6_compare(const network_ipaddr6_t *, const network_ipaddr6_t *);

extern char *network_ipaddr6_format(const network_ipaddr6_t *, char *, size_t);

extern int network_ipaddr6_cidr(const char *, uint8_t *, network_ipaddr6_t *, network_ipaddr6_t *);
extern int network_ipaddr6_parse(network_ipaddr6_t *, const char *);


#include "network-endpoint.h"


typedef uint8_t network_ipaddr_size_t;

typedef struct network_ipaddr {
  union {
    network_ipaddr4_t v4;
    network_ipaddr6_t v6;
  };

  network_ipaddr_size_t size;
} network_ipaddr_t;


extern int network_ipaddr_compare(const network_ipaddr_t *, const network_ipaddr_t *);

extern char *network_ipaddr_format(network_ipaddr_t *, char *, size_t);
extern int network_ipaddr_parse(network_ipaddr_t *, const char *);
extern int network_ipaddr(const char *);


extern uint16_t network_u16tohost(uint16_t);
extern uint16_t network_u16tonet(uint16_t);

extern uint32_t network_u32tohost(uint32_t);
extern uint32_t network_u32tonet(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
