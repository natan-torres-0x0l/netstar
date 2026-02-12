#include "strings.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <stdint.h>
#include <stdarg.h>

#include <ctype.h>


char *
string_replace(const char *string, const char *pattern, const char *substring, long count, bool match_case) {
  size_t substring_length = string_length(substring);
  size_t pattern_length = string_length(pattern);
  size_t required_length;

  const char *match = NULL;
  size_t position;

  char *replaced = NULL;
  size_t index = 0;

  if (count <= 0 && !(count = (long)string_count(string, pattern, match_case)))
    return string_new(string);

  required_length = string_length(string)-(pattern_length*(size_t)count)+(substring_length*(size_t)count);

  if (!(replaced = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (count && (match = string_search(string, pattern, match_case))) {
    position = match-string;

    string_copy(&replaced[index], string, position);
    index += position;

    string = match+pattern_length;

    string_copy(&replaced[index], substring, substring_length);
    index += substring_length;

    count--;
  }

  if (*string)
    string_copy(&replaced[index], string, string_length(string));

  return replaced;
}

char **
string_split(const char *string, const char *separator, bool match_case) {
  size_t count = string_count(string, separator, match_case);
  size_t separator_length = string_length(separator);
  size_t position;

  const char *match = string;
  char **strings = NULL;

  if ((*separator && !count) || !count) {
    if (!(strings = (char **)calloc(++count+1, sizeof(char *))))
      return NULL;
    *strings = string_new(string);

    return strings;
  }
  if (!*separator && !(count = string_length(string)))
    return NULL;

  if (!(strings = (char **)calloc(++count+1, sizeof(char *))))
    return NULL;

  count = 0;

  for (;;) {
    if (!(match = string_search(string, separator, match_case)))
      match = string+string_length(string);
    else if (!separator_length)
      match = string+sizeof(char);

    position = match-string;

    if (!(strings[count] = (char *)calloc(position+1, sizeof(char))))
      break;

    string_copy(strings[count], string, position);
    count++;

    if (!*match)
      break;

    string += separator_length+position;
  }

  return strings;
}

char *
string_join(const char **strings, const char *separator) {
  size_t separator_length = string_length(separator);
  size_t required_length = 0, index;

  char *string = NULL;

  for (index = 0; strings[index]; index++)
    required_length += string_length(strings[index])+((strings[index+1]) ? separator_length : 0);

  if (!(string = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  for (index = 0; strings[index]; index++) {
    string_concat(string, strings[index], string_length(strings[index]));

    if (strings[index+1])
      string_concat(string, separator, separator_length);
  }

  return string;
}

char **
string_fields(const char *string) {
  size_t count, index, characters;
  uint8_t space = 0;

  char **fields = NULL;

  for (count = 0, index = 0;; index++) {
    // @ identify spaces and fields
    //   1 active bit = sequence of spaces
    //   2 active bits = sequence of a field
    space = ((isspace(string[index]) || !string[index]) ? (((!(space & 1)) ? (space | 1) : space)) : ((!(space & (1 << 1))) ? (space | (1 << 1)) : space));

    if ((space & 1) && (space & (1 << 1)))
      count++, space = 0;

    if (!string[index])
      break;
  }

  if (!count || !(fields = (char **)calloc(++count+1, sizeof(char *))))
    return NULL;

  for (count = 0, index = 0;;) {
    while (string[index] && isspace(string[index]))
      index++;

    if (!string[index])
      break;

    characters = index;
    while (string[characters] && !isspace(string[characters]))
      characters++;

    if (!(fields[count] = (char *)calloc(characters+1, sizeof(char))))
      break;

    string_copy(fields[count], &string[index], characters);
    count++;

    index = characters;
  }

  return fields;
}

char *
string_repeat(const char *string, size_t times) {
  size_t string_total_length = string_length(string);
  size_t required_length = string_total_length*times;
  char *repeated = NULL;

  if (!(repeated = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (times--)
    string_concat(repeated, string, string_total_length);

  return repeated;
}

char *
string_copy(char *destination, const char *source, size_t bytes) {
  bytes = string_safelength(source, bytes);

  memcpy(destination, source, bytes);
  destination[bytes] = '\0';

  return destination;
}

char *
string_concat(char *destination, const char *source, size_t bytes) {
  size_t destination_length = string_length(destination);
  bytes = string_safelength(source, bytes);

  memcpy(destination+destination_length, source, bytes);
  destination[destination_length+bytes] = '\0';

  return destination;
}

char *
string_format(const char *format, ...) {
  va_list args;
  int length;

  char *formatted = NULL;

  va_start(args, format);
  if ((length = vsnprintf(NULL, 0, format, args)) < 0)
    return NULL;
  va_end(args);

  if (!(formatted = (char *)calloc((size_t)length+1, sizeof(char))))
    return NULL;

  va_start(args, format);
  vsprintf(formatted, format, args);

  return formatted;
}

char *
string_search(const char *string, const char *substring, bool match_case) {
  const char *match = NULL;

  while ((match = string_char(string, *substring, match_case))) {
    if (string_match(match+sizeof(char), substring+sizeof(char), -1, match_case))
      return (char *)match;

    string = match+sizeof(char);
  }

  return NULL;
}

long
string_find(const char *string, const char *substring, bool match_case) {
  char *match = NULL;

  if (!(match = string_search(string, substring, match_case)))
    return -1;

  return (long)(match-string);
}

char *
string_char(const char *string, char character, bool match_case) {
  while (*string && !string_char_equals(*string, character, match_case))
    string++;

  if (string_char_equals(*string, character, match_case))
    return (char *)string;

  return NULL;
}

size_t
string_count(const char *string, const char *substring, bool match_case) {
  const size_t substring_length = string_length(substring);
  const char *match = NULL;

  size_t count = 0;

  if (!*substring)
    return string_length(string);

  while ((match = string_search(string, substring, match_case))) {
    string = match+substring_length;
    count++;
  }

  return count;
}

char *
string_match(const char *string, const char *substring, long end, bool match_case) {
  size_t limit = ((end < 0) ? string_length(substring) : (size_t)end);

  while (limit && *string && *substring && string_char_equals(*string, *substring, match_case))
    limit--, string++, substring++;

  if ((!*string && !*substring) || !limit)
    return (char *)string;

  return NULL;
}

int
string_compare(const char *string_left, const char *string_right, bool match_case) {
  size_t string_left_length = string_length(string_left);
  size_t string_right_length = string_length(string_right);

  if (string_left_length < string_right_length)
    return -1;
  if (string_left_length > string_right_length)
    return 1;

  while (*string_left && *string_right && string_char_equals(*string_left, *string_right, match_case))
    string_left++, string_right++;

  if (*string_left < *string_right)
    return -1;
  if (*string_left > *string_right)
    return 1;

  return 0;
}

bool
string_equals(const char *string_left, const char *string_right, bool match_case) {
  return string_compare(string_left, string_right, match_case) == 0;
}

bool
string_startswith(const char *string, const char *prefix, bool match_case) {
  return string_match(string, prefix, -1, match_case) != NULL;
}

bool
string_endswith(const char *string, const char *suffix, bool match_case) {
  size_t string_total_length = string_length(string);
  size_t suffix_length = string_length(suffix);
  size_t at;

  if (suffix_length > string_total_length)
    return false;

  at = string_total_length-suffix_length;

  return string_match(string+at, suffix, -1, match_case) != NULL;
}

char *
string_toreverse(char *string) {
  char *string_start = string, *string_end = string+string_length(string)-1;
  char character;

  while (string_start < string_end)
    character = *string_start, *string_start++ = *string_end, *string_end-- = character;

  return string;
}

char *
string_reverse(const char *string) {
  char *reverse = NULL;

  if (!(reverse = string_new(string)))
    return NULL;

  return string_toreverse(reverse);
}

char *
string_toupper(char *string) {
  char *uppercase = string;

  while (*uppercase) {
    *uppercase = (char)toupper(*uppercase);
    uppercase++;
  }

  return string;
}

char *
string_upper(const char *string) {
  char *uppercase = NULL;

  if (!(uppercase = string_new(string)))
    return NULL;

  return string_toupper(uppercase);
}

char *
string_tolower(char *string) {
  char *lowercase = string;

  while (*lowercase) {
    *lowercase = (char)tolower(*lowercase);
    lowercase++;
  }

  return string;
}

char *
string_lower(const char *string) {
  char *lowercase = NULL;

  if (!(lowercase = string_new(string)))
    return NULL;

  return string_tolower(lowercase);
}

char *
string_capitalize(const char *string) {
  size_t required_length = string_length(string);
  char *capitalized = NULL;

  bool capitalize = true;

  char character;
  size_t index = 0;

  if (!(capitalized = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (*string) {
    character = *string;

    if (capitalize && islower(character)) {
      character = toupper(character);
      capitalize = false;
    } else if (!isalpha(character)) {
      capitalize = true;
    } else {
      capitalize = false;
    }

    capitalized[index++] = character;
    string++;
  }

  return capitalized;
}

bool
string_isempty(const char *string) {
  return !*string;
}

char *
string_zero(char *string, size_t zeros) {
  size_t index = 0;

  while (zeros && --zeros)
    string[index++] = '\0';

  return string;
}

char *
string_trimleft(char *string, const char *sequence) {
  size_t string_total_length = string_length(string);
  char *string_left = string;
  size_t offset;

  while (*string_left) {
    size_t byte = 0;

    while (sequence[byte] && *string_left != sequence[byte])
      byte++;
    if (!sequence[byte])
      break;

    string_left++;
  }

  memmove(string, string_left, string_length(string_left));

  offset = (string_total_length-(string_left-string));
  memset(string+offset, 0, string_left-string);

  return string;
}

char *
string_trimright(char *string, const char *sequence) {
  char *string_right = string+string_length(string)-1;

  while (string_right > string) {
    size_t byte = 0;

    while (sequence[byte] && *string_right != sequence[byte])
      byte++;
    if (!sequence[byte])
      break;

    string_right--;
  }

  if (string_right > string)
    memset(string_right+sizeof(char), 0, string_right-string);

  return string;
}

char *
string_trim(char *string, const char *sequence) {
  return string_trimright(string_trimleft(string, sequence), sequence);
}

size_t
string_length(const char *string) {
  const char *string_end = string;

  while (*string_end++);

  return (size_t)(string_end-string-1);
}

char *
string_substr(const char *string, long start, long end) {
  size_t string_total_length = string_length(string);
  size_t slice_length = (size_t)(end-start);
  char *substring = NULL;

  if (start < 0 || end < 0 || start > end /* || start > (long)length || end > (long)length */)
    return NULL;
  if (start > string_total_length || end > string_total_length)
    return NULL;

  if (!(substring = (char *)calloc(slice_length+1, sizeof(char))))
    return NULL;

  string_copy(substring, string+start, string_total_length-slice_length);

  return substring;
}

char **
string_lines(const char *string) {
  size_t count = string_count(string, "\n", true);

  const char *match = NULL;
  char **lines = NULL;

  size_t position, line = 0;

  if (!(lines = (char **)calloc(count, sizeof(char *))))
    return NULL;

  while ((match = string_char(string, '\n', true)) || (match = string_char(string, '\0', true))) {
    position = (size_t)(match-string);

    if (!(lines[line] = (char *)calloc(position+1, sizeof(char))))
      break;

    if (string[position] == '\r')
      position--;

    string_copy(lines[line], string, position);
    line++;

    if (!*match)
      break;

    string = match+sizeof(char);
  }

  return lines;
}

char *
string_new(const char *source) {
  size_t required_length = string_length(source);
  char *string = NULL;

  if (!(string = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  string_copy(string, source, required_length);

  return string;
}
