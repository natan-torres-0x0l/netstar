#ifndef _COLLECTIONS_UNORDERED_SET_UNORDERED_SET_H
#define _COLLECTIONS_UNORDERED_SET_UNORDERED_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct unordered_set unordered_set_t;

typedef struct unordered_set_iterator * unordered_set_iterator_t;
typedef unordered_set_iterator_t unordered_set_reverse_iterator_t;


extern unordered_set_reverse_iterator_t unordered_set_rnext(unordered_set_reverse_iterator_t);
extern unordered_set_reverse_iterator_t unordered_set_rprev(unordered_set_reverse_iterator_t);

extern unordered_set_reverse_iterator_t unordered_set_rbegin(unordered_set_t *);
extern unordered_set_reverse_iterator_t unordered_set_rend(unordered_set_t *);

extern unordered_set_iterator_t unordered_set_next(unordered_set_iterator_t);
extern unordered_set_iterator_t unordered_set_prev(unordered_set_iterator_t);

extern unordered_set_iterator_t unordered_set_begin(unordered_set_t *);
extern unordered_set_iterator_t unordered_set_end(unordered_set_t *);

extern collection_value_traits_t unordered_set_value_traits(unordered_set_iterator_t);
extern void *unordered_set_value(unordered_set_iterator_t);
extern size_t unordered_set_value_size(unordered_set_iterator_t);

#define unordered_set_lvalue  collection_lvalue
#define unordered_set_clvalue collection_clvalue

#define unordered_set_rvalue  collection_rvalue
#define unordered_set_crvalue collection_crvalue

extern int unordered_set_insert(unordered_set_t *, void *, size_t, bool, bool);
extern int unordered_set_remove(unordered_set_t *, const void *, size_t);

extern void *unordered_set_findif(unordered_set_t *, collection_predicate_t, ...);

extern size_t unordered_set_size(unordered_set_t *);
extern bool unordered_set_empty(unordered_set_t *);

extern collection_value_free_t unordered_set_value_free(unordered_set_t *);

extern unordered_set_t *unordered_set_new(collection_value_free_t);
extern void unordered_set_free(unordered_set_t *);

#ifdef __cplusplus
}
#endif

#endif
