#include "wstrings.h"

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include <stdarg.h>

#include <wctype.h>


wchar_t *
wstring_replace(const wchar_t *source, const wchar_t *string, const wchar_t *newstring, long nstrings, bool sensitive) {
  wchar_t *stringA = (wchar_t *)source, *stringB = (wchar_t *)source, *stringC = NULL, *stringX = NULL;
  wchar_t *stringZ = NULL;

  size_t newlength, length = wstring_length(string);

  if (nstrings <= 0 && !(nstrings = (long)wstring_count(source, string, sensitive)))
    return wstring_new(source);

  newlength = wstring_length(source)-(length*(size_t)nstrings)+wstring_length(newstring)*(size_t)nstrings;

  if (!(stringZ = (wchar_t *)calloc(newlength+1, sizeof(wchar_t))))
    return NULL;
  stringX = stringZ;

  length = (!(length) ? sizeof(*string) : length);

  for (;;) {
    if ((stringB = wstring_search(stringB, string, sensitive)) && nstrings) {
      while (stringA < stringB)
        *stringX++ = *stringA++;

      for (stringC = (wchar_t *)newstring; *stringC;)
        *stringX++ = *stringC++;

      stringA = (((stringB += length) && !*string) ? stringA : stringB);
      nstrings--;

      continue;
    }

    while (*stringA)
      *stringX++ = *stringA++;

    break;
  }

  return stringZ;
}

wchar_t **
wstring_split(const wchar_t *source, const wchar_t *string, bool sensitive) {
  wchar_t *stringA = (wchar_t *)source, *stringB = (wchar_t *)((!*string) ? source+1 : source), *stringX = NULL;
  wchar_t **stringZ = NULL;

  size_t nstrings, length = wstring_length(string);

  if ((*string && !(nstrings = wstring_count(source, string, sensitive))) || !nstrings++) {
    if (!(stringZ = (wchar_t **)calloc(++nstrings+1, sizeof(wchar_t *))))
      return NULL;
    *stringZ = wstring_new(source);

    return stringZ;
  }
  if (!*string && !(nstrings = wstring_length(source)))
    return NULL;

  if (!(stringZ = (wchar_t **)calloc(nstrings+1, sizeof(wchar_t *))))
    return NULL;

  length = (!(length) ? sizeof(*string) : length);
  nstrings = 0;

  for (;;) {
    if (!(stringB = wstring_search(stringB, string, sensitive)))
      stringB = stringA+wstring_length(stringA);

    if (!(stringZ[nstrings] = (wchar_t *)calloc((size_t)(stringB-stringA)+1, sizeof(wchar_t))))
      break;
    stringX = stringZ[nstrings++];

    while (stringA < stringB)
      *stringX++ = *stringA++;

    if (!*stringB)
      break;

    stringA = (((stringB += length) && !*string) ? stringA : stringB);
  }

  return stringZ;
}

wchar_t **
wstring_fields(const wchar_t *source) {
  wchar_t *stringA = (wchar_t *)source, *stringB = NULL, *stringX = NULL;
  wchar_t **stringZ = NULL;

  size_t nstrings = 0;
  uint8_t space = 0;

  for (;; stringA++) {
    space = ((iswspace(*stringA) || !*stringA) ? (((!(space & 1)) ? (space | 1) : space)) : ((!(space & (1 << 1))) ? (space | (1 << 1)) : space));

    if ((space & 1) && (space & (1 << 1)))
      nstrings++, space = 0;

    if (!*stringA)
      break;
  }

  if (!nstrings || !(stringZ = (wchar_t **)calloc(++nstrings+1, sizeof(wchar_t *))))
    return NULL;

  stringA = (wchar_t *)source;
  nstrings = 0;

  for (;;) {
    while (*stringA && iswspace(*stringA))
      stringA++;

    if (!*stringA)
      break;

    stringB = stringA;
    while (*stringB && !iswspace(*stringB))
      stringB++;

    if (!(stringZ[nstrings] = (wchar_t *)calloc((size_t)((stringB-stringA)+1), sizeof(wchar_t))))
      break;
    stringX = stringZ[nstrings++];

    while (stringA < stringB)
      *stringX++ = *stringA++;
  }

  return stringZ;
}

wchar_t *
wstring_repeat(const wchar_t *source, size_t times) {
  size_t length = wstring_length(source)*times;
  wchar_t *stringZ = NULL;

  if (!(stringZ = (wchar_t *)calloc(length+1, sizeof(wchar_t))))
    return NULL;

  while (times--)
    wstring_concat(stringZ, source, length);

  return stringZ;
}

