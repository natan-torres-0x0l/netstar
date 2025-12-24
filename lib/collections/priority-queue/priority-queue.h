#ifndef _COLLECTIONS_PRIORITY_QUEUE_PRIORITY_QUEUE_H
#define _COLLECTIONS_PRIORITY_QUEUE_PRIORITY_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct priority_queue priority_queue_t;

typedef bool (*priority_queue_compare_t)(const void *, const void *);

typedef struct priority_queue_iterator * priority_queue_iterator_t;
typedef priority_queue_iterator_t priority_queue_reverse_iterator_t;


extern priority_queue_reverse_iterator_t priority_queue_rnext(priority_queue_reverse_iterator_t);
extern priority_queue_reverse_iterator_t priority_queue_rprev(priority_queue_reverse_iterator_t);

extern priority_queue_reverse_iterator_t priority_queue_rbegin(priority_queue_t *);
extern priority_queue_reverse_iterator_t priority_queue_rend(priority_queue_t *);

extern priority_queue_iterator_t priority_queue_next(priority_queue_iterator_t);
extern priority_queue_iterator_t priority_queue_prev(priority_queue_iterator_t);

extern priority_queue_iterator_t priority_queue_begin(priority_queue_t *);
extern priority_queue_iterator_t priority_queue_end(priority_queue_t *);

extern collection_value_traits_t priority_queue_value_traits(priority_queue_iterator_t);
extern void *priority_queue_value(priority_queue_iterator_t);
extern size_t priority_queue_value_size(priority_queue_iterator_t);

extern collection_value_traits_t priority_queue_priority_traits(priority_queue_iterator_t);
extern void *priority_queue_priority(priority_queue_iterator_t);
extern size_t priority_queue_priority_size(priority_queue_iterator_t);

#define priority_queue_lvalue  collection_lvalue
#define priority_queue_clvalue collection_clvalue

#define priority_queue_rvalue  collection_rvalue
#define priority_queue_crvalue collection_crvalue

extern int priority_queue_push(priority_queue_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);
extern void *priority_queue_pop(priority_queue_t *);

extern void *priority_queue_peek(priority_queue_t *);

extern size_t priority_queue_size(priority_queue_t *);
extern bool priority_queue_empty(priority_queue_t *);

extern collection_value_free_t priority_queue_value_free(priority_queue_t *);

extern priority_queue_compare_t priority_queue_value_compare(priority_queue_t *);

extern priority_queue_t *priority_queue_new(priority_queue_compare_t, collection_value_free_t, collection_value_free_t);
extern void priority_queue_free(priority_queue_t *);

#ifdef __cplusplus
}
#endif

#endif
