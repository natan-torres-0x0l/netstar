#ifndef _COLLECTIONS_BSTREE_BSTREE_H
#define _COLLECTIONS_BSTREE_BSTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct bstree bstree_t;

typedef struct bstree_iterator * bstree_iterator_t;
typedef bstree_iterator_t bstree_reverse_iterator_t;

typedef int (*bstree_predicate_t)(void *, va_list);


extern bstree_reverse_iterator_t bstree_rnext(bstree_reverse_iterator_t);
extern bstree_reverse_iterator_t bstree_rprev(bstree_reverse_iterator_t);

extern bstree_reverse_iterator_t bstree_rbegin(bstree_t *);
extern bstree_reverse_iterator_t bstree_rend(bstree_t *);

extern bstree_iterator_t bstree_next(bstree_iterator_t);
extern bstree_iterator_t bstree_prev(bstree_iterator_t);

extern bstree_iterator_t bstree_begin(bstree_t *);
extern bstree_iterator_t bstree_end(bstree_t *);

extern collection_value_traits_t bstree_value_traits(bstree_iterator_t);
extern void *bstree_value(bstree_iterator_t);
extern size_t bstree_value_size(bstree_iterator_t);

#define bstree_lvalue  collection_lvalue
#define bstree_clvalue collection_clvalue

#define bstree_rvalue  collection_rvalue
#define bstree_crvalue collection_crvalue

extern int bstree_insert(bstree_t *, void *, size_t, bool, bool);
extern int bstree_remove(bstree_t *, const void *, size_t);

// extern void *bstree_findif(bstree_t *, collection_predicate_t, ...);
extern void *bstree_findif(bstree_t *, bstree_predicate_t, ...);
extern void *bstree_find(bstree_t *, const void *, size_t);

extern size_t bstree_size(bstree_t *);
extern bool bstree_empty(bstree_t *);

extern collection_compare_t bstree_value_compare(bstree_t *);
extern collection_value_free_t bstree_value_free(bstree_t *);

extern bstree_t *bstree_new(collection_compare_t, collection_value_free_t);
extern void bstree_free(bstree_t *);

#ifdef __cplusplus
}
#endif

#endif
