#include "sockets-iocp.h"
#include "sockets-errors.h"

#include "sockets.h"

#include <stdlib.h>


static bool socket_windows_api_started = false;
static WSADATA socket_windows_api_data;


__attribute__((__constructor__))
int
socket_windows_api_startup(void) {
  if (WSAStartup(MAKEWORD(SOCKET_WINDOWS_API_VERSION_MINOR, SOCKET_WINDOWS_API_VERSION_MAJOR), &socket_windows_api_data) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_windows_api_started = true;

  return 1;
}

__attribute__((__destructor__))
int
socket_windows_api_cleanup(void) {
  if (WSACleanup() < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_windows_api_started = false;

  return 1;
}


struct socket_overlapped {
  WSAOVERLAPPED overlapped;
  DWORD transferred;
  DWORD error;
};


static void CALLBACK
socket_overlapped_post(DWORD error, DWORD transferred, LPWSAOVERLAPPED overlapped, DWORD flags) {
  struct socket_overlapped *socket_overlapped = (struct socket_overlapped *)overlapped;

  socket_overlapped->transferred = transferred;
  socket_overlapped->error = error;
}


ssize_t
socket_recvfrom(socket_t socket, void *buffer, size_t buffer_length, int flags, struct sockaddr *socket_addr, socklen_t *socket_addr_length, struct socket_timeout *socket_timeout) {
  WSABUF wsabuffer = { .len = buffer_length, .buf = buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t received = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    /* status = */ WSARecvFrom(socket, &wsabuffer, 1, (LPDWORD)&received, (LPDWORD)&flags, socket_addr, (int *)socket_addr_length, &overlapped, NULL);
    syscall_error = syscall_geterrno();
    if (received > 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    syscall_seterrno(0);

    socket_event = socket_wait(socket, SOCKET_READ, socket_timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      received = socket_event;

      break;
    }
  }

  return received;
}

ssize_t
socket_sendto(socket_t socket, const void *buffer, size_t buffer_length, int flags, struct sockaddr *socket_addr, socklen_t socket_addr_size) {
  WSABUF wsabuffer = { .len = buffer_length, .buf = (void *)buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t sent = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    /* status = */ WSASendTo(socket, &wsabuffer, 1, (LPDWORD)&sent, (DWORD)flags, socket_addr, (int)socket_addr_size, &overlapped, NULL);
    syscall_error = syscall_geterrno();
    if (sent >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    syscall_seterrno(0);

    socket_wait(socket, SOCKET_WRITE, NULL);
  }

  return sent;
}

ssize_t
socket_recv(socket_t socket, void *buffer, size_t buffer_length, int flags, struct socket_timeout *socket_timeout) {
  WSABUF wsabuffer = { .len = buffer_length, .buf = buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t received = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    /* status = */ WSARecv(socket, &wsabuffer, 1, (LPDWORD)&received, (LPDWORD)&flags, &overlapped, NULL);
    syscall_error = syscall_geterrno();
    if (received > 0 && !syscall_error)
      break;

    if (received == 0) {
      socket_seterror(SOCKET_ECLOSED);
      received = -1;

      break;
    }

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    syscall_seterrno(0);

    socket_event = socket_wait(socket, SOCKET_READ, socket_timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      received = socket_event;

      break;
    }
  }

  return received;
}

ssize_t
socket_send(socket_t socket, const void *buffer, size_t buffer_length, int flags) {
  WSABUF wsabuffer = { .len = buffer_length, .buf = (void *)buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t sent = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    /* status = */ WSASend(socket, &wsabuffer, 1, (LPDWORD)&sent, (DWORD)flags, &overlapped, NULL);
    syscall_error = syscall_geterrno();
    if (sent >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    syscall_seterrno(0);

    socket_wait(socket, SOCKET_WRITE, NULL);
  }

  return sent;
}

int
socket_setoption(socket_t socket, int socket_level, int socket_option, const void *value, socklen_t size) {
  if (setsockopt(socket, socket_level, socket_option, value, (int)size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_getoption(socket_t socket, int socket_level, int socket_option, void *value, socklen_t *size) {
  if (getsockopt(socket, socket_level, socket_option, value, (int *)size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_setblocking(socket_t socket, bool blocking) {
  unsigned long socket_fionbio = ((blocking) ? 0 : 1);

  if (ioctlsocket(socket, (long int)FIONBIO, &socket_fionbio)) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socket_t
socket_accept(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_length, struct socket_timeout *socket_timeout) {
  char socket_addr_buffer[(sizeof(struct sockaddr_in) + 16) * 2] = {0};
  DWORD socket_addr_buffer_size = sizeof(struct sockaddr_in)+16;

  struct sockaddr_storage socket_bind_addr = {0};
  socklen_t socket_bind_addr_size;

  socket_t socket_connection = SOCKET_INVALID;

  GUID wsaid_acceptex = WSAID_ACCEPTEX;
  static LPFN_ACCEPTEX AcceptEx = NULL;
  DWORD bytes = 0;

  OVERLAPPED overlapped = {0};

  bool status = false;

  if (!AcceptEx && WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &wsaid_acceptex, sizeof(wsaid_acceptex), &AcceptEx, sizeof(AcceptEx), &bytes, NULL, NULL) == SOCKET_ERROR) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  if (socket_getname(socket, (struct sockaddr *)&socket_bind_addr, &socket_bind_addr_size) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  if ((socket_connection = socket_create(socket_bind_addr.ss_family, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_INVALID) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    status = AcceptEx(socket, socket_connection, socket_addr_buffer, 0, socket_addr_buffer_size, socket_addr_buffer_size, NULL, &overlapped);
    syscall_error = syscall_geterrno();
    if (status && !syscall_error) {
      if (socket_setoption(socket_connection, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&socket, sizeof(socket)) == -1) {
        socket_close(socket_connection);
        socket_connection = SOCKET_INVALID;
      }

      GetAcceptExSockaddrs(socket_addr_buffer, 0, socket_addr_buffer_size, socket_addr_buffer_size, NULL, NULL, &socket_addr, (int *)&socket_addr_length);

      if (socket_connection == SOCKET_INVALID)
        socket_seterror(SOCKET_ESYSCALL);

      break;
    }

    if (!status && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    syscall_seterrno(0);

    socket_event = socket_wait(socket, SOCKET_READ, socket_timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      break;
    }
  }

  return socket_connection;
}

int
socket_bind(socket_t socket, struct sockaddr *socket_addr, socklen_t socket_addr_size) {
  if (bind(socket, socket_addr, (int)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_listen(socket_t socket, int backlog) {
  if (listen(socket, backlog) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_connect(socket_t socket, struct sockaddr *socket_addr, socklen_t socket_addr_size, struct socket_timeout *timeout) {
  struct sockaddr_storage socket_bind_addr = { .ss_family = (int16_t)socket_addr->sa_family };
  socklen_t socket_bind_addr_size = ((socket_addr->sa_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));

  GUID wsaid_connectex = WSAID_CONNECTEX;
  static LPFN_CONNECTEX ConnectEx = NULL;
  DWORD bytes = 0;

  OVERLAPPED overlapped = {0};

  int connected = -1;
  bool status = false;

  if (!ConnectEx && WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &wsaid_connectex, sizeof(wsaid_connectex), &ConnectEx, sizeof(ConnectEx), &bytes, NULL, NULL) == SOCKET_ERROR) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  if (socket_bind(socket, (struct sockaddr *)&socket_bind_addr, socket_bind_addr_size) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  for (;;) {
    syscall_errno_t syscall_error;

    status = ConnectEx(socket, socket_addr, (int)socket_addr_size, NULL, 0, NULL, &overlapped);
    syscall_error = syscall_geterrno();
    if (status && !syscall_error) {
      connected = 1;
      break;
    }
    if (!status && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      connected = -1;

      break;
    }

    syscall_seterrno(0);

    connected = socket_wait(socket, SOCKET_WRITE, timeout);
    if (!connected) {
      socket_seterror(SOCKET_ETIMEOUT);
      break;
    }
    if (connected == 1)
      break;
  }

  return connected;
}

int
socket_shutdown(socket_t socket, int method) {
  if (shutdown(socket, method) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_close(socket_t socket) {
  int closed = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    closed = closesocket(socket);
    syscall_error = syscall_geterrno();
    if (syscall_error && syscall_error != WSAEINTR && syscall_error != WSAEINPROGRESS && syscall_error != WSAEWOULDBLOCK) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (closed == 0) {
      closed = 1;
      break;
    }
  }

  return closed;
}

int
socket_getname(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_size) {
  if (getsockname(socket, socket_addr, (int *)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_getpeer(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_size) {
  if (getpeername(socket, socket_addr, (int *)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socket_t
socket_create(int socket_family, int socket_type, int socket_protocol) {
  socket_t socket = SOCKET_INVALID;

  if ((socket = WSASocket(socket_family, socket_type, socket_protocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_INVALID) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  return socket;
}
