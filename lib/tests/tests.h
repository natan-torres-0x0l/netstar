#ifndef _TESTS_TESTS_H
#define _TESTS_TESTS_H

#include <stdbool.h>
#include <stddef.h>

#include <libgen.h>

#include <stdio.h>


static size_t __internal_tests, __internal_tests_completed, __internal_tests_failure;


#define test(description, func, statement)             \
  __attribute__((__constructor__))                     \
  static inline void func(void) {                      \
    fprintf(stdout, " @ testing %s\r\n", __func__); /* " %s\r\n", __func__, description); */ \
    __internal_tests++;                                \
    statement;                                         \
    __internal_tests_completed++;                      \
    puts("");                                          \
  }

#define assert(block)                                              \
  if (block) {                                                     \
      fprintf(stdout, " %s\r\n", #block);                          \
  } else {                                                         \
    fprintf(stderr, " %s:%d: %s: assertion \"%s\" failed\r\n\r\n", \
      basename(__FILE__),  __LINE__,  __func__, #block);           \
    __internal_tests_failure++;                                    \
    return;                                                        \
  }

#define __internal_start() int main(void) { return 0; }

static inline void
__internal_report_message(void) {
  fprintf(stdout, " +---------------------------------------+"
                  "\r\n |"
                  "\r\n |  @ Report: total tests: %02zu"
                  "\r\n |    @ tests completed : %02zu"
                  "\r\n |    @ tests failed    : %02zu\r\n\r\n", __internal_tests, __internal_tests_completed,
                                                               __internal_tests_failure);
}

#define __internal_report()       \
  __attribute__((__destructor__)) \
  static void                     \
  report(void) {                  \
    __internal_report_message();  \
  }

__internal_start();

__internal_report();

#endif
