#include "sockets-afd-reactor.h"
#include "sockets-handler.h"
#include "sockets-errors.h"

#include <collections/rbtree/rbtree.h>

#include <stdlib.h>

typedef NTSTATUS (NTAPI *NtCancelIoFileEx_t)(HANDLE, PIO_STATUS_BLOCK, PIO_STATUS_BLOCK);

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


struct socket_afd_event {
  IO_STATUS_BLOCK io_status_block;
  AFD_POLL_INFO pollinfos[2];

  socket_t socket;
};

typedef struct socket_afd_reactor {
  socket_reactor_interrupter_t interrupter;

  OVERLAPPED_ENTRY overlapped_entries[256];

  IO_STATUS_BLOCK io_status_block;
  HANDLE afd_handle, iocp_handle;

  rbtree_t *events;
} socket_afd_reactor_t;


static inline int
socket_afd_reactor_events_compare(const void *value1, const void *value2) {
  struct socket_afd_event *event1 = (struct socket_afd_event *)value1;
  struct socket_afd_event *event2 = (struct socket_afd_event *)value2;

  if (event1->socket < event2->socket)
    return -1;
  if (event1->socket > event2->socket)
    return 1;

  return 0;
}


static NtCancelIoFileEx_t NtCancelIoFileEx;


__attribute__((__constructor__))
static inline void
socket_afd_reactor_cancel_io_file_load(void) {
  HMODULE ntdll = NULL;

  if (!(ntdll = LoadLibraryA("ntdll.dll")))
    return;

  NtCancelIoFileEx = (NtCancelIoFileEx_t)GetProcAddress(ntdll, "NtCancelIoFileEx");
}

static inline void
socket_afd_reactor_cancel_io_file(struct socket_afd_event *event) {
  IO_STATUS_BLOCK cancel_io_status_block = {0};

  NtCancelIoFileEx(event->pollinfos[0].Handles[0].Handle, &event->io_status_block, &cancel_io_status_block);
}

static inline void
socket_afd_reactor_events_free(void *value) {
  struct socket_afd_event *event = (struct socket_afd_event *)value;

  socket_afd_reactor_cancel_io_file(event);

  free(event);
}

static inline int
socket_afd_reactor_events_io_status_block_find(const void *value, va_list args) {
  struct socket_afd_event *event = (struct socket_afd_event *)value;
  socket_t *socket = va_arg(args, socket_t *);

  if (event->socket < *socket)
    return -1;
  if (event->socket > *socket)
    return 1;

  return 0;
}

static int
socket_afd_reactor_events(socket_afd_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count) {
  struct socket_afd_event *event = NULL;
  rbtree_iterator_t events_iter = NULL;

  int activated = 0, entry;

  memset(socket_events, 0, sizeof(struct socket_event)*(size_t)socket_events_count);

  for (entry = 0; entry < socket_events_count; entry++) {
    event = (struct socket_afd_event *)CONTAINING_RECORD((IO_STATUS_BLOCK *)reactor->overlapped_entries[entry].lpOverlapped, struct socket_afd_event, io_status_block);

    if (event->socket == reactor->interrupter[0])
      break;

    if ((event->pollinfos[1].Handles[0].Events & AFD_POLL_ACCEPT) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_RECEIVE) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_RECEIVE_EXPEDITED))
      socket_events[activated].events |= SOCKET_READ;

    if ((event->pollinfos[1].Handles[0].Events & AFD_POLL_SEND) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_CONNECT))
      socket_events[activated].events |= SOCKET_WRITE;

    if ((event->pollinfos[1].Handles[0].Events & AFD_POLL_DISCONNECT) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_ABORT) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_LOCAL_CLOSE) || (event->pollinfos[1].Handles[0].Events & AFD_POLL_CONNECT_FAIL))
      socket_events[activated].events |= SOCKET_EXCEPT;

    if (socket_events[activated].events) {
      socket_afd_reactor_add(reactor, event->socket, socket_events[activated].events);

      socket_events[activated].socket = event->socket;
      activated++;
    }
  }

  return activated;
}

