#ifndef _COLLECTIONS_RBTREE_RBTREE_H
#define _COLLECTIONS_RBTREE_RBTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct rbtree rbtree_t;

typedef struct rbtree_iterator * rbtree_iterator_t;
typedef rbtree_iterator_t rbtree_reverse_iterator_t;

typedef int (*rbtree_predicate_t)(void *, va_list);


extern rbtree_reverse_iterator_t rbtree_rnext(rbtree_reverse_iterator_t);
extern rbtree_reverse_iterator_t rbtree_rprev(rbtree_reverse_iterator_t);

extern rbtree_reverse_iterator_t rbtree_rbegin(rbtree_t *);
extern rbtree_reverse_iterator_t rbtree_rend(rbtree_t *);

extern rbtree_iterator_t rbtree_next(rbtree_iterator_t);
extern rbtree_iterator_t rbtree_prev(rbtree_iterator_t);

extern rbtree_iterator_t rbtree_begin(rbtree_t *);
extern rbtree_iterator_t rbtree_end(rbtree_t *);

extern collection_value_traits_t rbtree_value_traits(rbtree_iterator_t);
extern void *rbtree_value(rbtree_iterator_t);
extern size_t rbtree_value_size(rbtree_iterator_t);

#define rbtree_lvalue  collection_lvalue
#define rbtree_clvalue collection_clvalue

#define rbtree_rvalue  collection_rvalue
#define rbtree_crvalue collection_crvalue

extern int rbtree_insert(rbtree_t *, void *, size_t, bool, bool);
extern int rbtree_remove(rbtree_t *, const void *, size_t);

// extern void *rbtree_findif(rbtree_t *, collection_predicate_t, ...);
extern void *rbtree_findif(rbtree_t *, rbtree_predicate_t, ...);
extern void *rbtree_find(rbtree_t *, const void *, size_t);

extern size_t rbtree_size(rbtree_t *);
extern bool rbtree_empty(rbtree_t *);

extern collection_compare_t rbtree_value_compare(rbtree_t *);
extern collection_value_free_t rbtree_value_free(rbtree_t *);

extern rbtree_t *rbtree_new(collection_compare_t, collection_value_free_t);
extern void rbtree_free(rbtree_t *);

#ifdef __cplusplus
}
#endif

#endif
