#include "console.h"

#include <sys/select.h>
#include <sys/ioctl.h>

#include <fcntl.h>

#include <termios.h>
#include <unistd.h>


size_t
console_height(void) {
  struct winsize console_window_size = {0};
  int tty = STDOUT_FILENO;

  ioctl(tty, TIOCGWINSZ, &console_window_size);

  return console_window_size.ws_row;
}

size_t
console_width(void) {
  struct winsize console_window_size = {0};
  int tty = STDOUT_FILENO;

  ioctl(tty, TIOCGWINSZ, &console_window_size);

  return console_window_size.ws_col;
}

int
console_setbuffer(bool buffer) {
  struct termios tty_attr = {0};
  int tty = STDIN_FILENO;

  if (tcgetattr(tty, &tty_attr) < 0)
    return -1;

  tty_attr.c_lflag = ((buffer) ? tty_attr.c_lflag | ICANON : tty_attr.c_lflag & (tcflag_t)~ICANON); 

  if (tcsetattr(tty, TCSANOW, &tty_attr) < 0)
    return -1;

  return 1;
}

int
console_setecho(bool echo) {
  struct termios tty_attr = {0};
  int tty = STDIN_FILENO;

  if (tcgetattr(tty, &tty_attr) < 0)
    return -1;

  tty_attr.c_lflag = ((echo) ? tty_attr.c_lflag|ECHO : tty_attr.c_lflag & (tcflag_t)~ECHO); 

  if (tcsetattr(tty, TCSANOW, &tty_attr) < 0)
    return -1;

  return 1;
}
