#ifndef _NETSTAR_SEMAPHORE_H
#define _NETSTAR_SEMAPHORE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
#include <windows.h>

typedef HANDLE netstar_semaphore_t;
#else
#define _POSIX_C_SOURCE 200809L
#include <semaphore.h>

typedef sem_t netstar_semaphore_t;
#endif

#include <netstar-time.h>


extern void netstar_semaphore_timedwait(netstar_semaphore_t *, netstar_time_t);

extern void netstar_semaphore_post(netstar_semaphore_t *);

extern void netstar_semaphore_initialize(netstar_semaphore_t *, uint32_t);
extern void netstar_semaphore_destroy(netstar_semaphore_t *);

#ifdef __cplusplus
}
#endif

#endif
