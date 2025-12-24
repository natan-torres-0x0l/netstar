#include "netstar-dissectors.h"

#include <netstar-utils.h>

#include <strings/strings.h>

#define NETSTAR_DISSECTORS 20+1


static struct netstar_dissector_protocol *netstar_dissectors[NETSTAR_DISSECTORS+1];
static size_t netstar_dissectors_registered;

extern struct netstar_dissector_protocol *netstar_dissector_root;


static int
netstar_dissector_buffers_initialize(netstar_dissector_t *dissector) {
  size_t buffers;

  for (buffers = 0; buffers < NETSTAR_DISSECTOR_BUFFERS; buffers++) {
    if (!dissector->buffers[buffers] && !(dissector->buffers[buffers] = buffer_make(0, 64)))
      return -1;

    buffer_truncate(dissector->buffers[buffers], 0);
  }

  return 1;
}

static void
netstar_dissector_buffers_deinitialize(netstar_dissector_t *dissector) {
  size_t buffers;

  for (buffers = 0; buffers < NETSTAR_DISSECTOR_BUFFERS; buffers++)
    if (dissector->buffers[buffers])
      buffer_free(dissector->buffers[buffers]), dissector->buffers[buffers] = NULL;
}

static inline netstar_dissector_layer_t
netstar_dissector_next_layer(netstar_dissector_layer_t layer) {
  return ((layer < NETSTAR_DISSECTOR_LAYER4) ? layer << 1 : NETSTAR_DISSECTOR_LAYER4);
}

static inline netstar_dissector_service_t
netstar_dissector_protocol_service(netstar_dissector_protocol_t type, struct netstar_capture_packet *packet) {
  if (type == NETSTAR_DISSECTOR_ETHERNET_PROTOCOL)
    return packet->layer2.protocol;

  if (type == NETSTAR_DISSECTOR_IP_PROTOCOL && packet->layer & NETSTAR_FORWARD_LAYER3)
    return packet->layer3.protocol;

  if (type == NETSTAR_DISSECTOR_TCP_PROTOCOL && packet->layer & NETSTAR_FORWARD_LAYER4)
    return packet->layer4.protocol->port;

  if (type == NETSTAR_DISSECTOR_UDP_PROTOCOL && packet->layer & NETSTAR_FORWARD_LAYER4)
    return packet->layer4.protocol->port;

  return 0x0000;
}

void
netstar_dissector_dissect(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  netstar_dissector_buffers_initialize(dissector); 

  if (!(dissector->layers & NETSTAR_DISSECTOR_LAYER2)) {
    netstar_dissector(dissector, packet, NETSTAR_DISSECTOR_ETHERNET_PROTOCOL, packet->layer2.protocol);
    return;
  }

  netstar_dissector_root->routine(dissector, packet);
}

void
netstar_dissector(netstar_dissector_t *dissector, struct netstar_capture_packet *packet, netstar_dissector_protocol_t type, netstar_dissector_service_t service) {
  struct netstar_dissector_protocol **dissectors = NULL;

  for (dissectors = netstar_dissectors; *dissectors; dissectors++) {
/*
    if (!(dissector->layers & (*dissectors)->layer) && type < NETSTAR_DISSECTOR_UDP_PROTOCOL) {
      netstar_dissector(dissector, packet, type+1, netstar_dissector_protocol_service(type+1, packet));
      break;
    }
*/
    if ((*dissectors)->type == type && (*dissectors)->service == service) {
      (*dissectors)->routine(dissector, packet);
      break;
    }
  }
}

void
netstar_dissector_format(netstar_dissector_t *dissector, netstar_dissector_slice_t slice, const char *restrict format, ...) {
  va_list args;

  va_start(args, format);
  netstar_utils_string_vformat(dissector->buffers[slice], format, args);
  va_end(args);
}

void
netstar_dissector_write(netstar_dissector_t *dissector, netstar_dissector_slice_t slice, const char *string, size_t length) {
  buffer_write(dissector->buffers[slice], string, length);
}

void
netstar_dissector_append(netstar_dissector_t *dissector, netstar_dissector_slice_t slice, const char *string, size_t length) {
  buffer_append(dissector->buffers[slice], string, length);
}

void
netstar_dissectors_register(struct netstar_dissector_protocol *protocol) {
  if (netstar_dissectors_registered >= NETSTAR_DISSECTORS)
    return;

  netstar_dissectors[netstar_dissectors_registered++] = protocol;
}

int
netstar_dissector_initialize(netstar_dissector_t *dissector) {
  if (netstar_dissector_buffers_initialize(dissector) == -1) {
    netstar_dissector_deinitialize(dissector);
    return -1;
  }

  dissector->layers = NETSTAR_DISSECTOR_LAYERS;

  return 1;
}

void
netstar_dissector_deinitialize(netstar_dissector_t *dissector) {
  if (dissector) {
    netstar_dissector_buffers_deinitialize(dissector);
  }
}


/*
static inline uint8_t *
netstar_monitor_packet_payload(struct netstar_capture_packet *packet, size_t *length) {
  if (packet->layer4.payload && packet->layer4.payload_length) {
    *length = packet->layer4.payload_length;
    return packet->layer4.payload;
  }

  if (packet->layer3.payload && packet->layer3.payload_length) {
    *length = packet->layer3.payload_length;
    return packet->layer3.payload;
  }

  if (packet->layer2.payload && packet->layer2.payload_length) {
    *length = packet->layer2.payload_length;
    return packet->layer2.payload;
  }

  return NULL;
}

static void
netstar_monitor_payload(struct netstar_capture_packet *packet, netstar_monitor_payload_format_t payload_format) {
  static const char *formats[] = { "%02" PRIX8, "%02" PRIo8, "%02" PRId8, "%c" };
  size_t payload_length = 0, byte;
  uint8_t *payload = NULL;

  netstar_monitor_write("\r\n");

  for (payload = netstar_monitor_packet_payload(packet, &payload_length), byte = 0; byte < payload_length; byte++) {
    if (payload_format == NETSTAR_MONITOR_PAYLOAD_DATA_ASCII) {
      char ascii = (isgraph((char)payload[byte]) && !isspace((char)payload[byte]) ? payload[byte] : '.');
      netstar_monitor_write(formats[payload_format], ascii);
    } else
      netstar_monitor_write(formats[payload_format], payload[byte]);

    if (byte+1 < payload_length)
      netstar_monitor_write(" ");
  }
}
*/
