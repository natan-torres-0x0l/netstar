#ifndef _CONSOLE_PTY_PTY_WINDOWS_H
#define _CONSOLE_PTY_PTY_WINDOWS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <processthreadsapi.h>
#include <windows.h>

struct console_pty {
  HANDLE conpty_pty_input, conpty_pty_output;
  HANDLE conpty_app_input, conpty_app_output;

  STARTUPINFOEXA startup_info;
  HPCON conpty;

  PROCESS_INFORMATION process_info;
};

#ifdef __cplusplus
}
#endif

#endif
