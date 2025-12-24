#include "strings.h"

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include <stdarg.h>

#include <ctype.h>


char *
string_replace(const char *source, const char *pattern, const char *string, long count, bool sensitive) {
  char *stringA = (char *)source, *stringB = (char *)source, *stringC = NULL, *stringX = NULL;
  char *stringZ = NULL;

  size_t allocation_length, length = string_length(pattern);

  if (count <= 0 && !(count = (long)string_count(source, pattern, sensitive)))
    return string_new(source);

  allocation_length = string_length(source)-(length*(size_t)count)+(string_length(string)*(size_t)count);

  if (!(stringZ = (char *)calloc(allocation_length+1, sizeof(char))))
    return NULL;
  stringX = stringZ;

  length = ((!length) ? sizeof(char) : length);

  for (;;) {
    if ((stringB = string_search(stringB, pattern, sensitive)) && count) {
      while (stringA < stringB)
        *stringX++ = *stringA++;

      for (stringC = (char *)string; *stringC;)
        *stringX++ = *stringC++;

      stringA = (((stringB += length) && !*pattern) ? stringA : stringB);
      count--;

      continue;
    }

    while (*stringA)
      *stringX++ = *stringA++;

    break;
  }

  return stringZ;
}

char **
string_split(const char *source, const char *separator, bool sensitive) {
  char *stringA = (char *)source, *stringB = (char *)((!*separator) ? source+1 : source), *stringX = NULL;
  char **stringZ = NULL;

  size_t count = string_count(source, separator, sensitive), length = string_length(separator);

  if ((*separator && !count) || !count++) {
    if (!(stringZ = (char **)calloc(++count+1, sizeof(char *))))
      return NULL;
    *stringZ = string_new(source);

    return stringZ;
  }
  if (!*separator && !(count = string_length(source)))
    return NULL;

  if (!(stringZ = (char **)calloc(count+1, sizeof(char *))))
    return NULL;

  length = (!(length) ? sizeof(*separator) : length);
  count = 0;

  for (;;) {
    if (!(stringB = string_search(stringB, separator, sensitive)))
      stringB = stringA+string_length(stringA);

    if (!(stringZ[count] = (char *)calloc((size_t)(stringB-stringA)+1, sizeof(char))))
      break;
    stringX = stringZ[count++];

    while (stringA < stringB)
      *stringX++ = *stringA++;

    if (!*stringB)
      break;

    stringA = (((stringB += length) && !*separator) ? stringA : stringB);
  }

  return stringZ;
}

char *
string_join(const char **strings, const char *separator) {
  size_t separator_length = string_length(separator), length = 0, index;
  const char *stringA = NULL;
  char *stringX = NULL, *stringZ = NULL;

  for (index = 0; strings[index]; index++)
    length += string_length(strings[index])+((strings[index+1]) ? separator_length : 0);

  if (!(stringZ = (char *)calloc(length+1, sizeof(char))))
    return NULL;
  stringX = stringZ;

  for (index = 0; strings[index]; index++) {
    for (stringA = strings[index]; *stringA;)
      *stringX++ = *stringA++;

    if (strings[index+1])
      for (stringA = separator; *stringA;)
        *stringX++ = *stringA++;
  }

  return stringZ;
}

