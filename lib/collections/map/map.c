#include "map.h"

#include <collections/rbtree/rbtree.h>

#include <strings/strings.h>
#include <fnv1a/fnv1a.h>

#include <stdlib.h>

#include <stdarg.h>
#include <stdint.h>

struct map_node {
  collection_value_t key, value;
  map_compare_t compare;
};

struct map {
  map_compare_t compare;
  collection_value_free_t key_free, value_free;
  rbtree_t *__tree;
};


static int
map_node_compare(const void *value1, const void *value2) {
  struct map_node *node1 = (struct map_node *)value1;
  struct map_node *node2 = (struct map_node *)value2;

  map_compare_t compare = node1->compare;

  return compare(node1->key.ptr, node1->key.size, node2->key.ptr, node2->key.size);
}

static struct map_node *
map_node_new(map_t *map, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct map_node *node = NULL;

  if (!(node = (struct map_node *)calloc(1, sizeof(struct map_node))))
    return NULL;

  node->key.ptr = ((node->key.copy = key_copy) ? collection_value_copy(key, key_size) : key);
  node->key.size = key_size;
  node->key.free = key_free;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;

  node->compare = map->compare;

  return node;
}

static inline void
map_update_node(map_t *map, struct map_node *node, void *value, size_t value_size, bool value_copy, bool value_free) {
  if (map->value_free && node->value.free && node->value.ptr)
    map->value_free(node->value.ptr), node->value.ptr = NULL;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;
}

static inline void
map_node_free(map_t *map, struct map_node *node) {
  if (node) {
    if (map->key_free && node->key.free && node->key.ptr)
      map->key_free(node->key.ptr), node->key.ptr = NULL;

    if (map->value_free && node->value.free && node->value.ptr)
      map->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

static int
map_node_remove(map_t *map, struct map_node *node) {
  int remove;

  remove = rbtree_remove(map->__tree, node, sizeof(struct map_node));

  map_node_free(map, node);

  return remove;
}

map_reverse_iterator_t
map_rnext(map_reverse_iterator_t iterator) {
  return (map_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

map_reverse_iterator_t
map_rprev(map_reverse_iterator_t iterator) {
  return (map_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

map_reverse_iterator_t
map_rbegin(map_t *map) {
  return (map_reverse_iterator_t)rbtree_rbegin(map->__tree);
}

map_reverse_iterator_t
map_rend(map_t *map) {
  return (map_reverse_iterator_t)rbtree_rend(map->__tree);
}

map_iterator_t
map_next(map_iterator_t iterator) {
  return (map_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

map_iterator_t
map_prev(map_iterator_t iterator) {
  return (map_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

map_iterator_t
map_begin(map_t *map) {
  return (map_iterator_t)rbtree_begin(map->__tree);
}

map_iterator_t
map_end(map_t *map) {
  return (map_iterator_t)rbtree_end(map->__tree);
}

void *
map_key(map_iterator_t iterator) {
  return ((iterator) ? ((struct map_node *)rbtree_value((rbtree_iterator_t)iterator))->key.ptr : NULL);
}

size_t
map_key_size(map_iterator_t iterator) {
  return ((iterator) ? ((struct map_node *)rbtree_value((rbtree_iterator_t)iterator))->key.size : 0);
}

void *
map_value(map_iterator_t iterator) {
  return ((iterator) ? ((struct map_node *)rbtree_value((rbtree_iterator_t)iterator))->value.ptr : NULL);
}

size_t
map_value_size(map_iterator_t iterator) {
  return ((iterator) ? ((struct map_node *)rbtree_value((rbtree_iterator_t)iterator))->value.size : 0);
}

/*
static struct map_node *
map_search(map_t *map, const void *key, size_t key_size) {
  map_iterator_t iterator = NULL;
  struct map_node *node = NULL;

  uint64_t hash = fnv1a_hash((const uint8_t *)key, key_size);

  for (iterator = rbtree_begin(map->__tree); iterator; iterator = rbtree_next(iterator)) {
    node = (struct map_node *)rbtree_value(iterator);

    if (node->hash == hash)
      return node;
  }

  return NULL;
}
*/

static int
map_search_node(void *value1, va_list args) {
  struct map_node *node = (struct map_node *)value1;
  const void *key = va_arg(args, const void *);
  size_t key_size = *va_arg(args, size_t *);

  return node->compare(key, key_size, node->key.ptr, node->key.size);
}

static struct map_node *
map_search(map_t *map, const void *key, size_t key_size) {
  return (struct map_node *)rbtree_findif(map->__tree, map_search_node, key, &key_size);
}

void *
map_findif(map_t *map, map_predicate_t predicate, ...) {
  rbtree_iterator_t iterator = NULL;
  va_list args;

  for (iterator = rbtree_begin(map->__tree); iterator; iterator = rbtree_next(iterator)) {
    struct map_node *node = (struct map_node *)rbtree_value(iterator);

    va_start(args, predicate);

    if (predicate(node->key.ptr, node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

int
map_insert(map_t *map, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct map_node *node = NULL;

  if ((node = map_search(map, key, key_size))) {
    map_update_node(map, node, value, value_size, value_copy, value_free); 
    return 1;
  }

  if (!(node = map_node_new(map, key, key_size, key_copy, key_free, value, value_size, value_copy, value_free)))
    return -1;

  return rbtree_insert(map->__tree, rbtree_rvalue(node, sizeof(struct map_node *)));
}

int
map_remove(map_t *map, const void *key, size_t key_size) {
  struct map_node *node = NULL;

  if (!(node = map_search(map, key, key_size)))
    return -1;

  return map_node_remove(map, node);
}

void *
map_get(map_t *map, const void *key, size_t key_size) {
  struct map_node *node = NULL;

  if (!(node = map_search(map, key, key_size)))
    return NULL;

  return node->value.ptr;
}

void *
map_getor(map_t *map, const void *key, size_t key_size, void *value_default) {
  void *value = map_get(map, key, key_size);
  return ((value) ? value : value_default);
}

size_t
map_size(map_t *map) {
  return rbtree_size(map->__tree);
}

bool
map_empty(map_t *map) {
  return rbtree_empty(map->__tree);
}

collection_value_free_t
map_key_free(map_t *map) {
  return map->key_free;
}

collection_value_free_t
map_value_free(map_t *map) {
  return map->value_free;
}

map_t *
map_new(map_compare_t compare, collection_value_free_t key_free, collection_value_free_t value_free) {
  map_t *map = NULL;

  if (!(map = (map_t *)calloc(1, sizeof(map_t))))
    goto _return;

  if (!(map->__tree = rbtree_new(map_node_compare, NULL)))
    goto _return;

  map->compare = compare;
  map->key_free = key_free;
  map->value_free = value_free;

  return map;

_return:
  map_free(map);
  return NULL;
}

void
map_free(map_t *map) {
  if (map) {
    if (map->__tree) {
      rbtree_iterator_t iterator = NULL;

      for (iterator = rbtree_begin(map->__tree); iterator; iterator = rbtree_next(iterator))
        map_node_free(map, (struct map_node *)rbtree_value(iterator));

      rbtree_free(map->__tree);
    }

    free(map);
  }
}
