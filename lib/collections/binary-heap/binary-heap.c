#include "binary-heap.h"

#include <stdlib.h>
#include <string.h>

#define BINARY_HEAP_INITIAL_CAPACITY 10

struct binary_heap_node {
  struct binary_heap_node **nodes;
  binary_heap_t *binary_heap;

  collection_value_t value;
};

struct binary_heap {
  struct binary_heap_node **begin, **end;
  struct binary_heap_node **nodes;
  size_t capacity, size;

  binary_heap_compare_t compare;
  collection_value_free_t value_free;
};


static inline bool
binary_heap_requires_reallocation(binary_heap_t *binary_heap) {
  return binary_heap->size == binary_heap->capacity;
}

static inline size_t
binary_heap_capacity_growth(size_t size) {
  return size*2;
}

static inline int
binary_heap_reallocate(binary_heap_t *binary_heap, size_t capacity) {
  struct binary_heap_node **nodes = NULL;

  if (!(nodes = (struct binary_heap_node **)realloc(binary_heap->nodes, capacity*sizeof(struct binary_heap_node *))))
    return -1;
  memset(nodes+binary_heap->capacity, 0, capacity*sizeof(struct binary_heap_node *));
  binary_heap->nodes = nodes;

  binary_heap->capacity = capacity;
  return 1;
}

static inline int
binary_heap_reallocate_capacity(binary_heap_t *binary_heap) {
  return binary_heap_reallocate(binary_heap, binary_heap_capacity_growth(binary_heap->size));
}

