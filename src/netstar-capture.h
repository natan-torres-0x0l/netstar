#ifndef _NETSTAR_CAPTURE_H
#define _NETSTAR_CAPTURE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct netstar_capture_packet netstar_capture_packet_t;

#include <netstar-forward.h>
#include <netstar.h>

#include <network/iana/iana.h>

#include <stdint.h>
#include <stddef.h>


typedef enum {
  NETSTAR_CAPTURE_PACKET_FORWARD = 0x01,
  NETSTAR_CAPTURE_PACKET_LIVE    = 0x02
} netstar_capture_packet_flags_t;

struct netstar_capture_packet {
  netstar_forward_protocol_t type;
  netstar_forward_layer_t layer;

  netstar_capture_packet_flags_t flags;

  struct {
    uint16_t          protocol;

    network_macaddr_t smac,
                      dmac;

    uint8_t           *payload;
    size_t             payload_length;

    void              *ptr;
  } layer2;

  struct {
    uint8_t          protocol;

    network_ipaddr_t saddr,
                     daddr;

    uint8_t         *payload;
    size_t           payload_length;

    size_t           length;
    void            *ptr;
  } layer3;

  struct {
    const network_iana_protocol_t *protocol;
    uint16_t         sport,
                     dport;

    uint8_t         *payload;
    size_t           payload_length;

    uint32_t         sequence,
                     acknowledgment;

    size_t           length;
    void            *ptr;
  } layer4;

  struct {
    size_t           length;
    void            *ptr;
  } data;

  uint8_t           *ptr;
  size_t             length;
};


extern void netstar_capture_start(netstar_t *);
extern void netstar_capture_stop(void);

#ifdef __cplusplus
}
#endif

#endif
