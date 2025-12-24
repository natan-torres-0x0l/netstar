#include "multimap.h"

#include <collections/rbtree/rbtree.h>

#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdint.h>

struct multimap_node {
  collection_value_t key, value;
  multimap_compare_t compare;
};

struct multimap {
  multimap_compare_t compare;
  collection_value_free_t key_free, value_free;
  rbtree_t *__tree;
};


static int
multimap_node_compare(const void *value1, const void *value2) {
  struct multimap_node *node1 = (struct multimap_node *)value1;
  struct multimap_node *node2 = (struct multimap_node *)value2;

  multimap_compare_t compare = node1->compare;

  if (compare(node1->key.ptr, node1->key.size, node2->key.ptr, node2->key.size))
    return -1;

  return 1;
}

static struct multimap_node *
multimap_node_new(multimap_t *multimap, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct multimap_node *node = NULL;

  if (!(node = (struct multimap_node *)calloc(1, sizeof(struct multimap_node))))
    return NULL;

  node->key.ptr = ((node->key.copy = key_copy) ? collection_value_copy(key, key_size) : key);
  node->key.size = key_size;
  node->key.free = key_free;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;

  node->compare = multimap->compare;

  return node;
}

static inline void
multimap_node_free(multimap_t *multimap, struct multimap_node *node) {
  if (node) {
    if (multimap->key_free && node->key.free && node->key.ptr)
      multimap->key_free(node->key.ptr), node->key.ptr = NULL;

    if (multimap->value_free && node->value.free && node->value.ptr)
      multimap->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

multimap_reverse_iterator_t
multimap_rnext(multimap_reverse_iterator_t iterator) {
  return (multimap_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

multimap_reverse_iterator_t
multimap_rprev(multimap_reverse_iterator_t iterator) {
  return (multimap_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

multimap_reverse_iterator_t
multimap_rbegin(multimap_t *multimap) {
  return (multimap_reverse_iterator_t)rbtree_rbegin(multimap->__tree);
}

multimap_reverse_iterator_t
multimap_rend(multimap_t *multimap) {
  return (multimap_reverse_iterator_t)rbtree_rend(multimap->__tree);
}

multimap_iterator_t
multimap_next(multimap_iterator_t iterator) {
  return (multimap_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

multimap_iterator_t
multimap_prev(multimap_iterator_t iterator) {
  return (multimap_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

multimap_iterator_t
multimap_begin(multimap_t *multimap) {
  return (multimap_iterator_t)rbtree_begin(multimap->__tree);
}

multimap_iterator_t
multimap_end(multimap_t *multimap) {
  return (multimap_iterator_t)rbtree_end(multimap->__tree);
}

void *
multimap_key(multimap_iterator_t iterator) {
  return ((iterator) ? ((struct multimap_node *)rbtree_value((rbtree_iterator_t)iterator))->key.ptr : NULL);
}

size_t
multimap_key_size(multimap_iterator_t iterator) {
  return ((iterator) ? ((struct multimap_node *)rbtree_value((rbtree_iterator_t)iterator))->key.size : 0);
}

void *
multimap_value(multimap_iterator_t iterator) {
  return ((iterator) ? ((struct multimap_node *)rbtree_value((rbtree_iterator_t)iterator))->value.ptr : NULL);
}

size_t
multimap_value_size(multimap_iterator_t iterator) {
  return ((iterator) ? ((struct multimap_node *)rbtree_value((rbtree_iterator_t)iterator))->value.size : 0);
}

/*
static struct multimap_node *
multimap_search(multimap_t *multimap, const void *key, size_t key_size) {
  multimap_iterator_t iterator = NULL;
  struct multimap_node *node = NULL;

  uint64_t hash = fnv1a_hash((const uint8_t *)key, key_size);

  for (iterator = rbtree_begin(multimap->__tree); iterator; iterator = rbtree_next(iterator)) {
    node = (struct multimap_node *)rbtree_value(iterator);

    if (node->hash == hash)
      return node;
  }

  return NULL;
}
*/

static int
multimap_search_node(void *value, va_list args) {
  struct multimap_node *node = (struct multimap_node *)value;

  const void *key = va_arg(args, const void *);
  size_t key_size = *va_arg(args, size_t *);

  if (node->key.size == key_size && !memcmp(node->key.ptr, key, node->key.size))
    return 0;
  else if (node->compare(key, key_size, node->key.ptr, node->key.size))
    return -1;

  return 1;
}

static struct multimap_node *
multimap_search(multimap_t *multimap, const void *key, size_t key_size) {
  return (struct multimap_node *)rbtree_findif(multimap->__tree, multimap_search_node, key, &key_size);
}

void *
multimap_findif(multimap_t *multimap, multimap_predicate_t predicate, ...) {
  rbtree_iterator_t iterator = NULL;
  va_list args;

  for (iterator = rbtree_begin(multimap->__tree); iterator; iterator = rbtree_next(iterator)) {
    struct multimap_node *node = (struct multimap_node *)rbtree_value(iterator);

    va_start(args, predicate);

    if (predicate(node->key.ptr, node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

int
multimap_insert(multimap_t *multimap, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct multimap_node *node = NULL;

  if (!(node = multimap_node_new(multimap, key, key_size, key_copy, key_free, value, value_size, value_copy, value_free)))
    return -1;

  return rbtree_insert(multimap->__tree, rbtree_rvalue(node, sizeof(struct multimap_node *)));
}

void *
multimap_get(multimap_t *multimap, const void *key, size_t key_size) {
  struct multimap_node *node = NULL;

  if (!(node = multimap_search(multimap, key, key_size)))
    return NULL;

  return node->value.ptr;
}

void *
multimap_getor(multimap_t *multimap, const void *key, size_t key_size, void *value_default) {
  void *value = multimap_get(multimap, key, key_size);
  return ((value) ? value : value_default);
}

size_t
multimap_size(multimap_t *multimap) {
  return rbtree_size(multimap->__tree);
}

bool
multimap_empty(multimap_t *multimap) {
  return rbtree_empty(multimap->__tree);
}

collection_value_free_t
multimap_key_free(multimap_t *multimap) {
  return multimap->key_free;
}

collection_value_free_t
multimap_value_free(multimap_t *multimap) {
  return multimap->value_free;
}

multimap_t *
multimap_new(multimap_compare_t compare, collection_value_free_t key_free, collection_value_free_t value_free) {
  multimap_t *multimap = NULL;

  if (!(multimap = (multimap_t *)calloc(1, sizeof(multimap_t))))
    goto _return;

  if (!(multimap->__tree = rbtree_new(multimap_node_compare, NULL)))
    goto _return;

  multimap->compare = compare;
  multimap->key_free = key_free;
  multimap->value_free = value_free;

  return multimap;

_return:
  multimap_free(multimap);
  return NULL;
}

void
multimap_free(multimap_t *multimap) {
  if (multimap) {
    if (multimap->__tree) {
      rbtree_iterator_t iterator = NULL;

      for (iterator = rbtree_begin(multimap->__tree); iterator; iterator = rbtree_next(iterator))
        multimap_node_free(multimap, (struct multimap_node *)rbtree_value(iterator));

      rbtree_free(multimap->__tree);
    }

    free(multimap);
  }
}
