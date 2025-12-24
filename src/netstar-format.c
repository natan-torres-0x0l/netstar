#include "netstar-format.h"

#include <netstar-threads.h>

#include <strings/strings.h>

#include <stdarg.h>
#include <stdio.h>


void
netstar_vfprintf(FILE *stream, const char *restrict fmt, va_list args) {
  vfprintf(stream, fmt, args);
  fflush(stream);
}

void
netstar_fprintf(FILE *stream, const char *restrict fmt, ...) {
  va_list args;

  va_start(args, fmt);

  netstar_vfprintf(stream, fmt, args);

  va_end(args);
}

void
netstar_printf(const char *restrict fmt, ...) {
  va_list args;

  va_start(args, fmt);

  netstar_vfprintf(stdout, fmt, args);

  va_end(args);
}

void
netstar_print(const char *restrict string) {
  netstar_printf("%s", string);
}

void
netstar_println(const char *restrict string) {
  netstar_printf("%s\r\n", string);
}
