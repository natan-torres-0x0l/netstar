#ifndef _NETWORK_SOCKETS_SOCKETS_UNIX_H
#define _NETWORK_SOCKETS_SOCKETS_UNIX_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined __APPLE__ && defined __MACH__) || (defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD)
  #include <sys/event.h>
#endif

#include <sys/select.h>

#include <sys/socket.h>
#include <sys/types.h>

#if defined __linux__ || defined __linux
  #include <sys/epoll.h>
#endif

#include <poll.h>

#include <netinet/tcp.h>
#include <netinet/in.h>

#include <arpa/inet.h>
// #include <netdb.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

// typedef signed long long int ssize_t; // #define ssize_t signed long long int
// typedef unsigned long long int size_t; //  #define size_t  unsigned long long int

#define SOCKET_INVALID -1

typedef int socket_t;

#ifdef __cplusplus
}
#endif

#endif

