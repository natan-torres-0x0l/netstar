#include "strings.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <stdint.h>
#include <stdarg.h>

#include <ctype.h>


char *
string_safereplace(const char *string, size_t string_size, const char *pattern, size_t pattern_size, const char *substring, size_t substring_size, long count, bool match_case) {
  size_t substring_length = string_safelength(substring, substring_size);
  size_t pattern_length = string_safelength(pattern, pattern_size);
  size_t required_length;

  const char *match = NULL;
  size_t position;

  char *replaced = NULL;
  size_t index = 0;

  if (count <= 0 && !(count = (long)string_safecount(string, string_size, pattern, pattern_size, match_case)))
    return string_safenew(string, string_size);

  required_length = string_safelength(string, string_size)-(pattern_length*(size_t)count)+(substring_length*(size_t)count);

  if (!(replaced = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (count && (match = string_safesearch(string, string_size, pattern, pattern_size, match_case))) {
    position = match-string;

    string_safecopy(&replaced[index], required_length-index, string, position);
    index += position;

    string = match+pattern_length;
    string_size -= position;

    string_safecopy(&replaced[index], required_length-index, substring, substring_length);
    index += substring_length;

    count--;
  }

  if (string_size && *string)
    string_safecopy(&replaced[index], required_length-index, string, string_size);

  return replaced;
}

char **
string_safesplit(const char *string, size_t string_size, const char *separator, size_t separator_size, bool match_case) {
  size_t count = string_safecount(string, string_size, separator, separator_size, match_case);
  size_t separator_length = string_safelength(separator, separator_size);
  size_t position;

  const char *match = string;
  char **strings = NULL;

  if ((*separator && !count) || !count) {
    if (!(strings = (char **)calloc(++count+1, sizeof(char *))))
      return NULL;
    *strings = string_safenew(string, string_size);

    return strings;
  }
  if (!*separator && !(count = string_safelength(string, string_size)))
    return NULL;

  if (!(strings = (char **)calloc(++count+1, sizeof(char *))))
    return NULL;

  count = 0;

  for (;;) {
    if (separator_length && !(match = string_safesearch(string, string_size, separator, separator_size, match_case)))
      match = string+string_safelength(string, string_size);
    else if (!separator_length)
      match = string+sizeof(char);

    position = match-string;

    if (!(strings[count] = (char *)calloc(position+1, sizeof(char))))
      break;

    string_safecopy(strings[count], position, string, position);
    count++;

    if (!(string_size-separator_length+position) || !*match)
      break;

    string += separator_length+position;
    string_size -= separator_length+position;
  }

  return strings;
}

char *
string_safejoin(const struct string_slice *strings, size_t strings_count, const char *separator, size_t separator_size) {
  size_t separator_length = string_safelength(separator, separator_size);
  size_t required_length = 0, index;

  char *string = NULL;

  for (index = 0; index < strings_count; index++)
    required_length += string_safelength(strings[index].self, strings[index].size)+((index+1 < strings_count) ? separator_length : 0);

  if (!(string = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  for (index = 0; index < strings_count; index++) {
    string_safeconcat(string, required_length, strings[index].self, string_safelength(strings[index].self, strings[index].size));

    if (index+1 < strings_count)
      string_safeconcat(string, required_length, separator, separator_length);
  }

  return string;
}

char **
string_safefields(const char *string, size_t string_size) {
  size_t count, index, characters;
  uint8_t space = 0;

  char **fields = NULL;

  for (count = 0, index = 0;; index++) {
    // @ identify spaces and fields
    //   1 active bit = sequence of spaces
    //   2 active bits = sequence of a field
    space = ((isspace(string[index]) || index >= string_size || !string[index]) ? (((!(space & 1)) ? (space | 1) : space)) : ((!(space & (1 << 1))) ? (space | (1 << 1)) : space));

    if ((space & 1) && (space & (1 << 1)))
      count++, space = 0;

    if (index >= string_size && !string[index])
      break;
  }

  if (!count || !(fields = (char **)calloc(++count+1, sizeof(char *))))
    return NULL;

  for (count = 0, index = 0;;) {
    while (index < string_size && string[index] && isspace(string[index]))
      index++;

    if (index >= string_size || !string[index])
      break;

    characters = index;
    while (characters < string_size && string[characters] && !isspace(string[characters]))
      characters++;

    if (!(fields[count] = (char *)calloc(characters+1, sizeof(char))))
      break;

    string_safecopy(fields[count], characters, &string[index], characters);
    count++;

    printf(" %d - %d\n", string_size, characters);
    printf(" %d\n", string_size);
 
    string_size -= characters;
    index = characters;
  }

  return fields;
}

char *
string_saferepeat(const char *string, size_t string_size, size_t times) {
  size_t string_total_length = string_safelength(string, string_size);
  size_t required_length = string_total_length*times;
  char *repeated = NULL;

  if (!(repeated = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (times--)
    string_safeconcat(repeated, required_length, string, string_total_length);

  return repeated;
}

char *
string_safecopy(char *destination, size_t destination_size, const char *source, size_t source_size) {
  size_t source_length = string_safelength(source, source_size);
  size_t bytes = ((source_length > destination_size) ? destination_size : source_length);

  memcpy(destination, source, bytes);
  destination[bytes] = '\0';

  return destination;
}

char *
string_safeconcat(char *destination, size_t destination_size, const char *source, size_t source_size) {
  size_t destination_length = string_safelength(destination, destination_size);
  size_t source_length = string_safelength(source, source_size);
  size_t capacity, bytes;

  if (!(capacity = destination_size-destination_length))
    return destination;

  bytes = ((source_length > capacity) ? capacity : source_length);

  memcpy(destination+destination_length, source, bytes);
  destination[destination_length+bytes] = '\0';

  return destination;
}

char *
string_safesearch(const char *string, size_t string_size, const char *substring, size_t substring_size, bool match_case) {
  const char *match = NULL;

  while ((match = string_safechar(string, string_size, *substring, match_case))) {
    if (string_safematch(match+sizeof(char), string_size-(match-string), substring+sizeof(char), substring_size-sizeof(char), -1, match_case))
      return (char *)match;

    string = match+sizeof(char);
  }

  return NULL;
}

long
string_safefind(const char *string, size_t string_size, const char *substring, size_t substring_size, bool match_case) {
  char *first = NULL;

  if (!(first = string_safesearch(string, string_size, substring, substring_size, match_case)))
    return -1;

  return (long)(first-string);
}

char *
string_safechar(const char *string, size_t string_size, char character, bool match_case) {
  while (string_size && *string && !string_char_equals(*string, character, match_case))
    string++, string_size--;

  if (string_char_equals(*string, character, match_case))
    return (char *)string;

  return NULL;
}

size_t
string_safecount(const char *string, size_t string_size, const char *substring, size_t substring_size, bool match_case) {
  const size_t substring_length = string_safelength(substring, substring_size);
  const char *match = NULL;

  size_t count = 0;

  if (!substring_length || !*substring)
    return string_safelength(string, string_size);

  while ((match = string_safesearch(string, string_size, substring, substring_size, match_case))) {
    string = match+substring_length;
    string_size -= (match-string)+substring_length;

    count++;
  }

  return count;
}

char *
string_safematch(const char *string, size_t string_size, const char *substring, size_t substring_size, long end, bool match_case) {
  size_t limit = ((end < 0) ? string_safelength(substring, substring_size) : (size_t)end);

  while (limit && *string && string_size && *substring && substring_size && string_char_equals(*string, *substring, match_case))
    limit--, string_size--, string++, substring_size--, substring++;

  if ((!*string && !*substring) || !limit)
    return (char *)string;

  return NULL;
}

int
string_safecompare(const char *string_left, size_t string_left_size, const char *string_right, size_t string_right_size, bool match_case) {
  size_t string_left_length = string_safelength(string_left, string_left_size);
  size_t string_right_length = string_safelength(string_right, string_right_size);

  if (string_left_length < string_right_length)
    return -1;
  if (string_left_length > string_right_length)
    return 1;

  while (string_left_size && *string_left && string_right_size && *string_right && string_char_equals(*string_left, *string_right, match_case))
    string_left_size--, string_left++, string_right_size--, string_right++;

  if (*string_left < *string_right)
    return -1;
  if (*string_left > *string_right)
    return 1;

  return 0;
}

bool
string_safeequals(const char *string_left, size_t string_left_size, const char *string_right, size_t string_right_size, bool match_case) {
  return string_safecompare(string_left, string_left_size, string_right, string_right_size, match_case) == 0;
}

bool
string_safestartswith(const char *string, size_t string_size, const char *prefix, size_t prefix_size, bool match_case) {
  return string_safematch(string, string_size, prefix, prefix_size, -1, match_case) != NULL;
}

bool
string_safeendswith(const char *string, size_t string_size, const char *suffix, size_t suffix_size, bool match_case) {
  size_t string_total_length = string_safelength(string, string_size);
  size_t suffix_length = string_safelength(suffix, suffix_size);
  size_t at;

  if (suffix_length > string_total_length)
    return false;

  at = string_total_length-suffix_length;

  return string_safematch(string+at, string_size, suffix, suffix_size, -1, match_case) != NULL;
}

char *
string_safetoreverse(char *string, size_t string_size) {
  char *start = string, *end = string+string_safelength(string, string_size)-1;
  char character;

  while (start < end)
    character = *start, *start++ = *end, *end-- = character;

  return string;
}

char *
string_safereverse(const char *string, size_t string_size) {
  char *reverse = NULL;

  if (!(reverse = string_safenew(string, string_size)))
    return NULL;

  return string_safetoreverse(reverse, string_size);
}

char *
string_safetoupper(char *string, size_t string_size) {
  char *uppercase = string;

  while (string_size && *uppercase)
    string_size--, *uppercase = (char)toupper(*uppercase), uppercase++;

  return string;
}

char *
string_safeupper(const char *string, size_t string_size) {
  char *uppercase = NULL;

  if (!(uppercase = string_safenew(string, string_size)))
    return NULL;

  return string_safetoupper(uppercase, string_size);
}

char *
string_safetolower(char *string, size_t string_size) {
  char *lowercase = string;

  while (string_size && *lowercase)
    string_size--, *lowercase = (char)tolower(*lowercase), lowercase++;

  return string;
}

char *
string_safelower(const char *string, size_t string_size) {
  char *lowercase = NULL;

  if (!(lowercase = string_safenew(string, string_size)))
    return NULL;

  return string_safetolower(lowercase, string_size);
}

char *
string_safecapitalize(const char *string, size_t string_size) {
  size_t required_length = string_safelength(string, string_size);
  char *capitalized = NULL;

  bool capitalize = true;

  char character;
  size_t index = 0;

  if (!(capitalized = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  while (string_size && *string) {
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
    string_size--, string++;
  }

  return capitalized;
}

bool
string_issafe(const char *string, size_t string_size) {
  while (string_size && *string)
    string_size--, string++;

  return *string == '\0';
}

size_t
string_safelength(const char *string, size_t string_size) {
  const char *terminator = NULL;

  if (!(terminator = (const char *)memchr(string, '\0', string_size)))
    return string_size;

  return (size_t)(terminator-string);
}

char *
string_safetrimleft(char *string, size_t string_size, const char *sequence, size_t sequence_size) {
  size_t string_total_length = string_safelength(string, string_size);
  char *string_left = string;
  size_t offset;

  while (*string_left) {
    size_t byte = 0;

    while (byte < sequence_size && sequence[byte] && *string_left != sequence[byte])
      byte++;
    if (byte >= sequence_size || !sequence[byte])
      break;

    string_left++;
  }

  offset = (string_total_length-(string_left-string));

  memmove(string, string_left, string_safelength(string_left, offset));
  memset(string+offset, 0, string_left-string);

  return string;
}

char *
string_safetrimright(char *string, size_t string_size, const char *sequence, size_t sequence_size) {
  char *string_right = string+string_safelength(string, string_size)-1;

  while (string_right > string) {
    size_t byte = 0;

    while (byte < sequence_size && sequence[byte] && *string_right != sequence[byte])
      byte++;
    if (byte >= sequence_size || !sequence[byte])
      break;

    string_right--;
  }

  if (string_right > string)
    memset(string_right+sizeof(char), 0, string_right-string);

  return string;
}

char *
string_safetrim(char *string, size_t string_size, const char *sequence, size_t sequence_size) {
  return string_safetrimright(string_safetrimleft(string, string_size, sequence, sequence_size), string_size, sequence, sequence_size);
}

char *
string_safesubstr(const char *string, size_t string_size, long start, long end) {
  size_t string_total_length = string_safelength(string, string_size);
  size_t required_length = (size_t)(end-start);
  char *substring = NULL;

  if (start > (long)string_total_length || end > (long)string_total_length)
    return NULL;

  if (start < 0 || end < 0 || start > end)
    return NULL;

  if (!(substring = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  string_safecopy(substring, required_length, string+start, string_total_length-start);

  return substring;
}

char **
string_safelines(const char *string, size_t string_size) {
  size_t count = string_safecount(string, string_size, string_const("\n"), true);

  const char *match = NULL;
  char **lines = NULL;

  size_t position, line = 0;

  if (!(lines = (char **)calloc(count, sizeof(char *))))
    return NULL;

  while ((match = string_safechar(string, string_size, '\n', true)) || (match = string_safechar(string, string_size, '\0', true))) {
    position = (size_t)(match-string);

    if (!(lines[line] = (char *)calloc(position+1, sizeof(char))))
      break;

    if (string[position] == '\r')
      position--;

    string_copy(lines[line], string, position);
    line++;

    if (!*match)
      break;

    string_size -= match-string;
    string = match+sizeof(char);
  }

  return lines;
}

char *
string_safenew(const char *source, size_t source_size) {
  size_t required_length = string_safelength(source, source_size);
  char *string = NULL;

  if (!(string = (char *)calloc(required_length+1, sizeof(char))))
    return NULL;

  string_safecopy(string, required_length, source, required_length);

  return string;
}
