#include "netstar-services.h"

#include <strings/strings.h>
#include <stddef.h>

#define NETSTAR_SERVICES 10


static struct netstar_service *netstar_services[NETSTAR_SERVICES+1];
static size_t netstar_services_registered;

static struct netstar_service netstar_service_unknown;


void
netstar_service_start(struct netstar_service *service) {
  service->flags |= NETSTAR_SERVICE_FLAGS_INITIALIZE|NETSTAR_SERVICE_FLAGS_START;
}

struct netstar_service *
netstar_service(const char *service_name) {
  struct netstar_service **services = NULL;

  for (services = netstar_services; *services; services++)
    if (string_equals((*services)->name, service_name, true))
      return *services;

  return &netstar_service_unknown;
}

void
netstar_services_register(struct netstar_service *service) {
  if (netstar_services_registered >= NETSTAR_SERVICES)
    return;

// netstar_log("#netstar-services %s registered\r\n", service->name);

  netstar_services[netstar_services_registered++] = service;
}

void
netstar_services_initialize(netstar_t *netstar) {
  struct netstar_service **services = NULL;

  for (services = netstar_services; *services; services++)
    if (!(*services)->initialized && (*services)->initialize && ((*services)->flags & NETSTAR_SERVICE_FLAGS_INITIALIZE)) {
      netstar_log("\b \b[ services ] initializing service %s...\r\n", (*services)->name);
      (*services)->initialize(netstar, (*services)->context);     
      (*services)->initialized = true;
    }

  for (services = netstar_services; *services; services++)
    if (!(*services)->started && (*services)->start && ((*services)->flags & NETSTAR_SERVICE_FLAGS_START)) {
      netstar_log("\b \b[ services ] starting service %s...\r\n", (*services)->name);
      (*services)->start((*services)->context);
      (*services)->started = true;
    }
}

void
netstar_services_deinitialize(void) {
  struct netstar_service **services = NULL;

  for (services = netstar_services; *services; services++)
    if ((*services)->started && (*services)->stop && ((*services)->flags & NETSTAR_SERVICE_FLAGS_START)) {
      netstar_log("\b \b[ services ] stopping service %s...\r\n", (*services)->name);
      (*services)->stop((*services)->context);
    }

  for (services = netstar_services; *services; services++)
    if ((*services)->initialized && (*services)->deinitialize && ((*services)->flags & NETSTAR_SERVICE_FLAGS_INITIALIZE)) {
      netstar_log("\b \b[ services ] deinitializing service %s...\r\n", (*services)->name);
      (*services)->deinitialize((*services)->context);
    }
}
