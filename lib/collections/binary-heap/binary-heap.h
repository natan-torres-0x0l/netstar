#ifndef _COLLECTIONS_BINARY_HEAP_BINARY_HEAP_H
#define _COLLECTIONS_BINARY_HEAP_BINARY_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct binary_heap binary_heap_t;

typedef bool (*binary_heap_compare_t)(const void *, const void *);

typedef struct binary_heap_iterator * binary_heap_iterator_t;
typedef binary_heap_iterator_t binary_heap_reverse_iterator_t;


extern binary_heap_reverse_iterator_t binary_heap_rnext(binary_heap_reverse_iterator_t);
extern binary_heap_reverse_iterator_t binary_heap_rprev(binary_heap_reverse_iterator_t);

extern binary_heap_reverse_iterator_t binary_heap_rbegin(binary_heap_t *);
extern binary_heap_reverse_iterator_t binary_heap_rend(binary_heap_t *);

extern binary_heap_iterator_t binary_heap_next(binary_heap_iterator_t);
extern binary_heap_iterator_t binary_heap_prev(binary_heap_iterator_t);

extern binary_heap_iterator_t binary_heap_begin(binary_heap_t *);
extern binary_heap_iterator_t binary_heap_end(binary_heap_t *);

extern collection_value_traits_t binary_heap_value_traits(binary_heap_iterator_t);
extern void *binary_heap_value(binary_heap_iterator_t);
extern size_t binary_heap_value_size(binary_heap_iterator_t);

#define binary_heap_lvalue  collection_lvalue
#define binary_heap_clvalue collection_clvalue

#define binary_heap_rvalue  collection_rvalue
#define binary_heap_crvalue collection_crvalue

extern int binary_heap_push(binary_heap_t *, void *, size_t, bool, bool);
extern void *binary_heap_pop(binary_heap_t *);

extern void *binary_heap_peek(binary_heap_t *);

extern size_t binary_heap_size(binary_heap_t *);
extern bool binary_heap_empty(binary_heap_t *);

extern collection_value_free_t binary_heap_value_free(binary_heap_t *);
extern collection_value_free_t binary_heap_priority_free(binary_heap_t *);

extern binary_heap_compare_t binary_heap_value_compare(binary_heap_t *);

extern binary_heap_t *binary_heap_new(binary_heap_compare_t, collection_value_free_t);
extern void binary_heap_free(binary_heap_t *);

#ifdef __cplusplus
}
#endif

#endif
