#ifndef _NETSTAR_MONITOR_H
#define _NETSTAR_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-dissectors/netstar-dissectors.h>
#include <netstar-forward.h>

#include <netstar-filter.h>
#include <netstar.h>


typedef enum {
  NETSTAR_MONITOR_MODE_LAYER2  = 0x02,
  NETSTAR_MONITOR_MODE_LAYER3  = 0x04,
  NETSTAR_MONITOR_MODE_LAYER4  = 0x08,
  NETSTAR_MONITOR_MODE_PAYLOAD = 0x10,
  NETSTAR_MONITOR_MODE_NORMAL  = (NETSTAR_MONITOR_MODE_LAYER2|NETSTAR_MONITOR_MODE_LAYER3|NETSTAR_MONITOR_MODE_LAYER4)
} netstar_monitor_mode_t;

typedef enum {
  NETSTAR_MONITOR_PAYLOAD_DATA_HEXADECIMAL,
  NETSTAR_MONITOR_PAYLOAD_DATA_OCTAL,
  NETSTAR_MONITOR_PAYLOAD_DATA_DECIMAL,
  NETSTAR_MONITOR_PAYLOAD_DATA_ASCII
} netstar_monitor_payload_format_t;

struct netstar_monitor {
  netstar_monitor_payload_format_t payload_format;
  netstar_monitor_mode_t mode;

  netstar_dissector_t dissector;
  netstar_filter_t filter;

  netstar_t *netstar;
};


extern int netstar_monitor_new(struct netstar_monitor *, netstar_t *);
extern void netstar_monitor_free(struct netstar_monitor *);

#ifdef __cplusplus
}
#endif

#endif
