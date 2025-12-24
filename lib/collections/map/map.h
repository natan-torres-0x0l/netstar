#ifndef _COLLECTIONS_MAP_MAP_H
#define _COLLECTIONS_MAP_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct map map_t;

typedef struct map_iterator * map_iterator_t;
typedef map_iterator_t map_reverse_iterator_t;

typedef int (*map_compare_t)(const void *, size_t, const void *, size_t);
typedef bool (*map_predicate_t)(const void *, const void *, va_list);


extern map_reverse_iterator_t map_rnext(map_reverse_iterator_t);
extern map_reverse_iterator_t map_rprev(map_reverse_iterator_t);

extern map_reverse_iterator_t map_rbegin(map_t *);
extern map_reverse_iterator_t map_rend(map_t *);

extern map_iterator_t map_next(map_iterator_t);
extern map_iterator_t map_prev(map_iterator_t);

extern map_iterator_t map_begin(map_t *);
extern map_iterator_t map_end(map_t *);

extern collection_value_traits_t map_key_traits(map_iterator_t);
extern void *map_key(map_iterator_t);
extern size_t map_key_size(map_iterator_t);

extern collection_value_traits_t map_value_traits(map_iterator_t);
extern void *map_value(map_iterator_t);
extern size_t map_value_size(map_iterator_t);

#define map_lkey    collection_lvalue
#define map_clkey   collection_clvalue

#define map_rkey    collection_rvalue
#define map_crkey   collection_crvalue

#define map_lvalue  collection_lvalue
#define map_clvalue collection_clvalue

#define map_rvalue  collection_rvalue
#define map_crvalue collection_crvalue

extern int map_insert(map_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);
extern int map_remove(map_t *, const void *, size_t);

extern void *map_get(map_t *, const void *, size_t);
extern void *map_getor(map_t *, const void *, size_t, void *);

extern void *map_findif(map_t *, map_predicate_t, ...);

extern size_t map_size(map_t *);
extern bool map_empty(map_t *);

extern collection_value_free_t map_key_free(map_t *);
extern collection_value_free_t map_value_free(map_t *);

extern map_t *map_new(map_compare_t, collection_value_free_t, collection_value_free_t);
extern void map_free(map_t *);

#ifdef __cplusplus
}
#endif

#endif