char **
string_fields(const char *source) {
  char *stringA = (char *)source, *stringB = NULL, *stringX = NULL;
  char **stringZ = NULL;

  size_t fields = 0;
  uint8_t space = 0;

  for (;; stringA++) {
    // @ identify spaces and fields
    //   1 active bit = sequence of spaces
    //   2 active bits = sequence of a field
    space = ((isspace(*stringA) || !*stringA) ? (((!(space & 1)) ? (space | 1) : space)) : ((!(space & (1 << 1))) ? (space | (1 << 1)) : space));

    if ((space & 1) && (space & (1 << 1)))
      fields++, space = 0;

    if (!*stringA)
      break;
  }

  if (!fields || !(stringZ = (char **)calloc(++fields+1, sizeof(char *))))
    return NULL;

  stringA = (char *)source;
  fields = 0;

  for (;;) {
    while (*stringA && isspace(*stringA))
      stringA++;

    if (!*stringA)
      break;

    stringB = stringA;
    while (*stringB && !isspace(*stringB))
      stringB++;

    if (!(stringZ[fields] = (char *)calloc((size_t)((stringB-stringA)+1), sizeof(char))))
      break;
    stringX = stringZ[fields++];

    while (stringA < stringB)
      *stringX++ = *stringA++;
  }

  return stringZ;
}

char *
string_repeat(const char *source, size_t times) {
  size_t length = string_length(source)*times;
  char *stringZ = NULL;

  if (!(stringZ = (char *)calloc(length+1, sizeof(char))))
    return NULL;

  while (times--)
    string_concat(stringZ, source, length);

  return stringZ;
}

char *
string_write(char *source, const char *string, size_t length) {
  char *stringA = source;

  while (length && (*stringA++ = *string++))
    length--;
  while (length && --length)
    *stringA++ = '\0';

  return source;
}

char *
string_concat(char *source, const char *string, size_t length) {
  char *stringA = source+string_length(source);

  while (*string && length && --length)
    *stringA++ = *string++;

  *stringA = '\0';

  return source;
}

char *
string_format(const char *fmt, ...) {
  va_list args;
  int length;

  char *stringZ = NULL;

  va_start(args, fmt);
  if ((length = vsnprintf(NULL, 0, fmt, args)) < 0)
    return NULL;

  if (!(stringZ = (char *)calloc((size_t)length+1, sizeof(char))))
    return NULL;

  va_start(args, fmt);
  vsprintf(stringZ, fmt, args);

  return stringZ;
}

char *
string_safesearch(const char *source, size_t source_size, const char *string, size_t string_size, bool sensitive) {
  const char *stringA = source, *stringB = NULL;
  size_t sizeA, sizeB;

  if (!source_size || !string_size)
    return (char *)source;

  while (source_size && *source) {
    sizeA = source_size, sizeB = string_size;
    stringA = source, stringB = string;

    if (!sensitive)
      while (sizeA && *stringA && sizeB && *stringB && toupper((unsigned char)*stringA) == toupper((unsigned char)*stringB))
        sizeA--, stringA++, sizeB--, stringB++;
    else
      while (sizeA && *stringA && sizeB && *stringB && *stringA == *stringB)
        sizeA--, stringA++, sizeB--, stringB++;

    if (!sizeB)
      return (char *)source;

    source_size--, source++;
  }

  return NULL;
}

long
string_safefind(const char *source, size_t source_size, const char *string, size_t string_size, bool sensitive) {
  char *stringA = NULL;

  if (!(stringA = string_safesearch(source, source_size, string, string_size, sensitive)))
    return -1;

  return (long)(stringA-source);
}

char *
string_search(const char *source, const char *string, bool sensitive) {
  char *stringA = (char *)source, *stringB = NULL, *stringC = NULL;

  if (!*string)
    return stringA;

  while (*stringA) {
    stringB = stringA, stringC = (char *)string;

    if (!sensitive)
      while (*stringB && *stringC && toupper(*stringB) == toupper(*stringC))
        stringB++, stringC++;
    else
      while (*stringB && *stringC && *stringB == *stringC)
        stringB++, stringC++;

    if (!*stringC)
      return stringA;

    stringA++;
  }

  return NULL;
}

long
string_find(const char *source, const char *string, bool sensitive) {
  char *stringA = NULL;

  if (!(stringA = string_search(source, string, sensitive)))
    return -1;

  return (long)(stringA-source);
}

