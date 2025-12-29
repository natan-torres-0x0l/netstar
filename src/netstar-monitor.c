#include "netstar-monitor.h"

#include <netstar-services.h>
#include <netstar-threads.h>

#include <netstar-capture.h>

#include <netstar-utils.h>

#include <buffers/buffers.h>
#include <strings/strings.h>


static bool netstar_monitor_initialized, netstar_monitor_started;

static buffer_t *netstar_monitor_buffer;


#define netstar_monitor_value(value) (const char *)buffer_ptr(monitor->dissector.buffers[value])

static void
netstar_monitor_write(const char *string) {
  if (*string && buffer_length(netstar_monitor_buffer))
    buffer_append(netstar_monitor_buffer, "  ", 2);

  buffer_append(netstar_monitor_buffer, string, string_length(string));
}

static void
netstar_monitor_format(const char *format, ...) {
  va_list args;

  if (buffer_length(netstar_monitor_buffer))
    buffer_append(netstar_monitor_buffer, "  ", 2);

  va_start(args, format);
  netstar_utils_string_vformat(netstar_monitor_buffer, format, args);
  va_end(args);
}

static void
netstar_monitor_dissect(struct netstar_monitor *monitor, struct netstar_capture_packet *packet) {
  netstar_dissector_dissect(&monitor->dissector, packet);
  buffer_truncate(netstar_monitor_buffer, 0);

  if (monitor->dissector.layers & NETSTAR_DISSECTOR_LAYER2) {
    netstar_monitor_format("%s - %s", netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_SOURCE_HARDWARE),
      netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_DESTINATION_HARDWARE));
  }

  if (packet->layer & NETSTAR_FORWARD_LAYER3 && monitor->dissector.layers & NETSTAR_DISSECTOR_LAYER3) {
    netstar_monitor_write(netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_ETHERNET_PROTOCOL));

    netstar_monitor_format("%s - %s", netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_SOURCE_ADDRESS),
      netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_DESTINATION_ADDRESS));

    netstar_monitor_write(netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_IP_PROTOCOL));
  }

  if (packet->layer & NETSTAR_FORWARD_LAYER4 && monitor->dissector.layers & NETSTAR_DISSECTOR_LAYER4) {
    netstar_monitor_format("%s - %s", netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_SOURCE_PORT),
      netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_DESTINATION_PORT));

    netstar_monitor_write(netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_APPLICATION_PROTOCOL));
  }

  netstar_monitor_write(netstar_monitor_value(NETSTAR_DISSECTOR_PACKET_INFORMATION));

  buffer_append(netstar_monitor_buffer, "\r\n", 3);
}

static void
netstar_monitor(netstar_t *netstar, struct netstar_capture_packet *packet, void *args) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)args;

  if (netstar_filter_initialized(&monitor->filter) && !netstar_filter_compile(&monitor->filter, packet))
    return;

  netstar_monitor_dissect(monitor, packet);

  netstar_log("\b \b[ monitor ] %s", (char *)buffer_ptr(netstar_monitor_buffer));
}

static void
netstar_monitor_initialize(netstar_t *netstar, void *context) {
  if (netstar_monitor_initialized)
    return;

  if (!(netstar_monitor_buffer = buffer_new()))
    return;

  netstar_monitor_initialized = true;
}

static void
netstar_monitor_deinitialize(void *context) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)context;

  if (!netstar_monitor_initialized)
    return;

  buffer_free(netstar_monitor_buffer);
  netstar_monitor_buffer = NULL;

  netstar_monitor_free(monitor);

  netstar_monitor_initialized = false;
}

static void
netstar_monitor_start(void *context) {
  struct netstar_monitor *monitor = (struct netstar_monitor *)context;

  if (netstar_monitor_started)
    return;

  netstar_forward_add(netstar_monitor, NETSTAR_FORWARD_ANY, monitor);

  netstar_monitor_started = true;
}

static void
netstar_monitor_stop(void *context) {
  if (!netstar_monitor_started)
    return;

  netstar_forward_remove(netstar_monitor);

  netstar_monitor_started = false;
}

int
netstar_monitor_new(struct netstar_monitor *monitor, netstar_t *netstar) {
  if (netstar_dissector_initialize(&monitor->dissector) == -1)
    return -1;
 
  monitor->mode = NETSTAR_MONITOR_MODE_NORMAL;
  monitor->netstar = netstar;

  return 1;
}

void
netstar_monitor_free(struct netstar_monitor *monitor) {
  if (monitor) {
    netstar_dissector_deinitialize(&monitor->dissector);

    netstar_filter_free(&monitor->filter);
  }
}

static struct netstar_monitor netstar_monitor_context;

static struct netstar_service netstar_monitor_context_service = {
  netstar_monitor_initialize, netstar_monitor_deinitialize,
  netstar_monitor_start, netstar_monitor_stop,
  "netstar:monitor", &netstar_monitor_context,
};

struct netstar_service *
netstar_monitor_service(void) {
  return &netstar_monitor_context_service;
}

__attribute__((__constructor__))
static inline void
netstar_monitor_register_service(void) {
  netstar_services_register(netstar_monitor_service());
}
