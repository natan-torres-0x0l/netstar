#ifndef _COLLECTIONS_MULTIMAP_MULTIMAP_H
#define _COLLECTIONS_MULTIMAP_MULTIMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct multimap multimap_t;

typedef bool (*multimap_compare_t)(const void *, size_t, const void *, size_t);
typedef bool (*multimap_predicate_t)(const void *, const void *, va_list);

typedef struct multimap_iterator * multimap_iterator_t;
typedef multimap_iterator_t multimap_reverse_iterator_t;


extern multimap_reverse_iterator_t multimap_rnext(multimap_reverse_iterator_t);
extern multimap_reverse_iterator_t multimap_rprev(multimap_reverse_iterator_t);

extern multimap_reverse_iterator_t multimap_rbegin(multimap_t *);
extern multimap_reverse_iterator_t multimap_rend(multimap_t *);

extern multimap_iterator_t multimap_next(multimap_iterator_t);
extern multimap_iterator_t multimap_prev(multimap_iterator_t);

extern multimap_iterator_t multimap_begin(multimap_t *);
extern multimap_iterator_t multimap_end(multimap_t *);

extern collection_value_traits_t multimap_key_traits(multimap_iterator_t);
extern void *multimap_key(multimap_iterator_t);
extern size_t multimap_key_size(multimap_iterator_t);

extern collection_value_traits_t multimap_value_traits(multimap_iterator_t);
extern void *multimap_value(multimap_iterator_t);
extern size_t multimap_value_size(multimap_iterator_t);

#define multimap_lkey    collection_lvalue
#define multimap_clkey   collection_clvalue

#define multimap_rkey    collection_rvalue
#define multimap_crkey   collection_crvalue

#define multimap_lvalue  collection_lvalue
#define multimap_clvalue collection_clvalue

#define multimap_rvalue  collection_rvalue
#define multimap_crvalue collection_crvalue

extern int multimap_insert(multimap_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);

extern void *multimap_get(multimap_t *, const void *, size_t);
extern void *multimap_getor(multimap_t *, const void *, size_t, void *);

extern void *multimap_findif(multimap_t *, multimap_predicate_t, ...);

extern size_t multimap_size(multimap_t *);
extern bool multimap_empty(multimap_t *);

extern collection_value_free_t multimap_key_free(multimap_t *);
extern collection_value_free_t multimap_value_free(multimap_t *);

extern multimap_t *multimap_new(multimap_compare_t, collection_value_free_t, collection_value_free_t);
extern void multimap_free(multimap_t *);

#ifdef __cplusplus
}
#endif

#endif
