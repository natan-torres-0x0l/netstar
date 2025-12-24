#if defined __linux__ || defined __linux
  #define _GNU_SOURCE // Using GNU source implementation for nanosleep
#endif

#include "netstar-time.h"

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include <windows.h>

#endif

#include <sys/time.h>
#include <time.h>

#include <stdbool.h>
#include <stdint.h>


#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
void
netstar_time_sleep(netstar_time_t millisecs) {
  Sleep((DWORD)millisecs);
// SleepEx(millisecs, false);
}

#else // elif defined __unix__ || defined __unix
void
netstar_time_sleep(netstar_time_t millisecs) {
  struct timespec request = { (time_t)millisecs/1000 }, remaining;
  nanosleep(&request, &remaining);
}

#endif

void
netstar_timer_start(netstar_timer_t *timer, netstar_time_t secs) {
  timer->time = time(NULL);
  timer->secs = secs;
}

netstar_time_t
netstar_timer(netstar_timer_t *timer) {
  netstar_time_t elapsed;

  if ((elapsed = (netstar_time_t)difftime(time(NULL), timer->time)) >= timer->secs)
    return 0;

  return timer->secs-elapsed;
}

char *
netstar_time_now(char *buffer, size_t length) {
  time_t timenow = time(NULL);
  struct tm *tm = NULL;

  tm = localtime(&timenow);
  strftime(buffer, length, "%H:%M:%S", tm);

  return buffer;
}
