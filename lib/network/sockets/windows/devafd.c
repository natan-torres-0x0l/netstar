#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winternl.h>

#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef SIO_BSP_HANDLE_POLL
#define SIO_BSP_HANDLE_POLL 0x4800001D
#endif

#ifndef SIO_BASE_HANDLE
#define SIO_BASE_HANDLE 0x48000022
#endif

#define IOCTL_AFD_POLL 0x00012024

#define AFD_POLL_RECEIVE                  0x0001
#define AFD_POLL_RECEIVE_EXPEDITED        0x0002
#define AFD_POLL_SEND                     0x0004
#define AFD_POLL_DISCONNECT               0x0008
#define AFD_POLL_ABORT                    0x0010
#define AFD_POLL_LOCAL_CLOSE              0x0020
#define AFD_POLL_CONNECT                  0x0040
#define AFD_POLL_ACCEPT                   0x0080
#define AFD_POLL_CONNECT_FAIL             0x0100
#define AFD_POLL_QOS                      0x0200
#define AFD_POLL_GROUP_QOS                0x0400
#define AFD_POLL_ROUTING_INTERFACE_CHANGE 0x0800
#define AFD_POLL_ADDRESS_LIST_CHANGE      0x1000

typedef struct _AFD_POLL_HANDLE_INFO {
  HANDLE Handle;
  ULONG Events;
  NTSTATUS Status;
} AFD_POLL_HANDLE_INFO;

typedef struct _AFD_POLL_INFO {
  LARGE_INTEGER Timeout;
  ULONG NumberOfHandles;
  ULONG Exclusive;
  AFD_POLL_HANDLE_INFO Handles[1];
} AFD_POLL_INFO;

typedef struct {
  HANDLE devafd_handle, iocp_handle;
  
} socket_devafd_reactor_t;


static inline SOCKET
devafd_base_socket_ioctl(SOCKET socket, DWORD ioctl) {
  SOCKET base_socket = INVALID_SOCKET;
  DWORD bytes = 0;

  if (WSAIoctl(socket, ioctl, NULL, 0, &base_socket, sizeof(SOCKET), &bytes, NULL, NULL) != SOCKET_ERROR)
    return base_socket;

  return INVALID_SOCKET;
}

static SOCKET
devafd_base_socket(SOCKET socket) {
  for (;;) {
    SOCKET base_socket = INVALID_SOCKET;

    if ((base_socket = devafd_base_socket_ioctl(socket, SIO_BASE_HANDLE)) != INVALID_SOCKET)
      return base_socket;

    base_socket = devafd_base_socket_ioctl(socket, SIO_BSP_HANDLE_POLL);

    if (base_socket != INVALID_SOCKET && base_socket != socket) {
      socket = base_socket;
      continue;
    }

    return INVALID_SOCKET;
  }
}

static inline SOCKET
devafd_socket(int socket_family, int socket_type, int socket_protocol) {
  return socket(socket_family, socket_type, socket_protocol);
}

