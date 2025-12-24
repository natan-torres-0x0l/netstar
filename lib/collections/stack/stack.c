#include "stack.h"

#include <collections/list/list.h>

#include <stdlib.h>
#include <string.h>

struct stack {
  collection_value_free_t value_free;
  list_t *__list;
};


stack_reverse_iterator_t
stack_rnext(stack_reverse_iterator_t iterator) {
  return (stack_reverse_iterator_t)list_rnext((list_reverse_iterator_t)iterator);
}

stack_reverse_iterator_t
stack_rprev(stack_reverse_iterator_t iterator) {
  return (stack_reverse_iterator_t)list_rprev((list_reverse_iterator_t)iterator);
}

stack_reverse_iterator_t
stack_rbegin(stack_t *stack) {
  return (stack_reverse_iterator_t)list_rbegin(stack->__list);
}

stack_reverse_iterator_t
stack_rend(stack_t *stack) {
  return (stack_reverse_iterator_t)list_rend(stack->__list);
}

stack_iterator_t
stack_next(stack_iterator_t iterator) {
  return (stack_iterator_t)list_next((list_iterator_t)iterator);
}

stack_iterator_t
stack_prev(stack_iterator_t iterator) {
  return (stack_iterator_t)list_prev((list_iterator_t)iterator);
}

stack_iterator_t
stack_begin(stack_t *stack) {
  return (stack_iterator_t)list_begin(stack->__list);
}

stack_iterator_t
stack_end(stack_t *stack) {
  return (stack_iterator_t)list_end(stack->__list);
}

void *
stack_value(stack_iterator_t iterator) {
  return list_value((list_iterator_t)iterator);
}

size_t
stack_value_size(stack_iterator_t iterator) {
  return list_value_size((list_iterator_t)iterator);
}

int
stack_push(stack_t *stack, void *value, size_t size, bool copy, bool free) {
  return list_pushfront(stack->__list, value, size, copy, free);
}

void *
stack_pop(stack_t *stack) {
  return list_popfront(stack->__list);
}

void *
stack_peek(stack_t *stack) {
  return list_front(stack->__list);
}

size_t
stack_size(stack_t *stack) {
  return list_size(stack->__list);
}

bool
stack_empty(stack_t *stack) {
  return list_empty(stack->__list);
}

collection_value_free_t
stack_value_free(stack_t *stack) {
  return stack->value_free;
}

stack_t *
stack_new(collection_value_free_t value_free) {
  stack_t *stack = NULL;

  if (!(stack = (stack_t *)calloc(1, sizeof(stack_t))))
    goto _return;

  if (!(stack->__list = list_new(value_free)))
    goto _return;

  stack->value_free = value_free;

  return stack;

_return:
  stack_free(stack);
  return NULL;
}

void
stack_free(stack_t *stack) {
  if (stack) {
    if (stack->__list)
      list_free(stack->__list);

    free(stack);
  }
}
