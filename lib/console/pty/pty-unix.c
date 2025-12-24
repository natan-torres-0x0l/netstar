#define _XOPEN_SOURCE 600

#include <console/console.h>
#include "pty.h"

#include <sys/select.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <stdlib.h>

#include <stdbool.h>
#include <stddef.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>


static struct termios console_tty_attr = {0};
static bool console_tty_attr_initialized;


static inline int
console_pty_tty_stdin(void) {
  return STDIN_FILENO;
}

static inline int
console_pty_tty_stdout(void) {
  return STDOUT_FILENO;
}

static inline int
console_pty_tty_stderr(void) {
  return STDERR_FILENO;
}

static inline void
console_pty_tty_restore(void) {
  if (!console_tty_attr_initialized || tcsetattr(console_pty_tty_stdin(), TCSANOW, &console_tty_attr) == -1)
    return;

  console_tty_attr_initialized = false;
}

static inline void
console_pty_tty_restore_at_signal(int signal) {
  console_pty_tty_restore();
  exit(signal);
}

static inline void
console_pty_tty_restore_at_interrupt(void) {
  signal(SIGINT, console_pty_tty_restore_at_signal);
}

static inline void
console_pty_tty_restore_at_exit(void) {
  atexit(console_pty_tty_restore);
}

static inline void
console_pty_tty_raw(void) {
  int tty = console_pty_tty_stdin();
	struct termios tty_attr = {0};

  if (console_tty_attr_initialized || tcgetattr(tty, &console_tty_attr) == -1)
    return;

  tty_attr = console_tty_attr;

  tty_attr.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  tty_attr.c_oflag &= ~OPOST;
  tty_attr.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  tty_attr.c_cflag &= ~(CSIZE|PARENB);
  tty_attr.c_cflag |= CS8;

  if (tcsetattr(tty, TCSANOW, &tty_attr) == -1)
    return;

  console_tty_attr_initialized = true;

  console_pty_tty_restore_at_interrupt();
  console_pty_tty_restore_at_exit();
}

static int
console_pty_master_open(int *master) {
  if ((*master = posix_openpt(O_RDWR|O_NOCTTY)) == -1)
    return -1;

  if (grantpt(*master) == -1 || unlockpt(*master) == -1)
    return -1;

  return 1;
}

static inline void
console_pty_slave_duplicate(int slave) {
  dup2(slave, console_pty_tty_stdin());
  dup2(slave, console_pty_tty_stdout());
  dup2(slave, console_pty_tty_stderr());

  if (slave > console_pty_tty_stdin())
    close(slave);
}

static inline void
console_pty_slave_exec(int slave, char **command) {
  execvp(command[0], command);
  close(slave);
  exit(EXIT_FAILURE);
}

static pid_t
console_pty_slave_open(int master, int *slave) {
  struct winsize slave_window_size = { .ws_row = console_height(), .ws_col = console_width() };
  char *slave_name = NULL;
  pid_t pid = -1;

  if (!(slave_name = ptsname(master)) || (pid = fork()) != 0)
    return pid;

  close(master);

  if ((*slave = open(slave_name, O_RDWR)) == -1)
    return -1;

  setsid();

  if (ioctl(*slave, TIOCSCTTY, 0) < 0)
    return -1;

  if (ioctl(*slave, TIOCSWINSZ, &slave_window_size) < 0)
    return -1;

  console_pty_slave_duplicate(*slave);

  return pid;
}

int
console_pty_spawn(console_pty_t *pty, char **command) {
  int master = -1, slave = -1;
  pid_t pid = -1;

  if (console_pty_master_open(&master) == -1 || (pid = console_pty_slave_open(master, &slave)) == -1)
    return -1;
  if (pid == 0)
    console_pty_slave_exec(slave, command);

// console_pty_tty_raw();

  pty->master = master;
  pty->pid = pid;

  return 1;
}

ssize_t
console_pty_read(console_pty_t *pty, void *buffer, size_t length) {
  return read(pty->master, buffer, length);
}

ssize_t
console_pty_write(console_pty_t *pty, const void *buffer, size_t length) {
  return write(pty->master, buffer, length);
}

static inline void
console_pty_wait_pid(console_pty_t *pty) {
  waitpid(pty->pid, NULL, 0);
}

void
console_pty_close(console_pty_t *pty) {
  if (pty->master != -1) {
    if (close(pty->master) == -1)
      return;
    pty->master = -1;

    console_pty_wait_pid(pty);
  }

  console_pty_tty_restore();
}

#define CONSOLE_PTY_PROCESS_BUFFER_LENGTH 520

struct console_pty_process {
  char buffer[CONSOLE_PTY_PROCESS_BUFFER_LENGTH];
  ssize_t bytes;
  
  int select_descriptor, master;
  fd_set readset_events;

  bool status;
};


static inline bool
console_pty_process_select_event(fd_set *readset_events, int pty) {
  return FD_ISSET(pty, readset_events);
}

#define CONSOLE_PTY_STDIN  0x00
#define CONSOLE_PTY_MASTER 0x01

static int
console_pty_process_select(struct console_pty_process *process) {
  fd_set readset_events = process->readset_events;

  if (select(process->select_descriptor+1, &readset_events, NULL, NULL, NULL) <= 0)
    return -1;

  if (console_pty_process_select_event(&readset_events, console_pty_tty_stdin()))
    return CONSOLE_PTY_STDIN;

  if (console_pty_process_select_event(&readset_events, process->master))
    return CONSOLE_PTY_MASTER;

  return -1;
}

static inline void
console_pty_process_initialize(struct console_pty_process *process, console_pty_t *pty) {
  process->select_descriptor = ((pty->master > console_pty_tty_stdin()) ? pty->master : console_pty_tty_stdin());
  process->master = pty->master;

  FD_SET(process->master, &process->readset_events);
  FD_SET(console_pty_tty_stdin(), &process->readset_events);

  process->status = true;
}

int
console_pty_login(console_pty_t *pty, console_pty_input_t input, console_pty_output_t output, void *args) {
  struct console_pty_process process = {0};

  console_pty_process_initialize(&process, pty);

  for (; process.status;) {
    switch (console_pty_process_select(&process)) {
      case CONSOLE_PTY_MASTER: {
        if ((process.bytes = read(pty->master, process.buffer, sizeof(process.buffer))) <= 0)
          return -1;

        output(pty, process.buffer, process.bytes, args);
        break;
      }

      case CONSOLE_PTY_STDIN: {
        if ((process.bytes = read(console_pty_tty_stdin(), process.buffer, sizeof(process.buffer))) <= 0)
          return -1;

        input(pty, process.buffer, process.bytes, args);
        break;
      }

      default:
        process.status = false;
        break;
    }
  }

  return 1;
}
