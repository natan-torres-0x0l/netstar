#include "queue.h"

#include <collections/list/list.h>

#include <stdlib.h>
#include <string.h>

struct queue {
  collection_value_free_t value_free;
  list_t *__list;
};


queue_reverse_iterator_t
queue_rnext(queue_reverse_iterator_t iterator) {
  return (queue_reverse_iterator_t)list_next((list_iterator_t)iterator);
}

queue_reverse_iterator_t
queue_rprev(queue_reverse_iterator_t iterator) {
  return (queue_reverse_iterator_t)list_prev((list_iterator_t)iterator);
}

queue_reverse_iterator_t
queue_rbegin(queue_t *queue) {
  return (queue_reverse_iterator_t)list_begin(queue->__list);
}

queue_reverse_iterator_t
queue_rend(queue_t *queue) {
  return (queue_reverse_iterator_t)list_end(queue->__list);
}

queue_iterator_t
queue_next(queue_iterator_t iterator) {
  return (queue_iterator_t)list_rnext((list_reverse_iterator_t)iterator);
}

queue_iterator_t
queue_prev(queue_iterator_t iterator) {
  return (queue_iterator_t)list_rprev((list_reverse_iterator_t)iterator);
}

queue_iterator_t
queue_begin(queue_t *queue) {
  return (queue_iterator_t)list_rbegin(queue->__list);
}

queue_iterator_t
queue_end(queue_t *queue) {
  return (queue_iterator_t)list_rend(queue->__list);
}

void *
queue_value(queue_iterator_t iterator) {
  return list_value((list_iterator_t)iterator);
}

size_t
queue_value_size(queue_iterator_t iterator) {
  return list_value_size((list_iterator_t)iterator);
}

int
queue_push(queue_t *queue, void *value, size_t size, bool copy, bool free) {
  return list_pushfront(queue->__list, value, size, copy, free);
}

void *
queue_pop(queue_t *queue) {
  return list_popback(queue->__list);
}

void *
queue_front(queue_t *queue) {
  return list_back(queue->__list);
}

void *
queue_back(queue_t *queue) {
  return list_front(queue->__list);
}

size_t
queue_size(queue_t *queue) {
  return list_size(queue->__list);
}

bool
queue_empty(queue_t *queue) {
  return list_empty(queue->__list);
}

collection_value_free_t
queue_value_free(queue_t *queue) {
  return queue->value_free;
}

queue_t *
queue_new(collection_value_free_t value_free) {
  queue_t *queue = NULL;

  if (!(queue = (queue_t *)calloc(1, sizeof(queue_t))))
    goto _return;

  if (!(queue->__list = list_new(value_free)))
    goto _return;

  queue->value_free = value_free;

  return queue;

_return:
  queue_free(queue);
  return NULL;
}

void
queue_free(queue_t *queue) {
  if (queue) {
    if (queue->__list)
      list_free(queue->__list);

    free(queue);
  }
}
