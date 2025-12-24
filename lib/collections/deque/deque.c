#include "deque.h"

#include <stdlib.h>
#include <string.h>


#define DEQUE_INITIAL_CAPACITY 10


struct deque_node {
  struct deque_node *left, *right; 
  collection_value_t value;
};

struct deque {
  struct deque_node *begin, *end;
  struct deque_node **blocks;
  long front, back;

  size_t capacity, size;

  collection_value_free_t value_free;
};


static inline bool
deque_requires_reallocation(deque_t *deque) {
  return deque->size == deque->capacity;
}

static inline size_t
deque_capacity_growth(size_t size) {
  return size*2;
}

static inline int
deque_reallocate(deque_t *deque, size_t capacity) {
  struct deque_node **blocks = deque->blocks;
  size_t nblock;

  if (!(blocks = (struct deque_node **)calloc(capacity, sizeof(void *))))
    return -1;

  for (nblock = 0; nblock < deque->size; nblock++)
    blocks[nblock] = deque->blocks[(deque->front + (long)nblock) % (long)deque->capacity];

  free(deque->blocks);
  deque->blocks = blocks;

  deque->capacity = capacity;
  deque->front = 0;
  deque->back  = (long)deque->size;

  return 1;
}

static inline int
deque_reallocate_capacity(deque_t *deque) {
  return deque_reallocate(deque, deque_capacity_growth(deque->size));
}