int
socket_afd_reactor_run(socket_afd_reactor_t *reactor, struct socket_event *socket_events, int socket_events_count, const struct socket_timeout *timeout) {
  DWORD timeval = (DWORD)((timeout) ? (timeout->sec*1000)+(timeout->usec) : INFINITE);
  bool status;

  ULONG activated = 0;

  status = GetQueuedCompletionStatusEx(reactor->iocp_handle, reactor->overlapped_entries, 256, &activated, timeval, false);
  if (!status && system_geterror() == WAIT_TIMEOUT) {
    socket_seterror(SOCKET_ETIMEOUT);
    return 0;
  }
  if (!status) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_events_count = ((activated > socket_events_count) ? socket_events_count : activated);

  return socket_afd_reactor_events(reactor, socket_events, socket_events_count);
}

void
socket_afd_reactor_cancel(socket_afd_reactor_t *reactor) {
  socket_reactor_interrupter_interrupt(reactor->interrupter);
}

static inline HANDLE
socket_afd_reactor_base_socket_ioctl(socket_t socket, DWORD ioctl) {
  HANDLE base_socket = INVALID_HANDLE_VALUE;
  DWORD bytes = 0;

  if (WSAIoctl(socket, ioctl, NULL, 0, &base_socket, sizeof(base_socket), &bytes, NULL, NULL) != SOCKET_ERROR)
    return base_socket;

  return INVALID_HANDLE_VALUE;
}

static HANDLE
socket_afd_reactor_base_socket(socket_t socket) {
  for (;;) {
    HANDLE base_socket = INVALID_HANDLE_VALUE;

    if ((base_socket = socket_afd_reactor_base_socket_ioctl(socket, SIO_BASE_HANDLE)) != INVALID_HANDLE_VALUE)
      return base_socket;

    base_socket = socket_afd_reactor_base_socket_ioctl(socket, SIO_BSP_HANDLE_POLL);

    if (base_socket != INVALID_HANDLE_VALUE && (socket_t)base_socket != socket) {
      socket = (socket_t)base_socket;
      continue;
    }

    break;
  }

  return INVALID_HANDLE_VALUE;
}

static int
socket_afd_reactor_events_socket_find(void *value, va_list args) {
  struct socket_afd_event *event = (struct socket_afd_event *)value;
  socket_t *socket = va_arg(args, socket_t *);

  if (event->socket < *socket)
    return -1;
  if (event->socket > *socket)
    return 1;

  return 0;
}

static inline struct socket_afd_event *
socket_afd_reactor_events_find(socket_afd_reactor_t *reactor, socket_t socket) {
  return (struct socket_afd_event *)rbtree_findif(reactor->events, socket_afd_reactor_events_socket_find, &socket);
}

static inline struct socket_afd_event *
socket_afd_reactor_events_event(socket_afd_reactor_t *reactor, socket_t socket) {
  struct socket_afd_event *event = NULL;

  if ((event = socket_afd_reactor_events_find(reactor, socket))) {
    socket_afd_reactor_cancel_io_file(event);
    return event;
  }

  if (!(event = (struct socket_afd_event *)calloc(1, sizeof(struct socket_afd_event)))) {
    socket_seterror(SOCKET_ESYSCALL);
    return NULL;
  }

  return event;
}

static inline int
socket_afd_reactor_events_add(socket_afd_reactor_t *reactor, struct socket_afd_event *event) {
  return rbtree_insert(reactor->events, rbtree_lvalue(event, sizeof(struct socket_afd_event *)));
}

static inline NTSTATUS
socket_afd_reactor_device_io_control_file(socket_afd_reactor_t *reactor, struct socket_afd_event *event) {
  return NtDeviceIoControlFile(reactor->afd_handle, NULL, NULL, &event->io_status_block, &event->io_status_block, IOCTL_AFD_POLL, &event->pollinfos[0], sizeof(AFD_POLL_INFO), &event->pollinfos[1], sizeof(AFD_POLL_INFO));
}

