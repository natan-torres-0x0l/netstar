#ifndef _COLLECTIONS_DEQUE_DEQUE_H
#define _COLLECTIONS_DEQUE_DEQUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct deque deque_t;

typedef struct deque_iterator * deque_iterator_t;
typedef deque_iterator_t deque_reverse_iterator_t;


extern deque_reverse_iterator_t deque_rnext(deque_reverse_iterator_t);
extern deque_reverse_iterator_t deque_rprev(deque_reverse_iterator_t);

extern deque_reverse_iterator_t deque_rbegin(deque_t *);
extern deque_reverse_iterator_t deque_rend(deque_t *);

extern deque_iterator_t deque_next(deque_iterator_t);
extern deque_iterator_t deque_prev(deque_iterator_t);

extern deque_iterator_t deque_begin(deque_t *);
extern deque_iterator_t deque_end(deque_t *);

extern collection_value_traits_t deque_value_traits(deque_iterator_t);
extern void *deque_value(deque_iterator_t);
extern size_t deque_value_size(deque_iterator_t);

#define deque_lvalue  collection_lvalue
#define deque_clvalue collection_clvalue

#define deque_rvalue  collection_rvalue
#define deque_crvalue collection_crvalue

extern int deque_pushfront(deque_t *, void *, size_t, bool, bool);
extern void *deque_popfront(deque_t *);
extern void *deque_front(deque_t *);

extern int deque_pushback(deque_t *, void *, size_t, bool, bool);
extern void *deque_popback(deque_t *);
extern void *deque_back(deque_t *);

extern void *deque_at(deque_t *, size_t);

extern void *deque_findif(deque_t *, collection_predicate_t, ...);
extern void *deque_find(deque_t *, const void *, size_t);

extern size_t deque_size(deque_t *);
extern bool deque_empty(deque_t *);

extern collection_value_free_t deque_value_free(deque_t *);

extern deque_t *deque_new(collection_value_free_t);
extern void deque_free(deque_t *);

#ifdef __cplusplus
}
#endif

#endif
