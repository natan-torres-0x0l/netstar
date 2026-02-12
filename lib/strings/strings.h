#ifndef _STRINGS_STRINGS_H
#define _STRINGS_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#define string_char_equals(left, right, match_case) ((match_case) ? (left == right) : (toupper(left) == toupper(right)))

#define string_const(string) string, sizeof(string)

struct string_slice {
  char *self;
  size_t size;
};


extern char *string_safereplace(const char *, size_t, const char *, size_t, const char *, size_t, long, bool);
extern char *string_replace(const char *, const char *, const char *, long, bool);

extern char **string_safesplit(const char *, size_t, const char *, size_t, bool);
extern char **string_split(const char *, const char *, bool);

extern char *string_safejoin(const struct string_slice *, size_t, const char *, size_t);
extern char *string_join(const char **, const char *);

extern char **string_safefields(const char *, size_t);
extern char **string_fields(const char *);

extern char *string_saferepeat(const char *, size_t, size_t);
extern char *string_repeat(const char *, size_t);

extern char *string_safecopy(char *, size_t, const char *, size_t);
extern char *string_copy(char *, const char *, size_t);

extern char *string_safeconcat(char *, size_t, const char *, size_t);
extern char *string_concat(char *, const char *, size_t);

extern char *string_format(const char *, ...);

extern char *string_safesearch(const char *, size_t, const char *, size_t, bool);
extern char *string_search(const char *, const char *, bool);

extern char *string_safechar(const char *, size_t, char, bool);
extern char *string_char(const char *, char, bool);

extern long string_safefind(const char *, size_t, const char *, size_t, bool);
extern long string_find(const char *, const char *, bool);

extern char *string_safematch(const char *, size_t, const char *, size_t, long, bool);
extern char *string_match(const char *, const char *, long, bool);

extern int string_safecompare(const char *, size_t, const char *, size_t, bool);
extern int string_compare(const char *, const char *, bool);

extern bool string_safeequals(const char *, size_t, const char *, size_t, bool);
extern bool string_equals(const char *, const char *, bool);

extern bool string_safestartswith(const char *, size_t, const char *, size_t, bool);
extern bool string_startswith(const char *, const char *, bool);

extern bool string_safeendswith(const char *, size_t, const char *, size_t, bool);
extern bool string_endswith(const char *, const char *, bool);

extern char *string_safereverse(const char *, size_t);
extern char *string_reverse(const char *);

extern char *string_safetoreverse(char *, size_t);
extern char *string_toreverse(char *);

extern char *string_safeupper(const char *, size_t);
extern char *string_upper(const char *);

extern char *string_safetoupper(char *, size_t);
extern char *string_toupper(char *);

extern char *string_safelower(const char *, size_t);
extern char *string_lower(const char *);

extern char *string_safetolower(char *, size_t);
extern char *string_tolower(char *);

extern size_t string_safecount(const char *, size_t, const char *, size_t, bool);
extern size_t string_count(const char *, const char *, bool);

extern size_t string_safelength(const char *, size_t);
extern size_t string_length(const char *);

extern char *string_safecapitalize(const char *, size_t);
extern char *string_capitalize(const char *);

extern bool string_issafe(const char *, size_t);
extern bool string_isempty(const char *);

extern char *string_zero(char *, size_t);

extern char *string_safetrimleft(char *, size_t, const char *, size_t);
extern char *string_safetrimrigh(char *, size_t, const char *, size_t);
extern char *string_safetrim(char *, size_t, const char *, size_t);

extern char *string_trimleft(char *, const char *);
extern char *string_trimright(char *, const char *);
extern char *string_trim(char *, const char *);

extern char **string_substrs(const char *, const char *, const char *, bool);
extern char *string_substr(const char *, long, long);

extern char **string_safelines(const char *, size_t);
extern char **string_lines(const char *);

extern char *string_safenew(const char *, size_t);
extern char *string_new(const char *);

#ifdef __cplusplus
}
#endif

#endif