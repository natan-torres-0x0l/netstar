#ifndef _NETWORK_SOCKETS_SOCKETS_WINDOWS_H
#define _NETWORK_SOCKETS_SOCKETS_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <netioapi.h>

#include <winternl.h>
#include <mswsock.h>

#include <basetsd.h>

#include <windows.h>

#define SOCKET_WINDOWS_API_VERSION_MINOR 0x02
#define SOCKET_WINDOWS_API_VERSION_MAJOR 0x02

// typedef ssize_t SSIZE_T; // #define ssize_t SSIZE_T // signed long long int
// typedef size_t SIZE_T; // #define size_t SIZE_T // unsigned long long int

#define SOCKET_INVALID INVALID_SOCKET

typedef SOCKET socket_t;


// extern int socket_windows_api_startup(void);
// extern int socket_windows_api_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif
