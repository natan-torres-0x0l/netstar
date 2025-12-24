#include "console.h"

#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


__attribute__((__constructor__))
static inline int
console_virtual_terminal_initialize(void) {
  HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD console_mode = 0;

  if (!GetConsoleMode(console_output, &console_mode))
    return -1;

  console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  if (!SetConsoleMode(console_output, console_mode))
    return -1;

  return 1;
}

size_t
console_height(void) {
  CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer = {0};
  HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);

  if (!GetConsoleScreenBufferInfo(console_output, &console_screen_buffer))
    return 0;

  return (size_t)(console_screen_buffer.srWindow.Bottom-console_screen_buffer.srWindow.Top+1);
}

size_t
console_width(void) {
  CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer = {0};
  HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);

  if (!GetConsoleScreenBufferInfo(console_output, &console_screen_buffer))
    return 0;

  return (size_t)(console_screen_buffer.srWindow.Right-console_screen_buffer.srWindow.Left+1);
}

int
console_setbuffer(bool buffer) {
  HANDLE console_output = GetStdHandle(STD_INPUT_HANDLE);
  DWORD console_mode = 0;

  if (!GetConsoleMode(console_input, &console_mode))
    return -1;

  console_mode = ((buffer) ? console_mode|ENABLE_LINE_INPUT : console_mode & (DWORD)(~ENABLE_LINE_INPUT));

  if (!SetConsoleMode(console_input, console_mode))
    return -1;

  return 1;
}

int
console_setecho(bool echo) {
  HANDLE console_output = GetStdHandle(STD_INPUT_HANDLE);
  DWORD console_mode = 0;

  if (!GetConsoleMode(console_input, &console_mode))
    return -1;

  console_mode = ((echo) ? console_mode|ENABLE_ECHO_INPUT : console_mode^ENABLE_ECHO_INPUT);

  if (!SetConsoleMode(console_input, console_mode))
    return -1;

  return 1;
}
