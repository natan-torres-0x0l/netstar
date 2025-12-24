#ifndef _COLLECTIONS_MULTISET_MULTISET_H
#define _COLLECTIONS_MULTISET_MULTISET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct multiset multiset_t;

typedef bool (*multiset_compare_t)(const void *, const void *);

typedef struct multiset_iterator * multiset_iterator_t;
typedef multiset_iterator_t multiset_reverse_iterator_t;


extern multiset_reverse_iterator_t multiset_rnext(multiset_reverse_iterator_t);
extern multiset_reverse_iterator_t multiset_rprev(multiset_reverse_iterator_t);

extern multiset_reverse_iterator_t multiset_rbegin(multiset_t *);
extern multiset_reverse_iterator_t multiset_rend(multiset_t *);

extern multiset_iterator_t multiset_next(multiset_iterator_t);
extern multiset_iterator_t multiset_prev(multiset_iterator_t);

extern multiset_iterator_t multiset_begin(multiset_t *);
extern multiset_iterator_t multiset_end(multiset_t *);

extern collection_value_traits_t multiset_value_traits(multiset_iterator_t);
extern void *multiset_value(multiset_iterator_t);
extern size_t multiset_value_size(multiset_iterator_t);

#define multiset_lvalue  collection_lvalue
#define multiset_clvalue collection_clvalue

#define multiset_rvalue  collection_rvalue
#define multiset_crvalue collection_crvalue

extern int multiset_insert(multiset_t *, void *, size_t, bool, bool);

extern size_t multiset_size(multiset_t *);
extern bool multiset_empty(multiset_t *);

extern multiset_compare_t multiset_value_compare(multiset_t *);
extern collection_value_free_t multiset_value_free(multiset_t *);

extern multiset_t *multiset_new(multiset_compare_t, collection_value_free_t);
extern void multiset_free(multiset_t *);

#ifdef __cplusplus
}
#endif

#endif
