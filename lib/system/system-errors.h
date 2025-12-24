#ifndef _SYSTEM_SYSTEM_ERRORS_H
#define _SYSTEM_SYSTEM_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
typedef /* DWORD */ uint32_t system_error_t;

#else // elif defined __unix__ || defined __unix
typedef int system_error_t;

#endif


extern const char *system_error_reason(system_error_t);

extern system_error_t system_geterror(void);
extern void system_seterror(system_error_t);

#ifdef __cplusplus
}
#endif

#endif
