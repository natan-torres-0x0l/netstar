#ifndef _CONSOLE_PTY_PTY_H
#define _CONSOLE_PTY_PTY_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "pty-windows.h"

#else // elif defined __unix__ || defined __unix
  #include "pty-unix.h"

#endif

typedef struct console_pty console_pty_t;

typedef void (*console_pty_input_t)(console_pty_t *, const void *, size_t, void * /* , const struct console_pty_timeout * */);
typedef void (*console_pty_output_t)(console_pty_t *, const void *, size_t, void *);


extern int console_pty_login(console_pty_t *, console_pty_input_t, console_pty_output_t, void *);

extern ssize_t console_pty_read(console_pty_t *, void *, size_t/*, const struct console_pty_timeout * */);
extern ssize_t console_pty_write(console_pty_t *, const void *, size_t);

extern int console_pty_spawn(console_pty_t *, char **);
extern void console_pty_close(console_pty_t *);

#ifdef __cplusplus
}
#endif

#endif
