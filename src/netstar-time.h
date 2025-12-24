#ifndef _NETSTAR_TIME_H
#define _NETSTAR_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <time.h>


typedef uint32_t netstar_time_t;


static inline netstar_time_t
netstar_time_usecstomillisecs(netstar_time_t usecs) {
  return usecs / 1000;
}

static inline netstar_time_t
netstar_time_usecstosecs(netstar_time_t usecs) {
  return usecs / 1000000;
}

static inline netstar_time_t
netstar_time_usecstominutes(netstar_time_t usecs) {
  return usecs / 60000000;
}

static inline netstar_time_t
netstar_time_millisecstousecs(netstar_time_t millisecs) {
  return millisecs * 1000;
}

static inline netstar_time_t
netstar_time_millisecstosecs(netstar_time_t millisecs) {
  return millisecs / 1000;
}

static inline netstar_time_t
netstar_time_millisecstominutes(netstar_time_t millisecs) {
  return millisecs / 60000;
}

static inline netstar_time_t
netstar_time_secstomicrosecs(netstar_time_t secs) {
  return secs * 1000000;
}

static inline netstar_time_t
netstar_time_secstomillisecs(netstar_time_t secs) {
  return secs * 1000;
}

static inline netstar_time_t
netstar_time_secstominutes(netstar_time_t secs) {
  return secs / 60;
}

static inline netstar_time_t
netstar_time_minutestousecs(netstar_time_t minutes) {
  return minutes * 60000000;
}

static inline netstar_time_t
netstar_time_minutestomillisecs(netstar_time_t minutes) {
  return minutes * 60000;
}

static inline netstar_time_t
netstar_time_minutestosecs(netstar_time_t minutes) {
  return minutes * 60;
}


typedef struct {
  netstar_time_t secs;
  time_t time;
} netstar_timer_t;


#define NETSTAR_TIMER_INITIALIZER { 0, 0 }


extern netstar_time_t netstar_timer(netstar_timer_t *);
extern void netstar_timer_start(netstar_timer_t *, netstar_time_t);

extern void netstar_time_sleep(netstar_time_t);


#define NETSTAR_TIME_FORMAT_LENGTH sizeof("hh:mm:ss")


extern char *netstar_time_now(char *, size_t);

#ifdef __cplusplus
}
#endif

#endif
