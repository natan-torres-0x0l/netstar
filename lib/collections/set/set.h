#ifndef _COLLECTIONS_SET_SET_H
#define _COLLECTIONS_SET_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct set set_t;

typedef struct set_iterator * set_iterator_t;
typedef set_iterator_t set_reverse_iterator_t;


extern set_reverse_iterator_t set_rnext(set_reverse_iterator_t);
extern set_reverse_iterator_t set_rprev(set_reverse_iterator_t);

extern set_reverse_iterator_t set_rbegin(set_t *);
extern set_reverse_iterator_t set_rend(set_t *);

extern set_iterator_t set_next(set_iterator_t);
extern set_iterator_t set_prev(set_iterator_t);

extern set_iterator_t set_begin(set_t *);
extern set_iterator_t set_end(set_t *);

extern collection_value_traits_t set_value_traits(set_iterator_t);
extern void *set_value(set_iterator_t);
extern size_t set_value_size(set_iterator_t);

#define set_lvalue  collection_lvalue
#define set_clvalue collection_clvalue

#define set_rvalue  collection_rvalue
#define set_crvalue collection_crvalue

extern int set_insert(set_t *, void *, size_t, bool, bool);
extern int set_remove(set_t *, const void *, size_t);

extern void *set_findif(set_t *, collection_predicate_t, ...);
extern void *set_find(set_t *, const void *, size_t);

extern size_t set_size(set_t *);
extern bool set_empty(set_t *);

extern collection_compare_t set_value_compare(set_t *);
extern collection_value_free_t set_value_free(set_t *);

extern set_t *set_new(collection_compare_t, collection_value_free_t);
extern void set_free(set_t *);

#ifdef __cplusplus
}
#endif

#endif