int
socket_afd_reactor_add(socket_afd_reactor_t *reactor, socket_t socket, socket_event_t socket_events) {
  struct socket_afd_event *event = NULL;

  if (!(event = socket_afd_reactor_events_event(reactor, socket)))
    return -1;

  event->io_status_block.Status = STATUS_PENDING;

  event->pollinfos[0].Exclusive        = false;
  event->pollinfos[0].NumberOfHandles  = 1;
  event->pollinfos[0].Timeout.QuadPart = INT64_MAX;

  if (socket_events & SOCKET_READ)
    event->pollinfos[0].Handles[0].Events |= AFD_POLL_ACCEPT|AFD_POLL_RECEIVE|AFD_POLL_RECEIVE_EXPEDITED;

  if (socket_events & SOCKET_WRITE)
    event->pollinfos[0].Handles[0].Events |= AFD_POLL_SEND|AFD_POLL_CONNECT;

  if (socket_events & SOCKET_EXCEPT)
    event->pollinfos[0].Handles[0].Events |= AFD_POLL_DISCONNECT|AFD_POLL_ABORT|AFD_POLL_LOCAL_CLOSE|AFD_POLL_CONNECT_FAIL;

  event->pollinfos[0].Handles[0].Handle = socket_afd_reactor_base_socket(socket);
  event->pollinfos[0].Handles[0].Status = 0;
  event->socket = socket;

  if (socket_afd_reactor_device_io_control_file(reactor, event) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return socket_afd_reactor_events_add(reactor, event);
}

static inline int
socket_afd_reactor_events_remove(socket_afd_reactor_t *reactor, struct socket_afd_event *event) {
  return rbtree_remove(reactor->events, event, sizeof(struct socket_afd_event));
}

int
socket_afd_reactor_remove(socket_afd_reactor_t *reactor, socket_t socket) {
  struct socket_afd_event *event = NULL;

  if (!(event = socket_afd_reactor_events_find(reactor, socket)))
    return -1;

  return socket_afd_reactor_events_remove(reactor, event);
}

socket_afd_reactor_t *
socket_afd_reactor_new(void) {
  static const ACCESS_MASK device_access = SYNCHRONIZE;
  static const ULONG device_share_access = FILE_SHARE_READ|FILE_SHARE_WRITE;

  static OBJECT_ATTRIBUTES device_attr = {0};
  static UNICODE_STRING device_name = {0};

  NTSTATUS status = -1;

  socket_afd_reactor_t *reactor = NULL;

  if (!(reactor = (socket_afd_reactor_t *)calloc(1, sizeof(socket_afd_reactor_t)))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  RtlInitUnicodeString(&device_name, L"\\Device\\Afd\\NetworkSocketsAfdReactor");
  InitializeObjectAttributes(&device_attr, &device_name, OBJ_CASE_INSENSITIVE, NULL, NULL);

  status = NtCreateFile(&reactor->afd_handle, device_access, &device_attr, &reactor->io_status_block, NULL, 0, device_share_access, FILE_OPEN, 0, NULL, 0);
  if (status == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  reactor->iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
  if (reactor->iocp_handle == INVALID_HANDLE_VALUE) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (CreateIoCompletionPort(reactor->afd_handle, reactor->iocp_handle, 0, 0) == INVALID_HANDLE_VALUE) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (!SetFileCompletionNotificationModes(reactor->afd_handle, FILE_SKIP_SET_EVENT_ON_HANDLE)) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (!(reactor->events = rbtree_new(socket_afd_reactor_events_compare, socket_afd_reactor_events_free))) {
    socket_seterror(SOCKET_ESYSCALL);
    goto _return;
  }

  if (socket_reactor_interrupter_new(reactor->interrupter) == -1)
    goto _return;

  socket_afd_reactor_add(reactor, reactor->interrupter[0], SOCKET_READ);

  return reactor;

_return:
  socket_afd_reactor_free(reactor);
  return NULL;
}

void
socket_afd_reactor_free(socket_afd_reactor_t *reactor) {
  if (reactor) {
    socket_reactor_interrupter_close(reactor->interrupter);

    if (reactor->afd_handle != INVALID_HANDLE_VALUE)
      CloseHandle(reactor->afd_handle), reactor->afd_handle = INVALID_HANDLE_VALUE;

    if (reactor->iocp_handle != INVALID_HANDLE_VALUE)
      CloseHandle(reactor->iocp_handle), reactor->iocp_handle = INVALID_HANDLE_VALUE;

    rbtree_free(reactor->events);
  }
}

#if defined SOCKET_HANDLER_AFD_REACTOR
struct socket_reactor_base socket_reactor_base = {
  socket_afd_reactor_new,
  socket_afd_reactor_free,
  socket_afd_reactor_run,
  socket_afd_reactor_cancel,
  socket_afd_reactor_add,
  socket_afd_reactor_remove
};

#endif
