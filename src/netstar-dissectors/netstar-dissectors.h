#ifndef _NETSTAR_PROTOCOL_H
#define _NETSTAR_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-capture.h>
#include <netstar-forward.h>

#include <buffers/buffers.h>

#include <inttypes.h>
#include <stdint.h>

typedef enum {
  NETSTAR_DISSECTOR_ETHERNET_PROTOCOL, NETSTAR_DISSECTOR_IP_PROTOCOL, NETSTAR_DISSECTOR_TCP_PROTOCOL, NETSTAR_DISSECTOR_UDP_PROTOCOL
} netstar_dissector_protocol_t;

typedef enum {
  NETSTAR_DISSECTOR_LAYER2 = 0x01, NETSTAR_DISSECTOR_LAYER3 = 0x02, NETSTAR_DISSECTOR_LAYER4 = 0x08, NETSTAR_DISSECTOR_LAYER7 = 0x10,
  NETSTAR_DISSECTOR_LAYERS = NETSTAR_DISSECTOR_LAYER2|NETSTAR_DISSECTOR_LAYER3|NETSTAR_DISSECTOR_LAYER4|NETSTAR_DISSECTOR_LAYER7
} netstar_dissector_layer_t;

typedef enum {
  NETSTAR_DISSECTOR_PACKET_SOURCE_HARDWARE,
  NETSTAR_DISSECTOR_PACKET_DESTINATION_HARDWARE,
  NETSTAR_DISSECTOR_PACKET_ETHERNET_PROTOCOL,
  NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS,
  NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS,
  NETSTAR_DISSECTOR_PACKET_IP_PROTOCOL,
  NETSTAR_DISSECTOR_PACKET_SOURCE_PORT,
  NETSTAR_DISSECTOR_PACKET_DESTINATION_PORT,
  NETSTAR_DISSECTOR_PACKET_APPLICATION_PROTOCOL,
  NETSTAR_DISSECTOR_PACKET_INFORMATION
} netstar_dissector_slice_t;

#define NETSTAR_DISSECTOR_BUFFERS 10

typedef struct {
  buffer_t *buffers[NETSTAR_DISSECTOR_BUFFERS];
  netstar_dissector_layer_t layers;
} netstar_dissector_t;

typedef void (*netstar_dissector_routine_t)(netstar_dissector_t *, struct netstar_capture_packet *);

typedef uint16_t netstar_dissector_service_t;

struct netstar_dissector_protocol {
  netstar_dissector_routine_t routine;

  netstar_dissector_protocol_t type;
  netstar_dissector_layer_t layer;

  netstar_dissector_service_t service;
};


extern void netstar_dissector(netstar_dissector_t *, struct netstar_capture_packet *, netstar_dissector_protocol_t, netstar_dissector_service_t);
extern void netstar_dissector_dissect(netstar_dissector_t *, struct netstar_capture_packet *);

extern void netstar_dissector_format(netstar_dissector_t *, netstar_dissector_slice_t, const char *restrict, ...);
extern void netstar_dissector_write(netstar_dissector_t *, netstar_dissector_slice_t, const char *restrict, size_t);
extern void netstar_dissector_append(netstar_dissector_t *, netstar_dissector_slice_t, const char *restrict, size_t);

extern void netstar_dissectors_register(struct netstar_dissector_protocol *); 

extern int netstar_dissector_initialize(netstar_dissector_t *);
extern void netstar_dissector_deinitialize(netstar_dissector_t *);

#ifdef __cplusplus
}
#endif

#endif
