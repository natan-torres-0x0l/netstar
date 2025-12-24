#ifndef _COLLECTIONS_LIST_LIST_H
#define _COLLECTIONS_LIST_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct list list_t;

typedef struct list_iterator * list_iterator_t;
typedef list_iterator_t list_reverse_iterator_t;


extern list_reverse_iterator_t list_rnext(list_reverse_iterator_t);
extern list_reverse_iterator_t list_rprev(list_reverse_iterator_t);

extern list_reverse_iterator_t list_rbegin(list_t *);
extern list_reverse_iterator_t list_rend(list_t *);

extern list_iterator_t list_next(list_iterator_t);
extern list_iterator_t list_prev(list_iterator_t);

extern list_iterator_t list_begin(list_t *);
extern list_iterator_t list_end(list_t *);

extern collection_value_traits_t list_value_traits(list_iterator_t);
extern void *list_value(list_iterator_t);
extern size_t list_value_size(list_iterator_t);

#define list_lvalue  collection_lvalue
#define list_clvalue collection_clvalue

#define list_rvalue  collection_rvalue
#define list_crvalue collection_crvalue

extern int list_pushfront(list_t *, void *, size_t, bool, bool);
extern void *list_popfront(list_t *);
extern void *list_front(list_t *);

extern int list_pushback(list_t *, void *, size_t, bool, bool);
extern void *list_popback(list_t *);
extern void *list_back(list_t *);

extern int list_remove(list_t *, void *, size_t);

extern void *list_findif(list_t *, collection_predicate_t, ...);
extern void *list_find(list_t *, const void *, size_t);

extern size_t list_size(list_t *);
extern bool list_empty(list_t *);

extern collection_value_free_t list_value_free(list_t *);

extern list_t *list_new(collection_value_free_t);
extern void list_free(list_t *);

#ifdef __cplusplus
}
#endif

#endif
