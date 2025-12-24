#ifndef _COLLECTIONS_QUEUE_QUEUE_H
#define _COLLECTIONS_QUEUE_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct queue queue_t;

typedef struct queue_iterator * queue_iterator_t;
typedef queue_iterator_t queue_reverse_iterator_t;


extern queue_reverse_iterator_t queue_rnext(queue_reverse_iterator_t);
extern queue_reverse_iterator_t queue_rprev(queue_reverse_iterator_t);

extern queue_reverse_iterator_t queue_rbegin(queue_t *);
extern queue_reverse_iterator_t queue_rend(queue_t *);

extern queue_iterator_t queue_next(queue_iterator_t);
extern queue_iterator_t queue_prev(queue_iterator_t);

extern queue_iterator_t queue_begin(queue_t *);
extern queue_iterator_t queue_end(queue_t *);

extern collection_value_traits_t queue_value_traits(queue_iterator_t);
extern void *queue_value(queue_iterator_t);
extern size_t queue_value_size(queue_iterator_t);

#define queue_lvalue  collection_lvalue
#define queue_clvalue collection_clvalue

#define queue_rvalue  collection_rvalue
#define queue_crvalue collection_crvalue

extern int queue_push(queue_t *, void *, size_t, bool, bool);
extern void *queue_pop(queue_t *);

extern void *queue_front(queue_t *);
extern void *queue_back(queue_t *);

extern size_t queue_size(queue_t *);
extern bool queue_empty(queue_t *);

extern collection_value_free_t queue_value_free(queue_t *);

extern queue_t *queue_new(collection_value_free_t);
extern void queue_free(queue_t *);

#ifdef __cplusplus
}
#endif

#endif
