#include "priority-queue.h"

#include <collections/binary-heap/binary-heap.h>

#include <stdlib.h>
#include <string.h>

struct priority_queue_node {
  priority_queue_compare_t compare;
  collection_value_t value, priority;
};

struct priority_queue {
  collection_value_free_t value_free, priority_free;
  priority_queue_compare_t compare;
  binary_heap_t *__binary_heap;
};


static bool
priority_queue_node_compare(const void *value1, const void *value2) {
  struct priority_queue_node *node1 = (struct priority_queue_node *)value1;
  struct priority_queue_node *node2 = (struct priority_queue_node *)value2;

  priority_queue_compare_t compare = node1->compare;

  return compare(node1->priority.ptr, node2->priority.ptr);
}

static struct priority_queue_node *
priority_queue_node_new(priority_queue_t *priority_queue, void *value, size_t value_size, bool value_copy, bool value_free, void *priority, size_t priority_size, bool priority_copy, bool priority_free) {
  struct priority_queue_node *node = NULL;

  if (!(node = (struct priority_queue_node *)calloc(1, sizeof(struct priority_queue_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;

  node->priority.ptr = ((node->value.copy = priority_copy) ? collection_value_copy(priority, priority_size) : priority);
  node->priority.size = priority_size;
  node->priority.free = priority_free;

  node->compare = priority_queue->compare;

  return node;
}

static inline void
priority_queue_node_free(priority_queue_t *priority_queue, struct priority_queue_node *node) {
  if (node) {
    if (priority_queue->value_free && node->value.free && node->value.ptr)
      priority_queue->value_free(node->value.ptr), node->value.ptr = NULL;

    if (priority_queue->priority_free && node->priority.free && node->priority.ptr)
      priority_queue->priority_free(node->priority.ptr), node->priority.ptr = NULL;

    free(node);
  }
}

priority_queue_reverse_iterator_t
priority_queue_rnext(priority_queue_reverse_iterator_t iterator) {
  return (priority_queue_reverse_iterator_t)binary_heap_rnext((binary_heap_reverse_iterator_t)iterator);
}

priority_queue_reverse_iterator_t
priority_queue_rprev(priority_queue_reverse_iterator_t iterator) {
  return (priority_queue_reverse_iterator_t)binary_heap_rprev((binary_heap_reverse_iterator_t)iterator);
}

priority_queue_reverse_iterator_t
priority_queue_rbegin(priority_queue_t *priority_queue) {
  return (priority_queue_reverse_iterator_t)binary_heap_rbegin(priority_queue->__binary_heap);
}

priority_queue_reverse_iterator_t
priority_queue_rend(priority_queue_t *priority_queue) {
  return (priority_queue_reverse_iterator_t)binary_heap_rend(priority_queue->__binary_heap);
}

priority_queue_iterator_t
priority_queue_next(priority_queue_iterator_t iterator) {
  return (priority_queue_iterator_t)binary_heap_next((binary_heap_iterator_t)iterator);
}

priority_queue_iterator_t
priority_queue_prev(priority_queue_iterator_t iterator) {
  return (priority_queue_iterator_t)binary_heap_prev((binary_heap_iterator_t)iterator);
}

priority_queue_iterator_t
priority_queue_begin(priority_queue_t *priority_queue) {
  return (priority_queue_iterator_t)binary_heap_begin(priority_queue->__binary_heap);
}

priority_queue_iterator_t
priority_queue_end(priority_queue_t *priority_queue) {
  return (priority_queue_iterator_t)binary_heap_end(priority_queue->__binary_heap);
}

void *
priority_queue_value(priority_queue_iterator_t iterator) {
  return ((iterator) ? ((struct priority_queue_node *)binary_heap_value((binary_heap_iterator_t)iterator))->value.ptr : NULL);
}

size_t
priority_queue_value_size(priority_queue_iterator_t iterator) {
  return ((iterator) ? ((struct priority_queue_node *)binary_heap_value((binary_heap_iterator_t)iterator))->value.size : 0);
}

void *
priority_queue_priority(priority_queue_iterator_t iterator) {
  return ((iterator) ? ((struct priority_queue_node *)binary_heap_value((binary_heap_iterator_t)iterator))->priority.ptr : NULL);
}

size_t
priority_queue_priority_size(priority_queue_iterator_t iterator) {
  return ((iterator) ? ((struct priority_queue_node *)binary_heap_value((binary_heap_iterator_t)iterator))->priority.size : 0);
}

int
priority_queue_push(priority_queue_t *priority_queue, void *value, size_t value_size, bool value_copy, bool value_free, void *priority, size_t priority_size, bool priority_copy, bool priority_free) {
  struct priority_queue_node *node = NULL;
  
  if (!(node = priority_queue_node_new(priority_queue, value, value_size, value_copy, value_free, priority, priority_size, priority_copy, priority_free)))
    return -1;

  return binary_heap_push(priority_queue->__binary_heap, binary_heap_rvalue(node, sizeof(struct priority_queue_node *)));
}

void *
priority_queue_pop(priority_queue_t *priority_queue) {
  struct priority_queue_node *node = NULL;
  void *value = NULL;
  
  if (!(node = (struct priority_queue_node *)binary_heap_pop(priority_queue->__binary_heap)))
    return NULL;

  value = node->value.ptr;
  free(node);

  return value;
}

void *
priority_queue_peek(priority_queue_t *priority_queue) {
  struct priority_queue_node *node = NULL;
  
  if (!(node = (struct priority_queue_node *)binary_heap_peek(priority_queue->__binary_heap)))
    return NULL;

  return node->value.ptr;
}

size_t
priority_queue_size(priority_queue_t *priority_queue) {
  return binary_heap_size(priority_queue->__binary_heap);
}

bool
priority_queue_empty(priority_queue_t *priority_queue) {
  return binary_heap_empty(priority_queue->__binary_heap);
}

priority_queue_compare_t
priority_queue_value_compare(priority_queue_t *priority_queue) {
  return priority_queue->compare;
}

collection_value_free_t
priority_queue_value_free(priority_queue_t *priority_queue) {
  return priority_queue->value_free;
}

priority_queue_t *
priority_queue_new(priority_queue_compare_t compare, collection_value_free_t value_free, collection_value_free_t priority_free) {
  priority_queue_t *priority_queue = NULL;

  if (!(priority_queue = (priority_queue_t *)calloc(1, sizeof(priority_queue_t))))
    goto _return;

  if (!(priority_queue->__binary_heap = binary_heap_new(priority_queue_node_compare, NULL)))
    goto _return;

  priority_queue->compare = compare;
  priority_queue->value_free = value_free;
  priority_queue->priority_free = priority_free;

  return priority_queue;

_return:
  priority_queue_free(priority_queue);
  return NULL;
}

void
priority_queue_free(priority_queue_t *priority_queue) {
  if (priority_queue) {
    if (priority_queue->__binary_heap) {
      binary_heap_iterator_t iterator = NULL;

      for (iterator = binary_heap_begin(priority_queue->__binary_heap); iterator; iterator = binary_heap_next(iterator))
        priority_queue_node_free(priority_queue, (struct priority_queue_node *)binary_heap_value(iterator));

      binary_heap_free(priority_queue->__binary_heap);
    }

    free(priority_queue);
  }
}
