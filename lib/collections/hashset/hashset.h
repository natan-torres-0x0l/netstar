#ifndef _COLLECTIONS_HASHSET_HASHSET_H
#define _COLLECTIONS_HASHSET_HASHSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct hashset hashset_t;

typedef struct hashset_iterator * hashset_iterator_t;
typedef hashset_iterator_t hashset_reverse_iterator_t;


extern hashset_reverse_iterator_t hashset_rnext(hashset_reverse_iterator_t);
extern hashset_reverse_iterator_t hashset_rprev(hashset_reverse_iterator_t);

extern hashset_reverse_iterator_t hashset_rbegin(hashset_t *);
extern hashset_reverse_iterator_t hashset_rend(hashset_t *);

extern hashset_iterator_t hashset_next(hashset_iterator_t);
extern hashset_iterator_t hashset_prev(hashset_iterator_t);

extern hashset_iterator_t hashset_begin(hashset_t *);
extern hashset_iterator_t hashset_end(hashset_t *);

extern collection_value_traits_t hashset_value_traits(hashset_iterator_t);
extern void *hashset_value(hashset_iterator_t);
extern size_t hashset_value_size(hashset_iterator_t);

#define hashset_lvalue  collection_lvalue
#define hashset_clvalue collection_clvalue

#define hashset_rvalue  collection_rvalue
#define hashset_crvalue collection_crvalue

extern int hashset_insert(hashset_t *, void *, size_t, bool, bool);
extern int hashset_remove(hashset_t *, const void *, size_t);

extern void *hashset_findif(hashset_t *, collection_predicate_t, ...);
extern void *hashset_find(hashset_t *, const void *, size_t);

extern size_t hashset_size(hashset_t *);
extern bool hashset_empty(hashset_t *);

extern collection_value_free_t hashset_value_free(hashset_t *);

extern hashset_t *hashset_new(collection_value_free_t);
extern void hashset_free(hashset_t *);

#ifdef __cplusplus
}
#endif

#endif
