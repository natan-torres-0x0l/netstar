#include "netstar-log.h"

#include <netstar-terminate.h>
#include <netstar-threads.h>

#include <netstar-colored.h>
#include <netstar-format.h>

#include <netstar-time.h>
#include <netstar.h>


static netstar_thread_mutex_t netstar_log_mutex;


void
netstar_critical(const char *restrict format, ...) {
  va_list args;
  char time[NETSTAR_TIME_FORMAT_LENGTH] = {0};

  va_start(args, format);

  netstar_thread_mutex_lock(&netstar_log_mutex);

  netstar_fprintf(stderr, "%s%s[ critical ]%s%s%s[ %s ] ",
    netstar_style("bold"), netstar_color("@light-red"), netstar_reset(),
    netstar_style("bold"), netstar_color("#light-red"), netstar_time_now(time, sizeof(time)));

  netstar_vfprintf(stderr, format, args);

  netstar_fprintf(stderr, "%s", netstar_reset());

  netstar_thread_mutex_unlock(&netstar_log_mutex);

  va_end(args);

  netstar_terminate();
}

void
netstar_error(const char *restrict format, ...) {
  va_list args;
  char time[NETSTAR_TIME_FORMAT_LENGTH] = {0};

  va_start(args, format);

  netstar_thread_mutex_lock(&netstar_log_mutex);

  netstar_fprintf(stderr, "%s%s[ error ]%s[ %s ] ",
    netstar_style("bold"), netstar_color("#red"), netstar_reset(), netstar_time_now(time, sizeof(time)));

  netstar_vfprintf(stderr, format, args);

  netstar_fprintf(stderr, "%s", netstar_reset());

  netstar_thread_mutex_unlock(&netstar_log_mutex);

  va_end(args);

  netstar_terminate();
}

void
netstar_warning(const char *restrict format, ...) {
  va_list args;
  char time[NETSTAR_TIME_FORMAT_LENGTH] = {0};

  va_start(args, format);

  netstar_thread_mutex_lock(&netstar_log_mutex);

  netstar_fprintf(stdout, "%s%s[ warning ][ %s ] ",
    netstar_color("#light-yellow"), netstar_style("bold"), netstar_time_now(time, sizeof(time)));

  netstar_vfprintf(stdout, format, args);
  netstar_fprintf(stdout, "%s", netstar_reset());

  netstar_thread_mutex_unlock(&netstar_log_mutex);

  va_end(args);
}

void
netstar_log(const char *restrict format, ...) {
  va_list args;
  char time[NETSTAR_TIME_FORMAT_LENGTH] = {0};

  va_start(args, format);

  netstar_thread_mutex_lock(&netstar_log_mutex);

  netstar_fprintf(stdout, "[ %s%s%s ] ",
    netstar_color("#magenta"), netstar_time_now(time, sizeof(time)), netstar_reset());

  netstar_vfprintf(stdout, format, args);

  netstar_thread_mutex_unlock(&netstar_log_mutex);

  va_end(args);
}