int
main(void) {
// @ AFD poll
  HANDLE devafd_handle = INVALID_HANDLE_VALUE;
  IO_STATUS_BLOCK io_status_block = {0}, io_status_block_poll = {0};

  UNICODE_STRING devafd_name = {0};
  OBJECT_ATTRIBUTES devafd_attr = {0};

  NTSTATUS status;

  HANDLE iocp_handle = INVALID_HANDLE_VALUE;

// @ AFD socket test
  AFD_POLL_INFO poll_info[2] = {0};
  ULONG events = AFD_POLL_ACCEPT; // AFD_POLL_CONNECT|AFD_POLL_CONNECT_FAIL;

  struct sockaddr_in server_addr = {0};
  SOCKET socket = INVALID_SOCKET;

  ULONG fionbio;

  int reuseaddr;

// @ Windows Socket API
  WSADATA windows_socket_api;

  if (WSAStartup(MAKEWORD(0x02, 0x02), &windows_socket_api) < 0)
    goto _return;

  RtlInitUnicodeString(&devafd_name, L"\\Device\\Afd\\explore");

  InitializeObjectAttributes(
    &devafd_attr,
    &devafd_name,
    OBJ_CASE_INSENSITIVE,
    NULL,
    NULL
  );

  status = NtCreateFile(
    &devafd_handle,
    GENERIC_READ|GENERIC_WRITE|SYNCHRONIZE,
    &devafd_attr,
    &io_status_block,
    NULL,
    0,
    FILE_SHARE_READ|FILE_SHARE_WRITE,
    FILE_OPEN,
    0,
    NULL,
    0
  );
  if (status < 0)
    goto _return;

  printf(" NtCreateFile: %d\r\n", status);

  iocp_handle = CreateIoCompletionPort(
    INVALID_HANDLE_VALUE,
    NULL,
    0,
    0
  );
  if (iocp_handle == NULL)
    goto _return;

  printf(" CreateIoCompletionPort: %d\r\n", iocp_handle);

  if (CreateIoCompletionPort(devafd_handle, iocp_handle, 0, 0) == NULL)
    goto _return;

  if (!SetFileCompletionNotificationModes(devafd_handle, FILE_SKIP_SET_EVENT_ON_HANDLE))
    goto _return;

  if ((socket = devafd_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    goto _return;

  reuseaddr = 1;
  setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(reuseaddr));

  fionbio = 1;
  if (ioctlsocket(socket, FIONBIO, &fionbio) == -1)
    goto _return;

/*
  server_addr.sin_addr.s_addr = inet_addr("8.8.8.8");
  server_addr.sin_port = htons(54);
  server_addr.sin_family = AF_INET;

  printf(" connect: %d\r\n", connect(socket, (struct sockaddr *)&server_addr, sizeof(server_addr)));
*/

  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(12345);
  server_addr.sin_family = AF_INET;

  printf(" bind: %d\r\n", bind(socket, (struct sockaddr *)&server_addr, sizeof(server_addr)));
  printf(" listen: %d\r\n", listen(socket, 10));

// printf(" accept: %d\r\n", accept(socket, NULL, NULL));

  poll_info[0].Exclusive         = false;
  poll_info[0].NumberOfHandles   = 1;
  poll_info[0].Timeout.QuadPart  = INT64_MAX;
  poll_info[0].Handles[0].Handle = (HANDLE)devafd_base_socket(socket);
  poll_info[0].Handles[0].Status = 0;
  poll_info[0].Handles[0].Events = events;

  status = NtDeviceIoControlFile(
    devafd_handle,
    NULL,
    NULL,
    &io_status_block_poll,
    &io_status_block_poll,
    IOCTL_AFD_POLL,
    &poll_info[0],
    sizeof(AFD_POLL_INFO),
    &poll_info[1],
    sizeof(AFD_POLL_INFO)
  );

  printf(" NtDeviceIoControlFile: %d\r\n", status);

  if (status == STATUS_PENDING) {
    DWORD bytes = 0;
    ULONG_PTR completion_key = 0;
    OVERLAPPED *overlapped = NULL;

    IO_STATUS_BLOCK *io_status_block = NULL;

    bool status = false;

    puts(" waiting");
  
    status = GetQueuedCompletionStatus(iocp_handle, &bytes, &completion_key, &overlapped, 5000);
    printf(" GetQueuedCompletionStatus: %d\r\n", status);

    io_status_block = (IO_STATUS_BLOCK *)overlapped;

    printf(" %d\r\n", io_status_block == &io_status_block_poll);
 // printf(" connect: %x\r\n", poll_info[1].Handles[0].Events);
    printf(" accept: %x\r\n", poll_info[1].Handles[0].Events);
  }

_return:
  CloseHandle(devafd_handle);
  CloseHandle(iocp_handle);

  WSACleanup();

  return 0;
}
