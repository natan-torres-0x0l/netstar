#ifndef _NETSTAR_LOG_H
#define _NETSTAR_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
  #define restrict __restrict
#endif

extern void netstar_critical(const char *restrict fmt, ...);
extern void netstar_error(const char *restrict fmt, ...);
extern void netstar_warning(const char *restrict fmt, ...);
extern void netstar_log(const char *restrict fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
