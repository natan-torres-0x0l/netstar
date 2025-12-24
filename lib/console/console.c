#include "console.h"

#include <locale.h>
#include <stdio.h>


__attribute__((__constructor__))
static void
console_locale_initialize(void) {
// setlocale(LC_ALL, ".UTF-8");
  setlocale(LC_ALL, "");
}

void
console_setcursorxy(size_t x, size_t y) {
  fprintf(stdout, CONSOLE_ASCII_ESCAPE "[%zu;%zuf", y, x);
  fflush(stdout);
}

void
console_setcursor(bool cursor) {
  fprintf(stdout, ((!cursor) ? CONSOLE_ASCII_ESCAPE "[?25l" : CONSOLE_ASCII_ESCAPE "[?25h"));
  fflush(stdout);
}

void
console_clear(void) {
  fprintf(stdout, CONSOLE_ASCII_ESCAPE "[1;1f" CONSOLE_ASCII_ESCAPE "[2J");
  fflush(stdout);
}
