#ifndef _STRINGS_STRINGS_H
#define _STRINGS_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>


extern char *string_replace(const char *, const char *, const char *, long, bool);
extern char **string_split(const char *, const char *, bool);
extern char *string_join(const char **, const char *);

extern char **string_fields(const char *);

extern char *string_repeat(const char *, size_t);

extern char *string_write(char *, const char *, size_t);
extern char *string_concat(char *, const char *, size_t);

extern char *string_format(const char *, ...);

extern char *string_safesearch(const char *, size_t, const char *, size_t, bool);
extern char *string_search(const char *, const char *, bool);

extern long string_safefind(const char *, size_t, const char *, size_t, bool);
extern long string_find(const char *, const char *, bool);

extern char *string_match(const char *, const char *, long, bool);
extern int string_compare(const char *, const char *, bool);
extern bool string_equals(const char *, const char *, bool);

extern bool string_lthan(const char *, const char *);
extern bool string_lethan(const char *, const char *);

extern bool string_gthan(const char *, const char *);
extern bool string_gethan(const char *, const char *);

extern bool string_startswith(const char *, const char *, bool);
extern bool string_endswith(const char *, const char *, bool);

extern char *string_reverse(const char *);
extern char *string_toreverse(char *);

extern char *string_upper(const char *);
extern char *string_toupper(char *);

extern char *string_lower(const char *);
extern char *string_tolower(char *);

extern size_t string_count(const char *, const char *, bool);

extern size_t string_length(const char *);

extern char *string_capitalize(const char *);

extern bool string_hasterminator(const char *, size_t);
extern bool string_isempty(const char *);

extern char *string_zero(char *, size_t);

extern char *string_chomp(char *);

extern char **string_substrs(const char *, const char *, const char *, bool);
extern char *string_substr(const char *, long, long);

extern char *string_new(const char *);

#ifdef __cplusplus
}
#endif

#endif
