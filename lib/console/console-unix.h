#ifndef _CONSOLE_CONSOLE_UNIX_H
#define _CONSOLE_CONSOLE_UNIX_H

#ifdef __cplusplus
extern "C" {
#endif

extern int console_stdout(void);
extern int console_stdin(void);
extern int console_stderr(void);

extern int console_tty(void);

#ifdef __cplusplus
}
#endif

#endif
