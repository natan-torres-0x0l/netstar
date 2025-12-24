#ifndef _NETSTAR_FILTER_H
#define _NETSTAR_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <netstar-capture.h>
#include <netstar.h>

#include <buffers/buffers.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define NETSTAR_FILTER_STACK_SIZE 1024 // 1024*256
#define NETSTAR_FILTER_TEXT_SIZE  1024 // 1024*256
#define NETSTAR_FILTER_DATA_SIZE  1024 // 1024*256


typedef enum {
  NETSTAR_FILTER_TOKEN_INVALID = -1,
  NETSTAR_FILTER_TOKEN_END,
  NETSTAR_FILTER_TOKEN_STARTBRACE,
  NETSTAR_FILTER_TOKEN_ENDBRACE,
  NETSTAR_FILTER_TOKEN_IDENTIFIER,
  NETSTAR_FILTER_TOKEN_OR,
  NETSTAR_FILTER_TOKEN_AND,
  NETSTAR_FILTER_TOKEN_NOT,
  NETSTAR_FILTER_TOKEN_EQUALS,
  NETSTAR_FILTER_TOKEN_NOTEQUALS,
  NETSTAR_FILTER_TOKEN_GREATER,
  NETSTAR_FILTER_TOKEN_GREATEROREQUALS,
  NETSTAR_FILTER_TOKEN_LESS,
  NETSTAR_FILTER_TOKEN_LESSOREQUALS
} netstar_filter_token_t;

typedef enum {
  NETSTAR_FILTER_INSTRUCTION_MOV,
  NETSTAR_FILTER_INSTRUCTION_PUSH,
  NETSTAR_FILTER_INSTRUCTION_OR,
  NETSTAR_FILTER_INSTRUCTION_AND,
  NETSTAR_FILTER_INSTRUCTION_NOT,
  NETSTAR_FILTER_INSTRUCTION_EQUALS,
  NETSTAR_FILTER_INSTRUCTION_NOTEQUALS,
  NETSTAR_FILTER_INSTRUCTION_GREATER,
  NETSTAR_FILTER_INSTRUCTION_GREATEROREQUALS,
  NETSTAR_FILTER_INSTRUCTION_LESS,
  NETSTAR_FILTER_INSTRUCTION_LESSOREQUALS,
  NETSTAR_FILTER_INSTRUCTION_END
} netstar_filter_instruction_t;

typedef struct netstar_filter_compiler {
// uintptr_t stack[NETSTAR_FILTER_STACK_SIZE], text[NETSTAR_FILTER_SESSION_TEXT_SIZE];
// uint8_t data[NETSTAR_FILTER_SESSION_DATA_SIZE];

  buffer_t *stack, *text, *data;

  uintptr_t r0, r1, r2, r3, *r13, *r15;

  netstar_filter_token_t token;
  uintptr_t token_class, token_value;

  char *expression, *source;
  size_t shift, line;

  struct netstar_capture_packet *packet;
  netstar_t *netstar;
} netstar_filter_compiler_t;

typedef struct netstar_filter {
  netstar_filter_compiler_t state;
} netstar_filter_t;


extern bool netstar_filter_compile(netstar_filter_t *, struct netstar_capture_packet *);

extern bool netstar_filter_initialized(netstar_filter_t *);

extern int netstar_filter_new(netstar_filter_t *, netstar_t *, const char *);
extern void netstar_filter_free(netstar_filter_t *);

#ifdef __cplusplus
}
#endif

#endif
