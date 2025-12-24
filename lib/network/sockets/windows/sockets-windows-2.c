#include "sockets.h"

#include <stdlib.h>


static bool socket_windows_api_started = false;
static WSADATA socket_windows_api_data;


__attribute__((__constructor__))
int
socket_startup(void) {
  if (WSAStartup(MAKEWORD(0x02, 0x02), &socket_windows_api_data) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_windows_api_started = true;

  return 1;
}

__attribute__((__destructor__))
int
socket_cleanup(void) {
  if (WSACleanup() < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_windows_api_started = false;

  return 1;
}


struct socket_overlapped_data {
  WSAOVERLAPPED overlapped;
  DWORD transferred;
  DWORD error;
};


static void CALLBACK
socketfd_overlapped_post(DWORD error, DWORD transferred, LPWSAOVERLAPPED overlapped, DWORD flags) {
  struct socket_overlapped_data *overlapped_data = (struct socket_overlapped_data *)overlapped;

  overlapped_data->transferred = transferred;
  overlapped_data->error = error;
}


ssize_t
socketfd_recvfrom(socketfd_t socketfd, void *buffer, size_t length, int flags, struct sockaddr *socket_addr, socklen_t *socket_addr_size, struct socket_timeout *timeout) {
  WSABUF wsabuffer = { .len = length, .buf = buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t received = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    /* status = */ WSARecvFrom(socketfd, &wsabuffer, 1, (LPDWORD)&received, (LPDWORD)&flags, socket_addr, (int *)socket_addr_size, &overlapped, NULL);
    syscall_error = socket_getapierror();
    if (received > 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    socket_setapierror(0);

    socket_event = socketfd_wait(socketfd, SOCKET_READ, timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      received = socket_event;

      break;
    }
  }

  return received;
}

ssize_t
socketfd_sendto(socketfd_t socketfd, const void *buffer, size_t length, int flags, struct sockaddr *socket_addr, socklen_t socket_addr_size) {
  WSABUF wsabuffer = { .len = length, .buf = (void *)buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t sended = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    /* status = */ WSASendTo(socketfd, &wsabuffer, 1, (LPDWORD)&sended, (DWORD)flags, socket_addr, (int)socket_addr_size, &overlapped, NULL);
    syscall_error = socket_getapierror();
    if (sended >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    socket_setapierror(0);

    socketfd_wait(socketfd, SOCKET_WRITE, NULL);
  }

  return sended;
}

ssize_t
socketfd_recv(socketfd_t socketfd, void *buffer, size_t length, int flags, struct socket_timeout *timeout) {
  WSABUF wsabuffer = { .len = length, .buf = buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t received = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    /* status = */ WSARecv(socketfd, &wsabuffer, 1, (LPDWORD)&received, (LPDWORD)&flags, &overlapped, NULL);
    syscall_error = socket_getapierror();
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

    socket_setapierror(0);

    socket_event = socketfd_wait(socketfd, SOCKET_READ, timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      received = socket_event;

      break;
    }
  }

  return received;
}

ssize_t
socketfd_send(socketfd_t socketfd, const void *buffer, size_t length, int flags) {
  WSABUF wsabuffer = { .len = length, .buf = (void *)buffer };
  WSAOVERLAPPED overlapped = {0};

  ssize_t sended = -1;
// int status = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    /* status = */ WSASend(socketfd, &wsabuffer, 1, (LPDWORD)&sended, (DWORD)flags, &overlapped, NULL);
    syscall_error = socket_getapierror();
    if (sended >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    socket_setapierror(0);

    socketfd_wait(socketfd, SOCKET_WRITE, NULL);
  }

  return sended;
}

int
socketfd_setoption(socketfd_t socketfd, int socket_level, int socket_option, const void *value, socklen_t size) {
  if (setsockopt(socketfd, socket_level, socket_option, value, (int)size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_getoption(socketfd_t socketfd, int socket_level, int socket_option, void *value, socklen_t *size) {
  if (getsockopt(socketfd, socket_level, socket_option, value, (int *)size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_setblocking(socketfd_t socketfd, bool blocking) {
  unsigned long socket_fionbio = ((blocking) ? 0 : 1);

  if (ioctlsocket(socketfd, (long int)FIONBIO, &socket_fionbio)) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socketfd_t
socketfd_accept(socketfd_t socketfd, struct sockaddr *socket_addr, socklen_t *socket_addr_size, struct socket_timeout *timeout) {
  char socket_addr_buffer[(sizeof(struct sockaddr_in) + 16) * 2] = {0};
  DWORD socket_addr_buffer_size = sizeof(struct sockaddr_in)+16;

  struct sockaddr_storage socket_bind_addr = {0};
  socklen_t socket_bind_addr_size;

  socketfd_t socket_connection = SOCKET_INVALID;

  GUID wsaid_acceptex = WSAID_ACCEPTEX;
  static LPFN_ACCEPTEX AcceptEx = NULL;
  DWORD bytes = 0;

  OVERLAPPED overlapped = {0};

  bool status = false;

  if (!AcceptEx && WSAIoctl(socketfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &wsaid_acceptex, sizeof(wsaid_acceptex), &AcceptEx, sizeof(AcceptEx), &bytes, NULL, NULL) == SOCKET_ERROR) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  if (socketfd_getname(socketfd, (struct sockaddr *)&socket_bind_addr, &socket_bind_addr_size) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  if ((socket_connection = socketfd_create(socket_bind_addr.ss_family, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_INVALID) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  for (;;) {
    syscall_errno_t syscall_error;
    int socket_event;

    status = AcceptEx(socketfd, socket_connection, socket_addr_buffer, 0, socket_addr_buffer_size, socket_addr_buffer_size, NULL, &overlapped);
    syscall_error = socket_getapierror();
    if (status && !syscall_error) {
      if (socketfd_setoption(socket_connection, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&socketfd, sizeof(socketfd)) == -1) {
        socketfd_close(socket_connection);
        socket_connection = SOCKET_INVALID;
      }

      GetAcceptExSockaddrs(socket_addr_buffer, 0, socket_addr_buffer_size, socket_addr_buffer_size, NULL, NULL, &socket_addr, (int *)&socket_addr_size);

      if (socket_connection == SOCKET_INVALID)
        socket_seterror(SOCKET_ESYSCALL);

      break;
    }

    if (!status && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    socket_setapierror(0);

    socket_event = socketfd_wait(socketfd, SOCKET_READ, timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      break;
    }
  }

  return socket_connection;
}

int
socketfd_bind(socketfd_t socketfd, struct sockaddr *socket_addr, socklen_t socket_addr_size) {
  if (bind(socketfd, socket_addr, (int)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_listen(socketfd_t socketfd, int backlog) {
  if (listen(socketfd, backlog) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_connect(socketfd_t socketfd, struct sockaddr *socket_addr, socklen_t socket_addr_size, struct socket_timeout *timeout) {
  struct sockaddr_storage socket_bind_addr = { .ss_family = (int16_t)socket_addr->sa_family };
  socklen_t socket_bind_addr_size = ((socket_addr->sa_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));

  GUID wsaid_connectex = WSAID_CONNECTEX;
  static LPFN_CONNECTEX ConnectEx = NULL;
  DWORD bytes = 0;

  OVERLAPPED overlapped = {0};

  int connected = -1;
  bool status = false;

  if (!ConnectEx && WSAIoctl(socketfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &wsaid_connectex, sizeof(wsaid_connectex), &ConnectEx, sizeof(ConnectEx), &bytes, NULL, NULL) == SOCKET_ERROR) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  if (socketfd_bind(socketfd, (struct sockaddr *)&socket_bind_addr, socket_bind_addr_size) == -1) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  for (;;) {
    syscall_errno_t syscall_error;

    status = ConnectEx(socketfd, socket_addr, (int)socket_addr_size, NULL, 0, NULL, &overlapped);
    syscall_error = socket_getapierror();
    if (status && !syscall_error) {
      connected = 1;
      break;
    }
    if (!status && syscall_error != WSA_IO_PENDING) {
      socket_seterror(SOCKET_ESYSCALL);
      connected = -1;

      break;
    }

    socket_setapierror(0);

    connected = socketfd_wait(socketfd, SOCKET_WRITE, timeout);
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
socketfd_shutdown(socketfd_t socketfd, int method) {
  if (shutdown(socketfd, method) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_close(socketfd_t socketfd) {
  int closed = -1;

  for (;;) {
    syscall_errno_t syscall_error;

    closed = closesocket(socketfd);
    syscall_error = socket_getapierror();
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
socketfd_getname(socketfd_t socketfd, struct sockaddr *socket_addr, socklen_t *socket_addr_size) {
  if (getsockname(socketfd, socket_addr, (int *)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socketfd_getpeer(socketfd_t socketfd, struct sockaddr *socket_addr, socklen_t *socket_addr_size) {
  if (getpeername(socketfd, socket_addr, (int *)socket_addr_size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socketfd_t
socketfd_create(int socket_family, int socket_type, int socket_protocol) {
  socketfd_t socketfd = SOCKET_INVALID;

  if ((socketfd = WSASocket(socket_family, socket_type, socket_protocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == SOCKET_INVALID) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  return socketfd;
}
