#ifndef _SYSTEM_SYSTEM_H
#define _SYSTEM_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "system-target-architectures.h"
#include "system-target-names.h"

#include "system-errors.h"


extern const char *system_architecture(void);
extern const char *system_bitness(void);

extern const char *system_name(void);

#ifdef __cplusplus
}
#endif

#endif