wchar_t *
wstring_write(wchar_t *source, const wchar_t *string, size_t length) {
  wchar_t *stringA = source;

  while (length && (*stringA++ = *string++))
    length--;
  while (length && --length)
    *stringA++ = '\0';

  return source;
}

wchar_t *
wstring_concat(wchar_t *source, const wchar_t *string, size_t length) {
  wchar_t *stringA = source+wstring_length(source);

  while (*string && length && --length)
    *stringA++ = *string++;

  *stringA = '\0';

  return source;
}

wchar_t *
wstring_format(const wchar_t *fmt, ...) {
  va_list args;
  int length;

  FILE *fnull = NULL;

  wchar_t *stringZ = NULL;

  va_start(args, fmt);

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  if (!(fnull = fopen("NUL", "wb")))
    return NULL;

#elif defined __linux__ || defined __linux || defined __unix__ || defined __unix
  if (!(fnull = fopen("/dev/null", "wb")))
    return NULL;

#endif

  if (!fnull || (length = vfwprintf(fnull, fmt, args)) < 0)
    return NULL;

  fclose(fnull);

  if (!(stringZ = (wchar_t *)calloc((size_t)++length+1, sizeof(wchar_t))))
    return NULL;

  vswprintf(stringZ, (size_t)length, fmt, args);

  return stringZ;
}