size_t
string_count(const char *source, const char *string, bool sensitive) {
  size_t count = (!*string), length = string_length(string);
  char *stringA = (char *)source;

  length = (!(length) ? sizeof(*string) : length);

  for (;;) {
    if (!*stringA || !(stringA = string_search(stringA, string, sensitive)))
      break;
    count++, stringA += length;
  }

  return count;
}

char *
string_match(const char *source, const char *string, long end, bool sensitive) {
  size_t length = ((end < 0) ? string_length(string) : (size_t)end);

  const char *stringA = source, *stringB = string;

  if (!sensitive)
    while (length && *stringA && *stringB && toupper(*stringA) == toupper(*stringB))
      length--, stringA++, stringB++;
  else
    while (length && *stringA && *stringB && *stringA == *stringB)
      length--, stringA++, stringB++;

  if ((!*stringA && !*stringB) || !length)
    return (char *)stringA;

  return NULL;
}

int
string_compare(const char *source, const char *string, bool sensitive) {
  const char *stringA = source, *stringB = string;

  if (!sensitive)
    while (*stringA && *stringB && toupper(*stringA) == toupper(*stringB))
      stringA++, stringB++;
  else
    while (*stringA && *stringB && *stringA == *stringB)
      stringA++, stringB++;

  if ((!*stringA && *stringB))
    return -1;
  if ((*stringA && !*stringB))
    return 1;
  if (*stringA < *stringB)
    return -1;
  if (*stringA > *stringB)
    return 1;

  return 0;
}

bool
string_equals(const char *source, const char *string, bool sensitive) {
  return string_compare(source, string, sensitive) == 0;
}

bool
string_lthan(const char *source, const char *string) {
  const char *stringA = source, *stringB = string;

  while (*stringA && *stringB && *stringA == *stringB)
    stringA++, stringB++;

  if (isalpha(*stringA) && isalpha(*stringB))
    return toupper(*stringA) < toupper(*stringB);

  return ((!*stringA && *stringB) || *stringA < *stringB);
}

bool
string_lethan(const char *source, const char *string) {
  if (string_equals(source, string, false))
    return true;

  return string_lthan(source, string);
}

bool
string_gthan(const char *source, const char *string) {
  const char *stringA = source, *stringB = string;

  while (*stringA && *stringB && *stringA == *stringB)
    stringA++, stringB++;

  if (isalpha(*stringA) && isalpha(*stringB))
    return toupper(*stringA) > toupper(*stringB);

  return ((*stringA && !*stringB) || *stringA > *stringB);
}

bool
string_gethan(const char *source, const char *string) {
  if (string_equals(source, string, false))
    return true;

  return string_gthan(source, string);
}

bool
string_startswith(const char *source, const char *string, bool sensitive) {
  size_t lengthA, lengthB;

  if ((lengthB = string_length(string)) > (lengthA = string_length(source)))
    return false;

  return string_match(source, string, (long)lengthB, sensitive) != NULL;
}

bool
string_endswith(const char *source, const char *string, bool sensitive) {
  size_t lengthA, lengthB;

  if ((lengthB = string_length(string)) > (lengthA = string_length(source)))
    return false;

  return string_match(source+(lengthA-lengthB), string, (long)lengthB, sensitive) != NULL;
}

char *
string_toreverse(char *source) {
  char *stringA = source+string_length(source)-1, *stringB = source;
  char character;

  while (stringB < stringA)
    character = *stringB, *stringB++ = *stringA, *stringA-- = character;

  return source;
}

char *
string_reverse(const char *source) {
  char *stringZ = NULL;

  if (!(stringZ = string_new(source)))
    return NULL;

  return string_toreverse(stringZ);
}

char *
string_toupper(char *source) {
  char *stringA = source;

  while (*stringA)
    *stringA = (char)toupper(*stringA), stringA++;

  return source;
}

char *
string_upper(const char *source) {
  char *stringZ = NULL;

  if (!(stringZ = string_new(source)))
    return NULL;

  return string_toupper(stringZ);
}

