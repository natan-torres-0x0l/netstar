#include "arguments.h"

#include <strings/strings.h>

#include <stdlib.h>
#include <stdio.h>


static struct console_args_command console_args_subcommand_unknown;


struct console_args_command *
console_args_command_subcommand(struct console_args_command *command, const char *name) {
  size_t count;

  for (count = 0; count < command->subcommands.count; count++) {
    struct console_args_command *subcommand = ((command->subcommands.reference) ? ((struct console_args_command **)command->subcommands.commands)[count] : &((struct console_args_command *)command->subcommands.commands)[count]);

    if (string_equals(subcommand->name, name, true))
      return subcommand;
  }

  return &console_args_subcommand_unknown;
}

static bool
console_args_command_flags_parse(struct console_args_command *command, const char *argument) {
  size_t argument_length = string_length(argument);

  char *flags = (char *)command->flags;
  long separator;

  for (;;) {
    if ((separator = string_find(flags, "|", true)) == -1)
      separator = (long)string_length(flags);

    if (argument_length == separator && string_match(flags, argument, separator, command->sensitive))
      return true;

    if (!*(flags+separator))
      break;

    flags += separator+1;
  }

  return false;
}

#if impl
static void *
console_args_command_values_parse(struct console_args_command *command, char **arguments) {
  char **values = NULL;
  size_t count;

  if (!(values = calloc((size_t)command->values_count, sizeof(char *))))
    return NULL;

  for (count = 0; count < command->values_count; count++, arguments++)
    values[count] = *arguments;

/*
  if (string_equals(command->type, "string", false)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(char *))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      ((char **)values)[count] = *arguments;
  } else if (string_equals(command->type, "bool", false)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(bool))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      if (string_equals(*arguments, "true", true) || string_equals(*arguments, "1", true))
        ((bool *)values)[count] = true;
      else if (string_equals(*arguments, "false", true) || string_equals(*arguments, "0", true))
        ((bool *)values)[count] = false;
  } else if (string_equals(command->type, "int", false)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(int))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      sscanf(*arguments, "%i", &((int *)values)[count]);
  } else if (string_equals(command->type, "double", true)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(double))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      sscanf(*arguments, "%lf", &((double *)values)[count]);
  } else if (string_equals(command->type, "float", true)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(float))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      sscanf(*arguments, "%f", &((float *)values)[count]);
  } else if (string_equals(command->type, "char", true)) {
    if (!(values = calloc((size_t)command->values_count, sizeof(char))))
      return NULL;

    for (count = 0; count < command->values_count; count++, arguments++)
      sscanf(*arguments, "%c", &((char *)values)[count]);
  }
*/

  return values;
}

#endif

static bool
console_args_parser_reserved_command(console_args_parser_t *parser, const char *argument) {
  size_t index;

  for (index = 0; parser->commands[index]; index++)
    if (console_args_command_flags_parse(parser->commands[index], argument))
      return true;

  return false;
}

static bool
console_args_command_reserved_subcommand(struct console_args_command *command, const char *argument) {
  struct console_args_command *parent = command->subcommands.parent;
  size_t count;

  for (count = 0; parent && count < parent->subcommands.count; count++) {
    struct console_args_command *subcommand = ((parent->subcommands.reference) ? ((struct console_args_command **)parent->subcommands.commands)[count] : &((struct console_args_command *)parent->subcommands.commands)[count]);

    if (console_args_command_flags_parse(subcommand, argument))
      return true;
  }

  return false;
}

static long
console_args_parser_command_values_count(console_args_parser_t *parser, struct console_args_command *command, size_t index) {
  long values_count = 0;

  for (; index < parser->arguments.count; index++) {
    if (console_args_parser_reserved_command(parser, parser->arguments.vector[index]))
      break;

    if (console_args_command_reserved_subcommand(command, parser->arguments.vector[index]))
      break;

    values_count++;
  }

  return values_count;
}

static bool
console_args_parser_command_parse(console_args_parser_t *parser, struct console_args_command *command) {
  long values_count;
  size_t index, count;

  for (index = 0; index < parser->arguments.count; index++) {
    if (console_args_command_flags_parse(command, parser->arguments.vector[index])) {
      values_count = console_args_parser_command_values_count(parser, command, index+1);

      if (command->required_values < 0)
        command->required_values = values_count;

      if (command->required_values && command->subcommands.commands) {
        char **arguments = parser->arguments.vector; // -(index+1);
        size_t arguments_count = parser->arguments.count;

        parser->arguments.vector = &parser->arguments.vector[index+1];
        parser->arguments.count = (size_t)command->required_values;

        command->subcommands.parent = command;

        for (count = 0; count < command->subcommands.count; count++) {
          struct console_args_command *subcommand = ((command->subcommands.reference) ? ((struct console_args_command **)command->subcommands.commands)[count] : &((struct console_args_command *)command->subcommands.commands)[count]);

          console_args_parser_command_parse(parser, subcommand);

          if (subcommand->defined)
            command->subcommands.parent->subcommands.defined = true;
        }

        parser->arguments.vector = arguments;
        parser->arguments.count = arguments_count;
      }

   // if (command->subcommands.parent && command->subcommands.commands)
        parser->defined++;

      if (command->required_values && index+1 < parser->arguments.count) {
        command->values_count = ((command->required_values > parser->arguments.count-1) ? parser->arguments.count-1 : (size_t)command->required_values);
        command->values = &parser->arguments.vector[index+1]; // console_args_command_values_parse(command, &parser->arguments.vector[index+1]);
        command->value = *command->values;

        command->initialized = true;
      }

   // if (!command->subcommands.parent) {
   //   parser->arguments.vector = parser->arguments.vector+1+command->values_count;
   //   parser->arguments.count -= 1+command->values_count;
   // }

      command->defined = true;
      break;
    }
  }

  if (command->defined && command->routine)
    command->routine(command);

  return true;
}

size_t
console_args_parser_defined(console_args_parser_t *parser) {
  return parser->defined;
}

bool
console_args_parser_parse(console_args_parser_t *parser) {
  struct console_args_command *command = NULL;
  size_t index;

  for (index = 0; parser->commands[index]; index++) {
    command = parser->commands[index];

    if (!command->defined && !console_args_parser_command_parse(parser, command))
      return false;

    if (command->required && (!command->defined || (command->required_values > 0 && !command->initialized)))
      return false;
  }

  return true;
}

int
console_args_parser_initialize(console_args_parser_t *parser, struct console_args_command **commands, int argc, char **argv) {
  parser->arguments.vector = argv;
  parser->arguments.count = (size_t)argc;

  parser->commands = commands;

  return 1;
}
