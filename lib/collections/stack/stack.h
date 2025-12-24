#ifndef _COLLECTIONS_STACK_STACK_H
#define _COLLECTIONS_STACK_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct stack stack_t;

typedef struct stack_iterator * stack_iterator_t;
typedef stack_iterator_t stack_reverse_iterator_t;


extern stack_reverse_iterator_t stack_rnext(stack_reverse_iterator_t);
extern stack_reverse_iterator_t stack_rprev(stack_reverse_iterator_t);

extern stack_reverse_iterator_t stack_rbegin(stack_t *);
extern stack_reverse_iterator_t stack_rend(stack_t *);

extern stack_iterator_t stack_next(stack_iterator_t);
extern stack_iterator_t stack_prev(stack_iterator_t);

extern stack_iterator_t stack_begin(stack_t *);
extern stack_iterator_t stack_end(stack_t *);

extern collection_value_traits_t stack_value_traits(stack_iterator_t);
extern void *stack_value(stack_iterator_t);
extern size_t stack_value_size(stack_iterator_t);

#define stack_lvalue  collection_lvalue
#define stack_clvalue collection_clvalue

#define stack_rvalue  collection_rvalue
#define stack_crvalue collection_crvalue

extern int stack_push(stack_t *, void *, size_t, bool, bool);
extern void *stack_pop(stack_t *);
extern void *stack_peek(stack_t *);

extern size_t stack_size(stack_t *);
extern bool stack_empty(stack_t *);

extern collection_value_free_t stack_value_free(stack_t *);

extern stack_t *stack_new(collection_value_free_t);
extern void stack_free(stack_t *);

#ifdef __cplusplus
}
#endif

#endif
