#ifndef _COLLECTIONS_AVLTREE_AVLTREE_H
#define _COLLECTIONS_AVLTREE_AVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct avltree avltree_t;

typedef struct avltree_iterator * avltree_iterator_t;
typedef avltree_iterator_t avltree_reverse_iterator_t;

typedef int (*avltree_predicate_t)(void *, va_list);


extern avltree_reverse_iterator_t avltree_rnext(avltree_reverse_iterator_t);
extern avltree_reverse_iterator_t avltree_rprev(avltree_reverse_iterator_t);

extern avltree_reverse_iterator_t avltree_rbegin(avltree_t *);
extern avltree_reverse_iterator_t avltree_rend(avltree_t *);

extern avltree_iterator_t avltree_next(avltree_iterator_t);
extern avltree_iterator_t avltree_prev(avltree_iterator_t);

extern avltree_iterator_t avltree_begin(avltree_t *);
extern avltree_iterator_t avltree_end(avltree_t *);

extern collection_value_traits_t avltree_value_traits(avltree_iterator_t);
extern void *avltree_value(avltree_iterator_t);
extern size_t avltree_value_size(avltree_iterator_t);

#define avltree_lvalue  collection_lvalue
#define avltree_clvalue collection_clvalue

#define avltree_rvalue  collection_rvalue
#define avltree_crvalue collection_crvalue

extern int avltree_insert(avltree_t *, void *, size_t, bool, bool);
extern int avltree_remove(avltree_t *, const void *, size_t);

// extern void *avltree_findif(avltree_t *, collection_predicate_t, ...);
extern void *avltree_findif(avltree_t *, avltree_predicate_t, ...);
extern void *avltree_find(avltree_t *, const void *, size_t);

extern size_t avltree_size(avltree_t *);
extern bool avltree_empty(avltree_t *);

extern collection_compare_t avltree_value_compare(avltree_t *);
extern collection_value_free_t avltree_value_free(avltree_t *);

extern avltree_t *avltree_new(collection_compare_t, collection_value_free_t);
extern void avltree_free(avltree_t *);

#ifdef __cplusplus
}
#endif

#endif
