#ifndef _NETSTAR_FORMAT_H
#define _NETSTAR_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
  #define restrict __restrict
#endif


extern void netstar_vfprintf(FILE *, const char *restrict, va_list);

extern void netstar_fprintf(FILE *, const char *restrict, ...);
extern void netstar_printf(const char *restrict, ...);

extern void netstar_print(const char *restrict);
extern void netstar_println(const char *restrict);

#ifdef __cplusplus
}
#endif

#endif
