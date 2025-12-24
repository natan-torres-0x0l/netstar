#include "sockets-events.h"
#include "sockets.h"

#include <stdbool.h>


const struct socket_timeout socket_nonblocking = { .sec = 0, .usec = 0 };


int
socket_select(int select_descriptor, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct socket_timeout *socket_timeout) {
  struct timeval timeval = {0}, *timevalptr = NULL;

  if (socket_timeout) {
    timeval.tv_sec  = socket_timeout->sec;
    timeval.tv_usec = socket_timeout->usec;

    timevalptr = &timeval;
  }

  return select(select_descriptor, readset, writeset, exceptset, timevalptr);
}

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
int
socket_poll(struct pollfd *pollfds, size_t pollfds_count, const struct socket_timeout *socket_timeout) {
  int timeout = (int)((socket_timeout) ? (socket_timeout->sec*1000)+(socket_timeout->usec/1000) : -1);
  return WSAPoll(pollfds, (ULONG)pollfds_count, timeout);
}

#else // elif defined __unix__ || defined __unix
int
socket_poll(struct pollfd *pollfds, size_t pollfds_count, const struct socket_timeout *socket_timeout) {
  int timeout = (int)((socket_timeout) ? (socket_timeout->sec*1000)+(socket_timeout->usec/1000) : -1);
  return poll(pollfds, (nfds_t)pollfds_count, timeout);
}

#endif

#if defined SOCKET_EVENTS_WAIT_SERVICE_POLL
int
socket_wait(socket_t socket, socket_event_t socket_events, const struct socket_timeout *socket_timeout) {
  struct pollfd pollfd = {0};

  if (socket_events & SOCKET_READ)
    pollfd.events |= POLLIN;

  if (socket_events & SOCKET_WRITE)
    pollfd.events |= POLLOUT;

  if (socket_events & SOCKET_EXCEPT)
    pollfd.events |= POLLERR|POLLHUP;

  pollfd.fd = socket;

  return socket_poll(&pollfd, 1, socket_timeout);
}

#else // defined SOCKET_EVENTS_WAIT_SERVICE_SELECT
int
socket_wait(socket_t socket, socket_event_t socket_events, const struct socket_timeout *socket_timeout) {
  fd_set readset, *readsetptr = NULL, writeset, *writesetptr = NULL, exceptset, *exceptsetptr = NULL;

  if (socket_events & SOCKET_READ) {
    FD_ZERO((readsetptr = &readset));
    FD_SET(socket, readsetptr);
  }

  if (socket_events & SOCKET_WRITE) {
    FD_ZERO((writesetptr = &writeset));
    FD_SET(socket, writesetptr);
  }

  if (socket_events & SOCKET_EXCEPT) {
    FD_ZERO((exceptsetptr = &exceptset));
    FD_SET(socket, exceptsetptr);
  }

  return socket_select((int)socket+1, readsetptr, writesetptr, exceptsetptr, socket_timeout);
}

#endif
