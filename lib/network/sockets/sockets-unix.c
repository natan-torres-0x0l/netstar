#include "sockets.h"


ssize_t
socket_recvfrom(socket_t socket, void *buffer, size_t buffer_length, int flags, struct sockaddr *socket_addr, socklen_t *socket_addr_length, const struct socket_timeout *socket_timeout) {
  ssize_t received = -1;

  for (;;) {
    system_error_t syscall_error;
    int socket_event;

    received = recvfrom(socket, buffer, buffer_length, flags, socket_addr, socket_addr_length);
    syscall_error = system_geterror();
    if (received > 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != EAGAIN && syscall_error != EWOULDBLOCK && syscall_error != EINTR) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    system_seterror(0);

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
socket_sendto(socket_t socket, const void *buffer, size_t buffer_length, int flags, struct sockaddr *socket_addr, socklen_t socket_addr_length) {
  ssize_t sent = -1;

  for (;;) {
    system_error_t syscall_error;

    sent = sendto(socket, buffer, buffer_length, flags, socket_addr, socket_addr_length);
    syscall_error = system_geterror();
    if (sent >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != EAGAIN && syscall_error != EWOULDBLOCK && syscall_error != EINTR) {
      socket_seterror(SOCKET_ESYSCALL); 
      break;
    }

    system_seterror(0);

    socket_wait(socket, SOCKET_WRITE, NULL);
  }

  return sent;
}

ssize_t
socket_recv(socket_t socket, void *buffer, size_t buffer_length, int flags, const struct socket_timeout *socket_timeout) {
  ssize_t received = -1;

  for (;;) {
    system_error_t syscall_error;
    int socket_event;
 
    received = recv(socket, buffer, buffer_length, flags);
    syscall_error = system_geterror();
    if (received > 0 && !syscall_error)
      break;

    if (received == 0) {
      socket_seterror(SOCKET_ECLOSED);
      received = -1;

      break;
    }

    if (syscall_error && syscall_error != EAGAIN && syscall_error != EWOULDBLOCK && syscall_error != EINTR) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    system_seterror(0);

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
  ssize_t sent = -1;

  for (;;) {
    system_error_t syscall_error;

    sent = send(socket, buffer, buffer_length, flags);
    syscall_error = system_geterror();
    if (sent >= 0 && !syscall_error)
      break;

    if (syscall_error && syscall_error != EAGAIN && syscall_error != EWOULDBLOCK && syscall_error != EINTR) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    system_seterror(0);

    socket_wait(socket, SOCKET_WRITE, NULL);
  }

  return sent;
}

int
socket_setoption(socket_t socket, int socket_level, int socket_option, const void *value, socklen_t size) {
  if (setsockopt(socket, socket_level, socket_option, value, size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_getoption(socket_t socket, int socket_level, int socket_option, void *value, socklen_t *size) {
  if (getsockopt(socket, socket_level, socket_option, value, size) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_setblocking(socket_t socket, bool blocking) {
  int socket_flags;

  if ((socket_flags = fcntl(socket, F_GETFL, 0)) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  socket_flags = ((blocking) ? socket_flags & (~O_NONBLOCK) : socket_flags|O_NONBLOCK);

  if (fcntl(socket, F_SETFL, socket_flags) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socket_t
socket_accept(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_length, const struct socket_timeout *socket_timeout) {
  socket_t socket_connection = SOCKET_INVALID;

  for (;;) {
    system_error_t syscall_error;
    int socket_event;

    socket_connection = accept(socket, socket_addr, socket_addr_length);
    syscall_error = system_geterror();
    if (socket_connection != SOCKET_INVALID && !syscall_error)
      break;

    if (syscall_error && syscall_error != EAGAIN && syscall_error != EWOULDBLOCK && syscall_error != EINTR) {
      socket_seterror(SOCKET_ESYSCALL);
      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    system_seterror(0);

    socket_event = socket_wait(socket, SOCKET_READ, socket_timeout);
    if (!socket_event) {
      socket_seterror(SOCKET_ETIMEOUT);
      break;
    }
  }

  if (socket_connection != SOCKET_INVALID)
    socket_setblocking(socket_connection, false);

  return socket_connection;
}

int
socket_bind(socket_t socket, struct sockaddr *socket_addr, socklen_t socket_addr_size) {
  if (bind(socket, socket_addr, socket_addr_size) < 0) {
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
socket_connect(socket_t socket, struct sockaddr *socket_addr, socklen_t socket_addr_length, const struct socket_timeout *socket_timeout) {
  int connected = -1;

  for (;;) {
    system_error_t syscall_error;

    connected = connect(socket, socket_addr, socket_addr_length);
    syscall_error = system_geterror();
    if (connected == 0 && !syscall_error) {
      connected = 1;
      break;
    }

    if (syscall_error && syscall_error != EINTR && syscall_error != EINPROGRESS) {
      socket_seterror(SOCKET_ESYSCALL);
      connected = -1;

      break;
    }

    if (socket_timeout == &SOCKET_NONBLOCKING)
      break;

    system_seterror(0);

    connected = socket_wait(socket, SOCKET_WRITE, socket_timeout);
    if (!connected) {
      socket_seterror(SOCKET_ETIMEOUT);
      break;
    }
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
    system_error_t syscall_error;

    closed = close(socket);
    syscall_error = system_geterror();
    if (syscall_error && syscall_error != EINTR) {
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
socket_localaddr(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_length) {
  if (getsockname(socket, socket_addr, socket_addr_length) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

int
socket_remoteaddr(socket_t socket, struct sockaddr *socket_addr, socklen_t *socket_addr_length) {
  if (getpeername(socket, socket_addr, socket_addr_length) < 0) {
    socket_seterror(SOCKET_ESYSCALL);
    return -1;
  }

  return 1;
}

socket_t
socket_open(int socket_family, int socket_type, int socket_protocol) {
  return socket(socket_family, socket_type, socket_protocol);
}

socket_t
socket_new(int socket_family, int socket_type, int socket_protocol) {
  socket_t socket = SOCKET_INVALID;

  if ((socket = socket_open(socket_family, socket_type, socket_protocol)) == SOCKET_INVALID) {
    socket_seterror(SOCKET_ESYSCALL);
    return SOCKET_INVALID;
  }

  socket_setblocking(socket, false);

  return socket;
}
