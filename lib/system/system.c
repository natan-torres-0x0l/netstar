#include "system.h"


const char *
system_architecture(void) {
  return SYSTEM_TARGET_ARCHITECTURE;
}

/*
const char *
system_bitness(void) {
  return SYSTEM_TARGET_ARCHITECTURE_BITNESS;
}
*/

const char *
system_name(void) {
  return SYSTEM_TARGET_NAME;
}