char *
string_tolower(char *source) {
  char *stringA = source;

  while (*stringA)
    *stringA = (char)tolower(*stringA), stringA++;

  return source;
}

char *
string_lower(const char *source) {
  char *stringZ = NULL;

  if (!(stringZ = string_new(source)))
    return NULL;

  return string_tolower(stringZ);
}

char *
string_capitalize(const char *source) {
  char *stringA = (char *)source, *stringX = NULL;
  char *stringZ = NULL;

  if (!(stringZ = (char *)calloc(string_length(source)+1, sizeof(char))))
    return NULL;
  stringX = stringZ;

  while (*stringA) {
    while (*stringA && !isalnum(*stringA))
      *stringX++ = *stringA++;

    *stringX++ = (char)toupper(*stringA++);

    while (*stringA && isalnum(*stringA))
      *stringX++ = (char)tolower(*stringA++);
  }

  return stringZ;
}

bool
string_hasterminator(const char *string, size_t length) {
  while (length && *string && length--)
    string++;

  return *string == '\0';
}

bool
string_isempty(const char *source) {
  return !*source;
}

char *
string_zero(char *source, size_t length) {
  char *stringA = source;

  while (length && --length)
    *stringA++ = '\0';

  return source;
}

char *
string_chomp(char *source) {
  char *stringA = source+string_length(source)-1;

  while (*stringA && *stringA != '\n')
    stringA--;
  while (*stringA)
    *stringA++ = '\0';

  return source;
}

size_t
string_safelength(const char *source, size_t length) {
  const char *stringA = source;

  while (length && *stringA)
    length--, stringA++;

  return (size_t)(stringA-source-1);
}

size_t
string_length(const char *source) {
  const char *stringA = source;

  while (*stringA++);

  return (size_t)(stringA-source-1);
}

char **
string_substrs(const char *source, const char *pattern_start, const char *pattern_end, bool sensitive) {
  size_t count, pattern_start_length = string_length(pattern_start), pattern_end_length = string_length(pattern_end);
  long start, end;

  char *stringA = (char *)source;
  char **stringZ = NULL;

  if (!*pattern_start && !*pattern_end)
    return string_split(source, "", sensitive);
  if (!*pattern_start || !*pattern_end)
    return NULL;

  for (count = 0; (start = string_find(stringA, pattern_start, sensitive)) != -1 && (end = string_find(stringA, pattern_end, sensitive)) != -1; count++)
    stringA += pattern_end_length+(size_t)(end+string_find(stringA, pattern_start, sensitive));

  if (!count || !(stringZ = (char **)calloc(count+1, sizeof(char *))))
    return NULL;

  stringA = (char *)source;
  count = 0;

  while ((start = string_find(stringA, pattern_start, sensitive)) != -1 && (end = string_find(stringA, pattern_end, sensitive)) != -1) {
    if (!(stringZ[count++] = string_substr(stringA, start+(long)pattern_start_length, end)))
      break;

    stringA += pattern_end_length+(size_t)(end+string_find(stringA, pattern_start, sensitive));
  }

  return stringZ;
}

char *
string_substr(const char *source, long start, long end) {
  char *stringA = (char *)source+start, *stringB = (char *)source+end, *stringX = NULL;
  char *stringZ = NULL;

// size_t length = string_length(source);

  if (start < 0 || end < 0 || start > end /* || start > (long)length || end > (long)length */)
    return NULL;

  if (!(stringZ = (char *)calloc((size_t)(end-start)+1, sizeof(char))))
    return NULL;
  stringX = stringZ;

  while (stringA < stringB)
    *stringX++ = *stringA++;

  return stringZ;
}

char *
string_new(const char *source) {
  size_t length = string_length(source);
  char *string = NULL;

  if (!(string = (char *)calloc(length+1, sizeof(char))))
    return NULL;

  string_write(string, source, length);

  return string;
}
