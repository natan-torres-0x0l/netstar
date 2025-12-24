#include "netstar-terminate.h"

#include <netstar-services.h>

#include <netstar-forward.h>
#include <netstar-capture.h>

#include <netstar.h>

#include <stdlib.h>


static inline void
netstar_terminate_processes(void) {
  netstar_services_deinitialize(/* netstar */);
  netstar_capture_stop(/* netstar */);

  netstar_forward_deinitialize(/* netstar */);
  netstar_threads_deinitialize(/* netstar */);
}

void
netstar_terminate(void) {
  netstar_terminate_processes();

  netstar_exit(NETSTAR_EXIT_SUCCESS);
}

void
netstar_exit(int status) {
  netstar_close(netstar_context());
  exit(status);
}

/*
static inline noreturn void
netstar_interrupt(int signal) {
  netstar_thread_mutex_print_lock();
  netstar_println("");
  netstar_thread_mutex_print_unlock();

  netstar_log("forced interruption by user action detected. Ctrl-C is not the recommended way to terminate this process\r\n");
  netstar.interrupted = true;

  netstar_terminate();
}
*/
