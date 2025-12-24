#ifndef _COLLECTIONS_HASHMAP_HASHMAP_H
#define _COLLECTIONS_HASHMAP_HASHMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct hashmap hashmap_t;

typedef struct hashmap_iterator * hashmap_iterator_t;
typedef hashmap_iterator_t hashmap_reverse_iterator_t;

typedef bool (*hashmap_predicate_t)(const void *, const void *, va_list);


extern hashmap_reverse_iterator_t hashmap_rnext(hashmap_reverse_iterator_t);
extern hashmap_reverse_iterator_t hashmap_rprev(hashmap_reverse_iterator_t);

extern hashmap_reverse_iterator_t hashmap_rbegin(hashmap_t *);
extern hashmap_reverse_iterator_t hashmap_rend(hashmap_t *);

extern hashmap_iterator_t hashmap_next(hashmap_iterator_t);
extern hashmap_iterator_t hashmap_prev(hashmap_iterator_t);

extern hashmap_iterator_t hashmap_begin(hashmap_t *);
extern hashmap_iterator_t hashmap_end(hashmap_t *);

extern collection_value_traits_t hashmap_key_traits(hashmap_iterator_t);
extern void *hashmap_key(hashmap_iterator_t);
extern size_t hashmap_key_size(hashmap_iterator_t);

extern collection_value_traits_t hashmap_value_traits(hashmap_iterator_t);
extern void *hashmap_value(hashmap_iterator_t);
extern size_t hashmap_value_size(hashmap_iterator_t);

#define hashmap_lkey    collection_lvalue
#define hashmap_clkey   collection_clvalue

#define hashmap_rkey    collection_rvalue
#define hashmap_crkey   collection_crvalue

#define hashmap_lvalue  collection_lvalue
#define hashmap_clvalue collection_clvalue

#define hashmap_rvalue  collection_rvalue
#define hashmap_crvalue collection_crvalue

extern int hashmap_insert(hashmap_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);
extern int hashmap_remove(hashmap_t *, const void *, size_t);

extern void *hashmap_get(hashmap_t *, const void *, size_t);
extern void *hashmap_getor(hashmap_t *, const void *, size_t, void *);

extern void *hashmap_findif(hashmap_t *, hashmap_predicate_t, ...);

extern size_t hashmap_size(hashmap_t *);
extern bool hashmap_empty(hashmap_t *);

extern collection_value_free_t hashmap_key_free(hashmap_t *);
extern collection_value_free_t hashmap_value_free(hashmap_t *);

extern hashmap_t *hashmap_new(collection_value_free_t, collection_value_free_t);
extern void hashmap_free(hashmap_t *);

#ifdef __cplusplus
}
#endif

#endif
