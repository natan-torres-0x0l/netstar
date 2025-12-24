#include "netstar-semaphore.h"

#include <sys/time.h>
#include <time.h>


#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
void
netstar_semaphore_timedwait(netstar_semaphore_t *semaphore, netstar_time_t secs) {
  WaitForSingleObject(*semaphore, secs*1000);
}

void
netstar_semaphore_post(netstar_semaphore_t *semaphore) {
  ReleaseSemaphore(*semaphore, 1, NULL);
}

void
netstar_semaphore_initialize(netstar_semaphore_t *semaphore, uint32_t value) {
  *semaphore = CreateSemaphoreA(NULL, (LONG)value, LONG_MAX, NULL);
}

void
netstar_semaphore_destroy(netstar_semaphore_t *semaphore) {
  CloseHandle(*semaphore);
}

#else
void
netstar_semaphore_timedwait(netstar_semaphore_t *semaphore, netstar_time_t secs) {
  struct timespec timespec = {0};
  struct timeval timeval = {0};

  gettimeofday(&timeval, NULL);
// clock_gettime(CLOCK_REALTIME, &timespec);
  timespec.tv_sec += timeval.tv_sec+secs;
  timespec.tv_nsec += timeval.tv_usec*1000;

  sem_timedwait(semaphore, &timespec);
}

void
netstar_semaphore_post(netstar_semaphore_t *semaphore) {
  sem_post(semaphore);
}

void
netstar_semaphore_initialize(netstar_semaphore_t *semaphore, uint32_t value) {
  sem_init(semaphore, 0, value);
}

void
netstar_semaphore_destroy(netstar_semaphore_t *semaphore) {
  sem_destroy(semaphore);
}

#endif
