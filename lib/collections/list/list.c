#include "list.h"

#include <stdlib.h>
#include <string.h>

struct list_node {
  struct list_node *left, *right;
  collection_value_t value;
};

struct list {
  struct list_node *begin, *end;
  size_t size;

  collection_value_free_t value_free;
};


static struct list_node *
list_node_new(void *value, size_t size, bool copy, bool free) {
  struct list_node *node = NULL;

  if (!(node = (struct list_node *)calloc(1, sizeof(struct list_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  return node;
}

static inline void
list_node_free(list_t *list, struct list_node *node) {
  if (node) {
    if (list->value_free && node->value.free && node->value.ptr)
      list->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

list_reverse_iterator_t
list_rnext(list_reverse_iterator_t iterator) {
  return ((iterator) ? (list_reverse_iterator_t)((struct list_node *)iterator)->left : NULL);
}

list_reverse_iterator_t
list_rprev(list_reverse_iterator_t iterator) {
  return ((iterator) ? (list_reverse_iterator_t)((struct list_node *)iterator)->right : NULL);
}

list_reverse_iterator_t
list_rbegin(list_t *list) {
  return (list_reverse_iterator_t)list->end;
}

list_reverse_iterator_t
list_rend(list_t *list) {
  return (list_reverse_iterator_t)list->begin;
}

list_iterator_t
list_next(list_iterator_t iterator) {
  return ((iterator) ? (list_iterator_t)((struct list_node *)iterator)->right : NULL);
}

list_iterator_t
list_prev(list_iterator_t iterator) {
  return ((iterator) ? (list_iterator_t)((struct list_node *)iterator)->left : NULL);
}

list_iterator_t
list_begin(list_t *list) {
  return (list_iterator_t)list->begin;
}

list_iterator_t
list_end(list_t *list) {
  return (list_iterator_t)list->end;
}

void *
list_value(list_iterator_t iterator) {
  return ((iterator) ? ((struct list_node *)iterator)->value.ptr : NULL);
}

size_t
list_value_size(list_iterator_t iterator) {
  return ((iterator) ? ((struct list_node *)iterator)->value.size : 0);
}

int
list_pushfront(list_t *list, void *value, size_t size, bool copy, bool free) {
  struct list_node *node = NULL, *begin = NULL;

  if (!(node = list_node_new(value, size, copy, free)))
    return -1;

  list->size++;

  if (list->begin) {
    if (!list->end->left)
      list->end->left = node;

    begin = list->begin;
    list->begin = node;
    list->begin->right = begin;
    begin->left = node;

    return 1;
  }

  list->begin = list->end = node;
  return 1;
}

void *
list_popfront(list_t *list) {
  struct list_node *right = NULL;
  void *value = NULL;

  if (list->begin) {
    value = list->begin->value.ptr;
    right = list->begin->right;

    free(list->begin);

    if (right) {
      list->begin = right;
      list->begin->left = NULL;
    } else {
      list->begin = NULL;
      list->end = NULL;
    }

    list->size--;
  }

  return value;
}

void *
list_front(list_t *list) {
  return (!list_empty(list) ? list->begin->value.ptr : NULL);
}

int
list_pushback(list_t *list, void *value, size_t size, bool copy, bool free) {
  struct list_node *node = NULL, *end = NULL;

  if (!(node = list_node_new(value, size, copy, free)))
    return -1;

  list->size++;

  if (list->begin) {
    if (!list->begin->right)
      list->begin->right = node;

    end = list->end;
    list->end = node;
    list->end->left = end;
    end->right = node;

    return 1;
  }

  list->begin = list->end = node;
  return 1;
}

void *
list_popback(list_t *list) {
  struct list_node *left = NULL;
  void *value = NULL;

  if (list->begin && list->end) {
    value = list->end->value.ptr;
    left = list->end->left;

    free(list->end);

    if (left) {
      list->end = left;
      list->end->right = NULL;
    } else {
      list->begin = NULL;
      list->end = NULL;
    }

    list->size--;
  }

  return value;
}

void *
list_back(list_t *list) {
  return (!list_empty(list) ? list->end->value.ptr : NULL);
}

static struct list_node *
list_search(list_t *list, const void *ptr, size_t size) {
  struct list_node *node = NULL;

  for (node = list->begin; node; node = node->right)
    if (node->value.size == size && !memcmp(node->value.ptr, ptr, size))
      return node;

  return NULL;
}

int
list_remove(list_t *list, void *ptr, size_t size) {
  struct list_node *node = NULL, *left = NULL, *right = NULL;

  if (!(node = list_search(list, ptr, size)))
    return -1;

  left = node->left;
  right = node->right;

  if (node == list->begin && right) {
    right->left = NULL;
    list->begin = right;
  } else if (node == list->end && left) {
    left->right = NULL;
    list->end = left;
  } else if (left && right) {
    left->right = right;
    right->left = left;
  } else {
    list->begin = NULL;
    list->end = NULL;
  }

  list_node_free(list, node);
  list->size--;

  return 1;
}

void *
list_findif(list_t *list, collection_predicate_t predicate, ...) {
  struct list_node *node = NULL;
  va_list args;

  for (node = list->begin; node; node = node->right) {
    va_start(args, predicate);

    if (predicate(node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

void *
list_find(list_t *list, const void *ptr, size_t size) {
  struct list_node *node = NULL;

  if (!(node = list_search(list, ptr, size)))
    return NULL;

  return node->value.ptr;
}

size_t
list_size(list_t *list) {
  return list->size;
}

bool
list_empty(list_t *list) {
  return !list->begin;
}

collection_value_free_t
list_value_free(list_t *list) {
  return list->value_free;
}

list_t *
list_new(collection_value_free_t value_free) {
  list_t *list = NULL;

  if (!(list = (list_t *)calloc(1, sizeof(list_t))))
    return NULL;

  list->value_free = value_free;

  return list;
}

void
list_free(list_t *list) {
  if (list) {
    struct list_node *node = NULL, *right = NULL;

    for (node = list->begin; node; list->size--) {
      right = node->right;

      list_node_free(list, node);
      node = right;
    }

    free(list);
  }
}
