#include "unordered-map.h"

#include <collections/hashtable/hashtable.h>

#include <strings/strings.h>
#include <fnv1a/fnv1a.h>

#include <stdlib.h>

#include <stdarg.h>
#include <stdint.h>

struct unordered_map {
  collection_value_free_t key_free, value_free;
  hashtable_t *__hashtable;
};


static uint64_t
unordered_map_hasher(const uint8_t *key, size_t key_size) {
  return fnv1a_hash(key, key_size);
}

unordered_map_reverse_iterator_t
unordered_map_rnext(unordered_map_reverse_iterator_t iterator) {
  return (unordered_map_reverse_iterator_t)hashtable_rnext((hashtable_reverse_iterator_t)iterator);
}

unordered_map_reverse_iterator_t
unordered_map_rprev(unordered_map_reverse_iterator_t iterator) {
  return (unordered_map_reverse_iterator_t)hashtable_rprev((hashtable_reverse_iterator_t)iterator);
}

unordered_map_reverse_iterator_t
unordered_map_rbegin(unordered_map_t *unordered_map) {
  return (unordered_map_reverse_iterator_t)hashtable_rbegin(unordered_map->__hashtable);
}

unordered_map_reverse_iterator_t
unordered_map_rend(unordered_map_t *unordered_map) {
  return (unordered_map_reverse_iterator_t)hashtable_rend(unordered_map->__hashtable);
}

unordered_map_iterator_t
unordered_map_next(unordered_map_iterator_t iterator) {
  return (unordered_map_iterator_t)hashtable_next((hashtable_iterator_t)iterator);
}

unordered_map_iterator_t
unordered_map_prev(unordered_map_iterator_t iterator) {
  return (unordered_map_iterator_t)hashtable_prev((hashtable_iterator_t)iterator);
}

unordered_map_iterator_t
unordered_map_begin(unordered_map_t *unordered_map) {
  return (unordered_map_iterator_t)hashtable_begin(unordered_map->__hashtable);
}

unordered_map_iterator_t
unordered_map_end(unordered_map_t *unordered_map) {
  return (unordered_map_iterator_t)hashtable_end(unordered_map->__hashtable);
}

void *
unordered_map_key(unordered_map_iterator_t iterator) {
  return hashtable_key((hashtable_iterator_t)iterator);
}

size_t
unordered_map_key_size(unordered_map_iterator_t iterator) {
  return hashtable_key_size((hashtable_iterator_t)iterator);
}

void *
unordered_map_value(unordered_map_iterator_t iterator) {
  return hashtable_value((hashtable_iterator_t)iterator);
}

size_t
unordered_map_value_size(unordered_map_iterator_t iterator) {
  return hashtable_value_size((hashtable_iterator_t)iterator);
}

int
unordered_map_insert(unordered_map_t *unordered_map, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  return hashtable_insert(unordered_map->__hashtable, key, key_size, key_copy, key_free, value, value_size, value_copy, value_free);
}

int
unordered_map_remove(unordered_map_t *unordered_map, const void *key, size_t key_size) {
  return hashtable_remove(unordered_map->__hashtable, key, key_size);
}

void *
unordered_map_get(unordered_map_t *unordered_map, const void *key, size_t key_size) {
  return hashtable_get(unordered_map->__hashtable, key, key_size);
}

void *
unordered_map_getor(unordered_map_t *unordered_map, const void *key, size_t key_size, void *value_default) {
  return hashtable_getor(unordered_map->__hashtable, key, key_size, value_default);
}

void *
unordered_map_findif(unordered_map_t *unordered_map, unordered_map_predicate_t predicate, ...) {
  hashtable_iterator_t iter = NULL;
  va_list args;

  for (iter = hashtable_begin(unordered_map->__hashtable); iter; iter = hashtable_next(iter)) {
    va_start(args, predicate);

    if (predicate(hashtable_key(iter), hashtable_value(iter), args))
      return hashtable_value(iter);

    va_end(args);
  }

  return NULL;
}

size_t
unordered_map_size(unordered_map_t *unordered_map) {
  return hashtable_size(unordered_map->__hashtable);
}

bool
unordered_map_empty(unordered_map_t *unordered_map) {
  return hashtable_empty(unordered_map->__hashtable);
}

collection_value_free_t
unordered_map_key_free(unordered_map_t *unordered_map) {
  return hashtable_key_free(unordered_map->__hashtable);
}

collection_value_free_t
unordered_map_value_free(unordered_map_t *unordered_map) {
  return hashtable_value_free(unordered_map->__hashtable);
}

unordered_map_t *
unordered_map_new(collection_value_free_t key_free, collection_value_free_t value_free) {
  unordered_map_t *unordered_map = NULL;

  if (!(unordered_map = (unordered_map_t *)calloc(1, sizeof(struct unordered_map))))
    goto _return;

  if (!(unordered_map->__hashtable = hashtable_new(unordered_map_hasher, key_free, value_free)))
    goto _return;

  return unordered_map;

_return:
  unordered_map_free(unordered_map);
  return NULL;
}

void
unordered_map_free(unordered_map_t *unordered_map) {
  if (unordered_map) {
    if (unordered_map->__hashtable)
      hashtable_free(unordered_map->__hashtable);

    free(unordered_map);
  }
}