wchar_t *
wstring_search(const wchar_t *source, const wchar_t *string, bool sensitive) {
  wchar_t *stringA = (wchar_t *)source, *stringB = NULL, *stringC = NULL;

  if (!*string)
    return stringA;

  while (*stringA) {
    stringB = stringA, stringC = (wchar_t *)string;

    if (!sensitive)
      while (*stringB && *stringC && towupper(*stringB) == towupper(*stringC))
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
wstring_find(const wchar_t *source, const wchar_t *string, bool sensitive) {
  wchar_t *stringA = NULL;

  if (!(stringA = wstring_search(source, string, sensitive)))
    return -1;

  return (long)(stringA-source);
}

size_t
wstring_count(const wchar_t *source, const wchar_t *string, bool sensitive) {
  size_t count = (!*string), length = wstring_length(string);
  wchar_t *stringA = (wchar_t *)source;

  length = (!(length) ? sizeof(*string) : length);

  for (;;) {
    if (!*stringA || !(stringA = wstring_search(stringA, string, sensitive)))
      break;
    count++, stringA += length;
  }

  return count;
}

wchar_t *
wstring_match(const wchar_t *source, const wchar_t *string, long end, bool sensitive) {
  size_t length = ((end < 0) ? wstring_length(string) : (size_t)end);

  const wchar_t *stringA = source, *stringB = string;

  if (!sensitive)
    while (length && *stringA && *stringB && towupper(*stringA) == towupper(*stringB))
      length--, stringA++, stringB++;
  else
    while (length && *stringA && *stringB && *stringA == *stringB)
      length--, stringA++, stringB++;

  if ((!*stringA && !*stringB) || !length)
    return (wchar_t *)stringA;

  return NULL;
}

int
wstring_compare(const wchar_t *source, const wchar_t *string, bool sensitive) {
  const wchar_t *stringA = source, *stringB = string;

  if (!sensitive)
    while (*stringA && *stringB && towupper(*stringA) == towupper(*stringB))
      stringA++, stringB++;
  else
    while (*stringA && *stringB && *stringA == *stringB)
      stringA++, stringB++;

  if ((!*stringA && *stringB) || (*stringA && !*stringB))
    return -1;
  if (!*stringA && !*stringB)
    return 1;

  return 0;
}

bool
wstring_equals(const wchar_t *source, const wchar_t *string, bool sensitive) {
  return wstring_compare(source, string, sensitive) == 1;
}

bool
wstring_startswith(const wchar_t *source, const wchar_t *string, bool sensitive) {
  size_t lengthA, lengthB;

  if ((lengthB = wstring_length(string)) > (lengthA = wstring_length(source)))
    return false;

  return wstring_match(source, string, (long)lengthB, sensitive) != NULL;
}

bool
wstring_endswith(const wchar_t *source, const wchar_t *string, bool sensitive) {
  size_t lengthA, lengthB;

  if ((lengthB = wstring_length(string)) > (lengthA = wstring_length(source)))
    return false;

  return wstring_match(source+(lengthA-lengthB), string, (long)lengthB, sensitive) != NULL;
}

wchar_t *
wstring_toreverse(wchar_t *source) {
  wchar_t *stringA = source+wstring_length(source)-1, *stringB = source;
  wchar_t wchar_tacter;

  while (stringB < stringA)
    wchar_tacter = *stringB, *stringB++ = *stringA, *stringA-- = wchar_tacter;

  return source;
}

wchar_t *
wstring_reverse(const wchar_t *source) {
  wchar_t *stringZ = NULL;

  if (!(stringZ = wstring_new(source)))
    return NULL;

  return wstring_toreverse(stringZ);
}

wchar_t *
wstring_towupper(wchar_t *source) {
  wchar_t *stringA = source;

  while (*stringA)
    *stringA = (wchar_t)towupper(*stringA), stringA++;

  return source;
}

wchar_t *
wstring_upper(const wchar_t *source) {
  wchar_t *stringZ = NULL;

  if (!(stringZ = wstring_new(source)))
    return NULL;

  return wstring_towupper(stringZ);
}

wchar_t *
wstring_towlower(wchar_t *source) {
  wchar_t *stringA = source;

  while (*stringA)
    *stringA = (wchar_t)towlower(*stringA), stringA++;

  return source;
}

wchar_t *
wstring_lower(const wchar_t *source) {
  wchar_t *stringZ = NULL;

  if (!(stringZ = wstring_new(source)))
    return NULL;

  return wstring_towlower(stringZ);
}

wchar_t *
wstring_capitalize(const wchar_t *source) {
  wchar_t *stringA = (wchar_t *)source, *stringX = NULL;
  wchar_t *stringZ = NULL;

  if (!(stringZ = (wchar_t *)calloc(wstring_length(source)+1, sizeof(wchar_t))))
    return NULL;
  stringX = stringZ;

  while (*stringA) {
    while (*stringA && !iswalnum(*stringA))
      *stringX++ = *stringA++;

    *stringX++ = (wchar_t)towupper(*stringA++);

    while (*stringA && iswalnum(*stringA))
      *stringX++ = (wchar_t)towlower(*stringA++);
  }

  return stringZ;
}

wchar_t *
wstring_zero(wchar_t *source, size_t length) {
  wchar_t *stringA = source;

  while (length && --length)
    *stringA++ = '\0';

  return source;
}

wchar_t *
wstring_chomp(wchar_t *source) {
  wchar_t *stringA = source+wstring_length(source)-1;

  while (*stringA && *stringA != '\n')
    stringA--;
  while (*stringA)
    *stringA++ = '\0';

  return source;
}

size_t
wstring_length(const wchar_t *source) {
  const wchar_t *stringA = source;

  while (*stringA++);

  return (size_t)(stringA-source-1);
}

wchar_t **
wstring_substrs(const wchar_t *source, const wchar_t *startstring, const wchar_t *endstring, bool sensitive) {
  size_t nstrings, startlength = wstring_length(startstring), endlength = wstring_length(endstring);
  long start, end;

  wchar_t *stringA = (wchar_t *)source;
  wchar_t **stringZ = NULL;

  if (!*startstring && !*endstring)
    return wstring_split(source, L"", sensitive);
  if (!*startstring || !*endstring)
    return NULL;

  for (nstrings = 0; (start = wstring_find(stringA, startstring, sensitive)) != -1 && (end = wstring_find(stringA, endstring, sensitive)) != -1; nstrings++)
    stringA += endlength+(size_t)(end+wstring_find(stringA, startstring, sensitive));

  if (!nstrings && !(stringZ = (wchar_t **)calloc(nstrings+1, sizeof(wchar_t *))))
    return NULL;

  stringA = (wchar_t *)source;
  nstrings = 0;

  while ((start = wstring_find(stringA, startstring, sensitive)) != -1 && (end = wstring_find(stringA, endstring, sensitive)) != -1) {
    if (!(stringZ[nstrings++] = wstring_substr(stringA, start+(long)startlength, end)))
      break;

    stringA += endlength+(size_t)(end+wstring_find(stringA, startstring, sensitive));
  }

  return stringZ;
}

wchar_t *
wstring_substr(const wchar_t *source, long start, long end) {
  wchar_t *stringA = (wchar_t *)source+start, *stringB = (wchar_t *)source+end, *stringX = NULL;
  wchar_t *stringZ = NULL;

  size_t length = wstring_length(source);

  if (start < 0 || end < 0 || start > (long)length || end > (long)length || start > end)
    return NULL;

  if (!(stringZ = (wchar_t *)calloc((size_t)(end-start)+1, sizeof(wchar_t))))
    return NULL;
  stringX = stringZ;

  while (stringA < stringB)
    *stringX++ = *stringA++;

  return stringZ;
}

wchar_t *
wstring_new(const wchar_t *source) {
  size_t length = wstring_length(source);
  wchar_t *string = NULL;

  if (!(string = (wchar_t *)calloc(length+1, sizeof(wchar_t))))
    return NULL;

  wstring_write(string, source, length);

  return string;
}