static struct binary_heap_node *
binary_heap_node_new(binary_heap_t *binary_heap, void *value, size_t size, bool copy, bool free) {
  struct binary_heap_node *node = NULL;

  if (!(node = (struct binary_heap_node *)calloc(1, sizeof(struct binary_heap_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  node->binary_heap = binary_heap;

  return node;
}

static inline void
binary_heap_node_free(binary_heap_t *binary_heap, struct binary_heap_node *node) {
  if (node) {
    if (binary_heap->value_free && node->value.free && node->value.ptr)
      binary_heap->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

binary_heap_reverse_iterator_t
binary_heap_rnext(binary_heap_reverse_iterator_t iterator) {
  struct binary_heap_node **node = (struct binary_heap_node **)iterator;
  return ((iterator && node != (*node)->binary_heap->begin) ? (binary_heap_iterator_t)(node-1) : NULL);
}

binary_heap_reverse_iterator_t
binary_heap_rprev(binary_heap_reverse_iterator_t iterator) {
  struct binary_heap_node **node = (struct binary_heap_node **)iterator;
  return ((iterator && node != (*node)->binary_heap->end) ? (binary_heap_iterator_t)(node+1) : NULL);
}

binary_heap_reverse_iterator_t
binary_heap_rbegin(binary_heap_t *binary_heap) {
  return (binary_heap_iterator_t)binary_heap->end;
}

binary_heap_reverse_iterator_t
binary_heap_rend(binary_heap_t *binary_heap) {
  return (binary_heap_iterator_t)binary_heap->begin;
}

binary_heap_iterator_t
binary_heap_next(binary_heap_iterator_t iterator) {
  struct binary_heap_node **node = (struct binary_heap_node **)iterator;
  return ((iterator && node != (*node)->binary_heap->end) ? (binary_heap_iterator_t)(node+1) : NULL);
}

binary_heap_iterator_t
binary_heap_prev(binary_heap_iterator_t iterator) {
  struct binary_heap_node **node = (struct binary_heap_node **)iterator;
  return ((iterator && node != (*node)->binary_heap->begin) ? (binary_heap_iterator_t)(node-1) : NULL);
}

binary_heap_iterator_t
binary_heap_begin(binary_heap_t *binary_heap) {
  return (binary_heap_iterator_t)binary_heap->begin;
}

binary_heap_iterator_t
binary_heap_end(binary_heap_t *binary_heap) {
  return (binary_heap_iterator_t)binary_heap->end;
}

void *
binary_heap_value(binary_heap_iterator_t iterator) {
  return ((iterator) ? (*(struct binary_heap_node **)iterator)->value.ptr : NULL);
}

size_t
binary_heap_value_size(binary_heap_iterator_t iterator) {
  return ((iterator) ? (*(struct binary_heap_node **)iterator)->value.size : 0);
}

static inline void
binary_heap_node_swap(struct binary_heap_node **__x, struct binary_heap_node **__y) {
  struct binary_heap_node *__z = *__x;

  *__x = *__y;
  *__y = __z;
}

static inline size_t
binary_heap_node_parent(size_t __x) {
  return ((__x - 1) / 2);
}

static inline size_t
binary_heap_node_left(size_t __x) {
  return ((__x * 2) + 1);
}

static inline size_t
binary_heap_node_right(size_t __x) {
  return ((__x * 2) + 2);
}

int
binary_heap_push(binary_heap_t *binary_heap, void *value, size_t size, bool copy, bool free) {
  size_t __x, __y;

  if (binary_heap_requires_reallocation(binary_heap) && binary_heap_reallocate_capacity(binary_heap) == -1)
    return -1;

  __x = binary_heap->size;

  if (!(binary_heap->nodes[__x] = binary_heap_node_new(binary_heap, value, size, copy, free)))
    return -1;

  binary_heap->size++;

  for (;;) {
    __y = binary_heap_node_parent(__x);

    if (!(__x && !binary_heap->compare(binary_heap->nodes[__y]->value.ptr, binary_heap->nodes[__x]->value.ptr)))
      break;

    binary_heap_node_swap(&binary_heap->nodes[__x], &binary_heap->nodes[__y]);
    __x = __y;
  }

  if (binary_heap->begin) {
    binary_heap->begin = &binary_heap->nodes[0];
    binary_heap->end = &binary_heap->nodes[binary_heap->size-1];

    return 1;
  }

  binary_heap->begin = binary_heap->end = &binary_heap->nodes[0];
  return 1;
}

static inline void
binary_heap_heapify(binary_heap_t *binary_heap, size_t __w) {
  size_t __x, __y, __z = __w;

  for (;;) {
    __x = binary_heap_node_left(__w);
    __y = binary_heap_node_right(__w);

    if (__x < binary_heap->size && binary_heap->compare(binary_heap->nodes[__x]->value.ptr, binary_heap->nodes[__z]->value.ptr))
      __z = __x;
    if (__y < binary_heap->size && binary_heap->compare(binary_heap->nodes[__y]->value.ptr, binary_heap->nodes[__z]->value.ptr))
      __z = __y;

    if (__z != __w) {
      binary_heap_node_swap(&binary_heap->nodes[__w], &binary_heap->nodes[__z]);
      __w = __z;

      continue;
    }

    break;
  }
}

void *
binary_heap_pop(binary_heap_t *binary_heap) {
  void *value = NULL;

  if (*binary_heap->nodes) {
    value = (*binary_heap->nodes)->value.ptr;
    
    free(*binary_heap->nodes);
    *binary_heap->nodes = NULL;

    binary_heap->size--;

    if (binary_heap->size) {
      *binary_heap->nodes = binary_heap->nodes[binary_heap->size];
      binary_heap_heapify(binary_heap, 0);
    }

    binary_heap->begin = &binary_heap->nodes[0];
    binary_heap->end = &binary_heap->nodes[binary_heap->size-1];
  }

  return value;
}

void *
binary_heap_peek(binary_heap_t *binary_heap) {
  return ((*binary_heap->nodes) ? (*binary_heap->nodes)->value.ptr : NULL);
}

size_t
binary_heap_size(binary_heap_t *binary_heap) {
  return binary_heap->size;
}

bool
binary_heap_empty(binary_heap_t *binary_heap) {
  return !*binary_heap->nodes;
}

collection_value_free_t
binary_heap_value_free(binary_heap_t *binary_heap) {
  return binary_heap->value_free;
}

binary_heap_compare_t
binary_heap_value_compare(binary_heap_t *binary_heap) {
  return binary_heap->compare;
}

binary_heap_t *
binary_heap_new(binary_heap_compare_t compare, collection_value_free_t value_free) {
  binary_heap_t *binary_heap = NULL;

  if (!(binary_heap = (binary_heap_t *)calloc(1, sizeof(struct binary_heap))))
    goto _return;

  binary_heap->capacity = BINARY_HEAP_INITIAL_CAPACITY;

  if (!(binary_heap->nodes = (struct binary_heap_node **)calloc(binary_heap->capacity, sizeof(struct binary_heap_node *))))
    goto _return;

  binary_heap->compare = compare;
  binary_heap->value_free = value_free;

  return binary_heap;

_return:
  binary_heap_free(binary_heap);
  return NULL;
}

void
binary_heap_free(binary_heap_t *binary_heap) {
  if (binary_heap) {
    if (binary_heap->nodes) {
      size_t __x;

      for (__x = 0; __x < binary_heap->size; __x++)
        binary_heap_node_free(binary_heap, binary_heap->nodes[__x]);

      free(binary_heap->nodes);
    }

    free(binary_heap);
  }
}
