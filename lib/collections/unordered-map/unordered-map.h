#ifndef _COLLECTIONS_UNORDERED_MAP_UNORDERED_MAP_H
#define _COLLECTIONS_UNORDERED_MAP_UNORDERED_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct unordered_map unordered_map_t;

typedef struct unordered_map_iterator * unordered_map_iterator_t;
typedef unordered_map_iterator_t unordered_map_reverse_iterator_t;

typedef bool (*unordered_map_predicate_t)(const void *, const void *, va_list);


extern unordered_map_reverse_iterator_t unordered_map_rnext(unordered_map_reverse_iterator_t);
extern unordered_map_reverse_iterator_t unordered_map_rprev(unordered_map_reverse_iterator_t);

extern unordered_map_reverse_iterator_t unordered_map_rbegin(unordered_map_t *);
extern unordered_map_reverse_iterator_t unordered_map_rend(unordered_map_t *);

extern unordered_map_iterator_t unordered_map_next(unordered_map_iterator_t);
extern unordered_map_iterator_t unordered_map_prev(unordered_map_iterator_t);

extern unordered_map_iterator_t unordered_map_begin(unordered_map_t *);
extern unordered_map_iterator_t unordered_map_end(unordered_map_t *);

extern collection_value_traits_t unordered_map_key_traits(unordered_map_iterator_t);
extern void *unordered_map_key(unordered_map_iterator_t);
extern size_t unordered_map_key_size(unordered_map_iterator_t);

extern collection_value_traits_t unordered_map_value_traits(unordered_map_iterator_t);
extern void *unordered_map_value(unordered_map_iterator_t);
extern size_t unordered_map_value_size(unordered_map_iterator_t);

#define unordered_map_lkey    collection_lvalue
#define unordered_map_clkey   collection_clvalue

#define unordered_map_rkey    collection_rvalue
#define unordered_map_crkey   collection_crvalue

#define unordered_map_lvalue  collection_lvalue
#define unordered_map_clvalue collection_clvalue

#define unordered_map_rvalue  collection_rvalue
#define unordered_map_crvalue collection_crvalue

extern int unordered_map_insert(unordered_map_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);
extern int unordered_map_remove(unordered_map_t *, const void *, size_t);

extern void *unordered_map_get(unordered_map_t *, const void *, size_t);
extern void *unordered_map_getor(unordered_map_t *, const void *, size_t, void *);

extern void *unordered_map_findif(unordered_map_t *, unordered_map_predicate_t, ...);

extern size_t unordered_map_size(unordered_map_t *);
extern bool unordered_map_empty(unordered_map_t *);

extern collection_value_free_t unordered_map_key_free(unordered_map_t *);
extern collection_value_free_t unordered_map_value_free(unordered_map_t *);

extern unordered_map_t *unordered_map_new(collection_value_free_t, collection_value_free_t);
extern void unordered_map_free(unordered_map_t *);

#ifdef __cplusplus
}
#endif

#endif
