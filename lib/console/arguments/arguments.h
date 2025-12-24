#ifndef _CONSOLE_ARGUMENTS_ARGUMENTS_H
#define _CONSOLE_ARGUMENTS_ARGUMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>


#define console_args_subcommands_array(subcommands)     { (void *)subcommands, sizeof(subcommands)/sizeof(subcommands[0]), false, NULL, false }
#define console_args_subcommands_reference(subcommands) { (void *)subcommands, sizeof(subcommands)/sizeof(subcommands[0]), true, NULL, false }

struct console_args_subcommand {
// struct console_args_command **commands;
  void *commands;
  size_t count;

  bool reference;

  struct console_args_command *parent;

  bool defined;
};

typedef void (*netstar_args_command_routine_t)(struct console_args_command *);

struct console_args_command {
  const char *name, /* *type, */ *flags;
  bool sensitive, required;

  long required_values;

  netstar_args_command_routine_t routine;

  const char *help, *description;
  bool defined, initialized;

  char *value, **values;
  size_t values_count;

  struct console_args_subcommand subcommands;
};


extern void console_args_command_addsubcommands(struct console_args_command *, struct console_args_command *, size_t);
extern void console_args_command_addsubcommand(struct console_args_command *, struct console_args_command *);

extern struct console_args_command *console_args_command_subcommand(struct console_args_command *, const char *);

extern int console_args_command_subcommands_new(struct console_args_command *, struct console_args_command **, size_t);


struct console_args_parser_arguments {
  char **vector;
  size_t count;
};

typedef struct console_args_parser {
  struct console_args_parser_arguments arguments;

  struct console_args_command **commands;
  size_t defined;
} console_args_parser_t;


extern size_t console_args_parser_defined(console_args_parser_t *);

extern bool console_args_parser_parse(console_args_parser_t *);

extern int console_args_parser_initialize(console_args_parser_t *, struct console_args_command **, int, char **);

#ifdef __cplusplus
}
#endif

#endif
