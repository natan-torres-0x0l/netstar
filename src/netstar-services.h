#ifndef _NETSTAR_SERVICES_H
#define _NETSTAR_SERVICES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar.h>

#include <stdbool.h>

typedef void * netstar_service_context_t;

typedef void (*netstar_service_initialize_t)(netstar_t *, netstar_service_context_t);
typedef void (*netstar_service_deinitialize_t)(netstar_service_context_t);

typedef void (*netstar_service_command_t)(netstar_service_context_t);

typedef enum {
  NETSTAR_SERVICE_FLAGS_INITIALIZE = 0x01, NETSTAR_SERVICE_FLAGS_START = 0x02
} netstar_service_flags_t;

struct netstar_service {
  netstar_service_initialize_t initialize;
  netstar_service_deinitialize_t deinitialize;

	netstar_service_command_t start, stop;

  const char *name;
  netstar_service_context_t context;

  netstar_service_flags_t flags;

  bool initialized, started;
};


extern struct netstar_service *netstar_service(const char *);
extern void netstar_service_start(struct netstar_service *);

extern void netstar_services_register(struct netstar_service *);

extern void netstar_services_initialize(netstar_t *);
extern void netstar_services_deinitialize(void);

#ifdef __cplusplus
}
#endif

#endif
