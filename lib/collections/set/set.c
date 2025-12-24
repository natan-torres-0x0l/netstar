#include "set.h"

#include <collections/rbtree/rbtree.h>

#include <stdlib.h>

struct set {
  collection_compare_t compare;
  collection_value_free_t value_free;
  rbtree_t *__tree;
};


set_reverse_iterator_t
set_rnext(set_reverse_iterator_t iterator) {
  return (set_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

set_reverse_iterator_t
set_rprev(set_reverse_iterator_t iterator) {
  return (set_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

set_reverse_iterator_t
set_rbegin(set_t *set) {
  return (set_reverse_iterator_t)rbtree_rbegin(set->__tree);
}

set_reverse_iterator_t
set_rend(set_t *set) {
  return (set_reverse_iterator_t)rbtree_rend(set->__tree);
}

set_iterator_t
set_next(set_iterator_t iterator) {
  return (set_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

set_iterator_t
set_prev(set_iterator_t iterator) {
  return (set_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

set_iterator_t
set_begin(set_t *set) {
  return (set_iterator_t)rbtree_begin(set->__tree);
}

set_iterator_t
set_end(set_t *set) {
  return (set_iterator_t)rbtree_end(set->__tree);
}

void *
set_value(set_iterator_t iterator) {
  return rbtree_value((rbtree_iterator_t)iterator);
}

size_t
set_value_size(set_iterator_t iterator) {
  return rbtree_value_size((rbtree_iterator_t)iterator);
}

void *
set_findif(set_t *set, collection_predicate_t predicate, ...) {
  rbtree_iterator_t iterator = NULL;
  va_list args;

  for (iterator = rbtree_begin(set->__tree); iterator; iterator = rbtree_next(iterator)) {
    va_start(args, predicate);

    if (predicate(rbtree_value(iterator), args))
      return rbtree_value(iterator);

    va_end(args);
  }

  return NULL;
}

void *
set_find(set_t *set, const void *value, size_t size) {
  return rbtree_find(set->__tree, value, size);
}

int
set_insert(set_t *set, void *value, size_t size, bool copy, bool free) {
  return rbtree_insert(set->__tree, value, size, copy, free);
}

int
set_remove(set_t *set, const void *value, size_t size) {
  return rbtree_remove(set->__tree, value, size);
}

size_t
set_size(set_t *set) {
  return rbtree_size(set->__tree);
}

bool
set_empty(set_t *set) {
  return rbtree_empty(set->__tree);
}

collection_compare_t
set_value_compare(set_t *set) {
  return set->compare;
}

collection_value_free_t
set_value_free(set_t *set) {
  return set->value_free;
}

set_t *
set_new(collection_compare_t compare, collection_value_free_t value_free) {
  set_t *set = NULL;

  if (!(set = (set_t *)calloc(1, sizeof(set_t))))
    goto _return;

  if (!(set->__tree = rbtree_new(compare, value_free)))
    goto _return;

  set->compare = compare;
  set->value_free = value_free;

  return set;

_return:
  set_free(set);
  return NULL;
}

void
set_free(set_t *set) {
  if (set) {
    rbtree_free(set->__tree);
    free(set);
  }
}
