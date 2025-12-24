#ifndef _CONSOLE_PTY_PTY_UNIX_H
#define _CONSOLE_PTY_PTY_UNIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

struct console_pty {
  int master; /*, slave; */
  pid_t pid;
};

#ifdef __cplusplus
}
#endif

#endif