static struct deque_node *
deque_node_new(void *value, size_t size, bool copy, bool free) {
  struct deque_node *node = NULL;

  if (!(node = (struct deque_node *)calloc(1, sizeof(struct deque_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  return node;
}

static inline void
deque_node_free(deque_t *deque, struct deque_node *node) {
  if (node) {
    if (deque->value_free && node->value.free && node->value.ptr)
      deque->value_free(node->value.ptr);
    
    free(node);
  }
}

deque_reverse_iterator_t
deque_rnext(deque_reverse_iterator_t iterator) {
  return ((iterator) ? (deque_reverse_iterator_t)((struct deque_node *)iterator)->left : NULL);
}

deque_reverse_iterator_t
deque_rprev(deque_reverse_iterator_t iterator) {
  return ((iterator) ? (deque_reverse_iterator_t)((struct deque_node *)iterator)->right : NULL);
}

deque_reverse_iterator_t
deque_rbegin(deque_t *deque) {
  return (deque_reverse_iterator_t)deque->end;
}

deque_reverse_iterator_t
deque_rend(deque_t *deque) {
  return (deque_reverse_iterator_t)deque->begin;
}

deque_iterator_t
deque_next(deque_iterator_t iterator) {
  return ((iterator) ? (deque_iterator_t)((struct deque_node *)iterator)->right : NULL);
}

deque_iterator_t
deque_prev(deque_iterator_t iterator) {
  return ((iterator) ? (deque_iterator_t)((struct deque_node *)iterator)->left : NULL);
}

deque_iterator_t
deque_begin(deque_t *deque) {
  return (deque_iterator_t)deque->begin;
}

deque_iterator_t
deque_end(deque_t *deque) {
  return (deque_iterator_t)deque->end;
}

void *
deque_value(deque_iterator_t iterator) {
  return ((iterator) ? ((struct deque_node *)iterator)->value.ptr : NULL);
}

size_t
deque_value_size(deque_iterator_t iterator) {
  return ((iterator) ? ((struct deque_node *)iterator)->value.size : 0);
}

int
deque_pushfront(deque_t *deque, void *value, size_t size, bool copy, bool free) {
  struct deque_node *node = NULL, *begin = NULL;

  if (deque_requires_reallocation(deque) && deque_reallocate_capacity(deque) == -1)
    return -1;

  if (!(node = deque_node_new(value, size, copy, free)))
    return -1;

  deque->front = (deque->front-1+(long)deque->capacity) % (long)deque->capacity;
  deque->blocks[deque->front] = node;
//deque->front = (deque->front+1) % deque->capacity;
  deque->size++;

  if (deque->begin) {
    if (!deque->end->left)
      deque->end->left = node;

    begin = deque->begin;
    deque->begin = node;
    deque->begin->right = begin;
    begin->left = node;

    return 1;
  }

  deque->begin = deque->end = node;
  return 1;
}

void *
deque_popfront(deque_t *deque) {
  struct deque_node *right = NULL;
  void *value = NULL;

  if (deque->begin) {
    value = deque->begin->value.ptr;
    right = deque->begin->right;

    if (right) {
      deque->begin = right;
      deque->begin->left = NULL;
    } else {
      deque->begin = NULL;
      deque->end = NULL;
    }

    deque->front = (deque->front+1) % (long)deque->capacity;
    deque->size--;
  }

  return value;
}

void *
deque_front(deque_t *deque) {
  return (!deque_empty(deque) ? deque->begin->value.ptr : NULL);
}

int
deque_pushback(deque_t *deque, void *value, size_t size, bool copy, bool free) {
  struct deque_node *node = NULL, *end = NULL;

  if (deque_requires_reallocation(deque) && deque_reallocate_capacity(deque) == -1)
    return -1;

  if (!(node = deque_node_new(value, size, copy, free)))
    return -1;

// deque->back = (deque->back-1+deque->capacity) % deque->capacity;
  deque->blocks[deque->back] = node;
  deque->back = (deque->back+1) % (long)deque->capacity;
  deque->size++;

  if (deque->begin) {
    if (!deque->begin->right)
      deque->begin->right = node;

    end = deque->end;
    deque->end = node;
    deque->end->left = end;
    end->right = node;

    return 1;
  }

  deque->begin = deque->end = node;
  return 1;
}

void *
deque_popback(deque_t *deque) {
  struct deque_node *left = NULL;
  void *value = NULL;

  if (deque->begin && deque->end) {
    value = deque->end->value.ptr;
    left = deque->end->left;

    if (deque) {
      deque->end = left;
      deque->end->right = NULL;
    } else {
      deque->begin = NULL;
      deque->end = NULL;
    }

    deque->back = (deque->back-1+(long)deque->capacity) % (long)deque->capacity;
    deque->size--;
  }

  return value;
}

void *
deque_back(deque_t *deque) {
  return (!deque_empty(deque) ? deque->end->value.ptr : NULL);
}

static struct deque_node *
deque_search(deque_t *deque, const void *ptr, size_t size) {
  struct deque_node *node = NULL;

  for (node = deque->begin; node; node = node->right)
    if (node->value.size == size && !memcmp(node->value.ptr, ptr, size))
      return node;

  return NULL;
}

void *
deque_findif(deque_t *deque, collection_predicate_t predicate, ...) {
  struct deque_node *node = NULL;
  va_list args;

  for (node = deque->begin; node; node = node->right) {
    va_start(args, predicate);

    if (predicate(node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

void *
deque_find(deque_t *deque, const void *ptr, size_t size) {
  struct deque_node *node = NULL;

  if (!(node = deque_search(deque, ptr, size)))
    return NULL;

  return node->value.ptr;
}

static bool
deque_range_check(deque_t *deque, size_t index) {
  return (deque->size && index < deque->size);
}

void *
deque_at(deque_t *deque, size_t index) {
  return ((deque_range_check(deque, index)) ? deque->blocks[(deque->front+(long)index) % (long)deque->capacity]->value.ptr : NULL);
}

size_t
deque_size(deque_t *deque) {
  return deque->size;
}

bool
deque_empty(deque_t *deque) {
  return !deque->begin;
}

collection_value_free_t
deque_value_free(deque_t *deque) {
  return deque->value_free;
}

deque_t *
deque_new(collection_value_free_t value_free) {
  deque_t *deque = NULL;

  if (!(deque = (deque_t *)calloc(1, sizeof(struct deque))))
    goto _return;
 
  deque->capacity = DEQUE_INITIAL_CAPACITY;

  if (!(deque->blocks = (struct deque_node **)calloc(deque->capacity, sizeof(struct deque_node *))))
    goto _return;

  deque->value_free = value_free;
  return deque;

_return:
  deque_free(deque);
  return NULL;
}

void
deque_free(deque_t *deque) {
  if (deque) {
    if (deque->blocks) {
      size_t nblock;

      for (nblock = 0; nblock < deque->capacity; nblock++)
        deque_node_free(deque, deque->blocks[nblock]);

      free(deque->blocks);
    }

    free(deque);
  }
}
