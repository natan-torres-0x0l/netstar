#ifndef _CONSOLE_CONSOLE_H
#define _CONSOLE_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include "console-windows.h"

#elif defined __unix__ || defined __unix
  #include "console-unix.h"

#endif

#include <stdbool.h>
#include <stddef.h>

#define CONSOLE_ASCII_ESCAPE "\033" // "\e", "\x1b"


extern void console_setcursorxy(size_t, size_t);
extern void console_setcursor(bool);

extern void console_clear(void);

extern size_t console_height(void);
extern size_t console_width(void);

extern int console_setbuffer(bool);
extern int console_setecho(bool);

#ifdef __cplusplus
}
#endif

#endif
