#include "netstar-colored.h"

#include <console/colored/colored.h>


#ifndef NETSTAR_COLORED
static char netstar_colored_zero[1];
#endif

const char *
netstar_color(const char *color) {
#ifdef NETSTAR_COLORED
  return colored_color(color);
#else
  return netstar_colored_zero;
#endif
}

const char *
netstar_style(const char *style) {
#ifdef NETSTAR_COLORED
  return colored_style(style);
#else
  return netstar_colored_zero;
#endif
}

const char *
netstar_reset(void) {
#ifdef NETSTAR_COLORED
  return colored_reset();
#else
  return netstar_colored_zero;
#endif
}
