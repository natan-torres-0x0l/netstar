#if defined __linux__ || defined __linux
  #define _GNU_SOURCE // Using GNU source implementation for strerror_r
#endif

#include "system-errors.h"

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include <winsock2.h>
  #include <windows.h>

#else // elif defined __unix__ || defined __unix
  #include <string.h>
  #include <errno.h>

#endif

#include <strings/strings.h>

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#ifndef __cplusplus
  #define thread_local _Thread_local
#endif

#define SYSTEM_ERROR_LENGTH 2048 // _POSIX2_LINE_MAX


thread_local char system_error_message[SYSTEM_ERROR_LENGTH];
thread_local system_error_t system_error;


#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
const char *
system_error_reason(system_error_t error) {
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), system_error_message, sizeof(system_error_message), NULL);
  return string_tolower(system_error_message);
}

system_error_t
system_geterror(void) {
  return (((system_error = WSAGetLastError()) > WSABASEERR && system_error < WSABASEERR+1100) ? system_error : (system_error = GetLastError()));
}

void
system_seterror(system_error_t error) {
  (((system_error = error) > WSABASEERR && system_error < WSABASEERR+1100) ? WSASetLastError(error) : SetLastError(error));
}

#else // elif defined __unix__ || defined __unix
const char *
system_error_reason(system_error_t error) {
#if defined _POSIX_C_SOURCE // Using POSIX source implementation for strerror_r
  char *string = strerror_r(error, system_error_message, sizeof(system_error_message));
  string_safecopy(system_error_message, sizeof(system_error_message), string, string_length(string));
#else // elif defined _GNU_SOURCE // Using GNU source implementation for strerror_r
  strerror_r(error, system_error_message, sizeof(system_error_message));
#endif
  return string_tolower(system_error_message);
}

system_error_t
system_geterror(void) {
  return (system_error = errno);
}

void
system_seterror(system_error_t error) {
  system_error = errno = error;
}

#endif
