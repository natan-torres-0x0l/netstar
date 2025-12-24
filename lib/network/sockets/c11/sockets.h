#ifndef _NETWORK_SOCKETS_SOCKETS_C11_H
#define _NETWORK_SOCKETS_SOCKETS_C11_H

#if defined __cplusplus
extern "C" {
#endif

#include "sockets.h"

#define generic _Generic


#if defined generic
#define socket_recvfrom(socket, args...) generic((socket), socketfd_t: socket_recvfrom_, socket_t *: socket_recvfrom)(socket, args)
#endif

#if defined generic
#define socket_sendto(socket, args...) generic((socket), socketfd_t: socket_sendto_, socket_t *: socket_sendto)(socket, args)
#endif

#if defined generic
#define socket_recv(socket, args...) generic((socket), socketfd_t: socket_recv_, socket_t *: socket_recv)(socket, args)
#endif

#if defined generic
#define socket_send(socket, args...) generic((socket), socketfd_t: socket_send_, socket_t *: socket_send)(socket, args)
#endif

#if defined generic
#define socket_setoption(socket, args...) generic((socket), socketfd_t: socket_setoption_, socket_t *: socket_setoption)(socket, args)
#endif

#if defined generic
#define socket_getoption(socket, args...) generic((socket), socketfd_t: socket_getoption_, socket_t *: socket_getoption)(socket, args)
#endif

#if defined generic
#define socket_setnlocking(socket, args...) generic((socket), socketfd_t: socket_setblocking_, socket_t *: socket_setblocking)(socket, args)
#endif

#if defined generic
#define socket_accept(socket, args...) generic((socket), socketfd_t: socket_accept_, socket_t *: socket_accept)(socket, args)
#endif

#if defined generic
#define socket_bind(socket, args...) generic((socket), socketfd_t: socket_bind_, socket_t *: socket_bind)(socket, args)
#endif

#if defined generic
#define socket_listen(socket, args...) generic((socket), socketfd_t: socket_listen_, socket_t *: socket_listen)(socket, args)
#endif

#if defined generic
#define socket_connect(socket, args...) generic((socket), socketfd_t: socket_connect_, socket_t *: socket_connect)(socket, args)
#endif

#if defined generic
#define socket_shutdown(socket, args...) generic((socket), socketfd_t: socket_shutdown_, socket_t *: socket_shutdown)(socket, args)
#endif

#if defined generic
#define socket_close(socket) generic((socket), socketfd_t: socket_close_, socket_t *: socket_close)(socket)
#endif

#if defined generic
#define socket_getname(socket, args...) generic((socket), socketfd_t: socket_getname_, socket_t *: socket_getname)(socket, args)
#endif

#if defined generic
#define socket_getpeer(socket, args...) generic((socket), socketfd_t: socket_getpeer_, socket_t *: socket_getpeer)(socket, args)
#endif

#if defined __cplusplus
}
#endif

#endif
