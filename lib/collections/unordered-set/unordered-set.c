#include "unordered-set.h"

#include <collections/hashtable/hashtable.h>

#include <strings/strings.h>
#include <fnv1a/fnv1a.h>

#include <stdlib.h>

#include <stdarg.h>
#include <stdint.h>

struct unordered_set {
  collection_value_free_t value_free;
  hashtable_t *__hashtable;
};


static uint64_t
unordered_set_hasher(const uint8_t *key, size_t key_size) {
  return fnv1a_hash(key, key_size);
}

unordered_set_reverse_iterator_t
unordered_set_rnext(unordered_set_reverse_iterator_t iterator) {
  return (unordered_set_reverse_iterator_t)hashtable_rnext((hashtable_reverse_iterator_t)iterator);
}

unordered_set_reverse_iterator_t
unordered_set_rprev(unordered_set_reverse_iterator_t iterator) {
  return (unordered_set_reverse_iterator_t)hashtable_rprev((hashtable_reverse_iterator_t)iterator);
}

unordered_set_reverse_iterator_t
unordered_set_rbegin(unordered_set_t *unordered_set) {
  return (unordered_set_reverse_iterator_t)hashtable_rbegin(unordered_set->__hashtable);
}

unordered_set_reverse_iterator_t
unordered_set_rend(unordered_set_t *unordered_set) {
  return (unordered_set_reverse_iterator_t)hashtable_rend(unordered_set->__hashtable);
}

unordered_set_iterator_t
unordered_set_next(unordered_set_iterator_t iterator) {
  return (unordered_set_iterator_t)hashtable_next((hashtable_iterator_t)iterator);
}

unordered_set_iterator_t
unordered_set_prev(unordered_set_iterator_t iterator) {
  return (unordered_set_iterator_t)hashtable_prev((hashtable_iterator_t)iterator);
}

unordered_set_iterator_t
unordered_set_begin(unordered_set_t *unordered_set) {
  return (unordered_set_iterator_t)hashtable_begin(unordered_set->__hashtable);
}

unordered_set_iterator_t
unordered_set_end(unordered_set_t *unordered_set) {
  return (unordered_set_iterator_t)hashtable_end(unordered_set->__hashtable);
}

void *
unordered_set_value(unordered_set_iterator_t iterator) {
  return hashtable_value((hashtable_iterator_t)iterator);
}

size_t
unordered_set_value_size(unordered_set_iterator_t iterator) {
  return hashtable_value_size((hashtable_iterator_t)iterator);
}

int
unordered_set_insert(unordered_set_t *unordered_set, void *value, size_t value_size, bool value_copy, bool value_free) {
  return hashtable_insert(unordered_set->__hashtable, value, value_size, value_copy, value_free, value, value_size, value_copy, value_free);
}

int
unordered_set_remove(unordered_set_t *unordered_set, const void *value, size_t value_size) {
  return hashtable_remove(unordered_set->__hashtable, value, value_size);
}

void *
unordered_set_findif(unordered_set_t *unordered_set, collection_predicate_t predicate, ...) {
  hashtable_iterator_t iter = NULL;
  va_list args;

  for (iter = hashtable_begin(unordered_set->__hashtable); iter; iter = hashtable_next(iter)) {
    va_start(args, predicate);

    if (predicate(hashtable_value(iter), args))
      return hashtable_value(iter);

    va_end(args);
  }

  return NULL;
}

size_t
unordered_set_size(unordered_set_t *unordered_set) {
  return hashtable_size(unordered_set->__hashtable);
}

bool
unordered_set_empty(unordered_set_t *unordered_set) {
  return hashtable_empty(unordered_set->__hashtable);
}

collection_value_free_t
unordered_set_value_free(unordered_set_t *unordered_set) {
  return hashtable_value_free(unordered_set->__hashtable);
}

unordered_set_t *
unordered_set_new(collection_value_free_t value_free) {
  unordered_set_t *unordered_set = NULL;

  if (!(unordered_set = (unordered_set_t *)calloc(1, sizeof(struct unordered_set))))
    goto _return;

  if (!(unordered_set->__hashtable = hashtable_new(unordered_set_hasher, value_free, value_free)))
    goto _return;

  return unordered_set;

_return:
  unordered_set_free(unordered_set);
  return NULL;
}

void
unordered_set_free(unordered_set_t *unordered_set) {
  if (unordered_set) {
    if (unordered_set->__hashtable)
      hashtable_free(unordered_set->__hashtable);

    free(unordered_set);
  }
}
