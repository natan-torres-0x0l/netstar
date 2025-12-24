#ifndef _STRINGS_WSTRINGS_H
#define _STRINGS_WSTRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include <wchar.h>


extern wchar_t *wstring_replace(const wchar_t *, const wchar_t *, const wchar_t *, long, bool);
extern wchar_t **wstring_split(const wchar_t *, const wchar_t *, bool);

extern wchar_t **wstring_fields(const wchar_t *);

extern wchar_t *wstring_repeat(const wchar_t *, size_t);

extern wchar_t *wstring_write(wchar_t *, const wchar_t *, size_t);
extern wchar_t *wstring_concat(wchar_t *, const wchar_t *, size_t);

extern wchar_t *wstring_format(const wchar_t *, ...);

extern wchar_t *wstring_search(const wchar_t *, const wchar_t *, bool);
extern long wstring_find(const wchar_t *, const wchar_t *, bool);

extern wchar_t *wstring_match(const wchar_t *, const wchar_t *, long, bool);

extern int wstring_compare(const wchar_t *, const wchar_t *, bool);
extern bool wstring_equals(const wchar_t *, const wchar_t *, bool);

extern bool wstring_startswith(const wchar_t *, const wchar_t *, bool);
extern bool wstring_endswith(const wchar_t *, const wchar_t *, bool);

extern wchar_t *wstring_reverse(const wchar_t *);
extern wchar_t *wstring_toreverse(wchar_t *);

extern wchar_t *wstring_upper(const wchar_t *);
extern wchar_t *wstring_toupper(wchar_t *);

extern wchar_t *wstring_lower(const wchar_t *);
extern wchar_t *wstring_tolower(wchar_t *);

extern size_t wstring_count(const wchar_t *, const wchar_t *, bool);

extern size_t wstring_length(const wchar_t *);

extern wchar_t *wstring_capitalize(const wchar_t *);

extern wchar_t *wstring_zero(wchar_t *, size_t);

extern wchar_t *wstring_chomp(wchar_t *);

extern wchar_t **wstring_substrs(const wchar_t *, const wchar_t *, const wchar_t *, bool);
extern wchar_t *wstring_substr(const wchar_t *, long, long);

extern wchar_t *wstring_new(const wchar_t *);

#ifdef __cplusplus
}
#endif

#endif
