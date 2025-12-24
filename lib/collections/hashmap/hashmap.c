#include "hashmap.h"

#include <collections/rbtree/rbtree.h>

#include <strings/strings.h>
#include <fnv1a/fnv1a.h>

#include <stdlib.h>

#include <stdarg.h>
#include <stdint.h>

struct hashmap_node {
  collection_value_t key, value;
  uint64_t hash;
};

struct hashmap {
  collection_value_free_t key_free, value_free;
  rbtree_t *__tree;
};


static int
hashmap_node_compare(const void *value1L, const void *value2L) {
  struct hashmap_node *value1 = (struct hashmap_node *)value1L;
  struct hashmap_node *value2 = (struct hashmap_node *)value2L;

  if (value1->hash < value2->hash)
    return -1;
  if (value1->hash > value2->hash)
    return 1;

  return 0;
}

static struct hashmap_node *
hashmap_node_new(void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct hashmap_node *node = NULL;

  if (!(node = (struct hashmap_node *)calloc(1, sizeof(struct hashmap_node))))
    return NULL;

  node->key.ptr = ((node->key.copy = key_copy) ? collection_value_copy(key, key_size) : key);
  node->key.size = key_size;
  node->key.free = key_free;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;

  node->hash = fnv1a_hash((const uint8_t *)key, key_size);

  return node;
}

static inline void
hashmap_update_node(hashmap_t *hashmap, struct hashmap_node *node, void *value, size_t value_size, bool value_copy, bool value_free) {
  if (hashmap->value_free && node->value.free && node->value.ptr)
    hashmap->value_free(node->value.ptr), node->value.ptr = NULL;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;
}

static inline void
hashmap_node_free(hashmap_t *hashmap, struct hashmap_node *node) {
  if (node) {
    if (hashmap->key_free && node->key.free && node->key.ptr)
      hashmap->key_free(node->key.ptr), node->key.ptr = NULL;

    if (hashmap->value_free && node->value.free && node->value.ptr)
      hashmap->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

static int
hashmap_node_remove(hashmap_t *hashmap, struct hashmap_node *node) {
  int remove;

  remove = rbtree_remove(hashmap->__tree, node, sizeof(struct hashmap_node));

  hashmap_node_free(hashmap, node);

  return remove;
}

hashmap_reverse_iterator_t
hashmap_rnext(hashmap_reverse_iterator_t iterator) {
  return (hashmap_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

hashmap_reverse_iterator_t
hashmap_rprev(hashmap_reverse_iterator_t iterator) {
  return (hashmap_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

hashmap_reverse_iterator_t
hashmap_rbegin(hashmap_t *hashmap) {
  return (hashmap_reverse_iterator_t)rbtree_rbegin(hashmap->__tree);
}

hashmap_reverse_iterator_t
hashmap_rend(hashmap_t *hashmap) {
  return (hashmap_reverse_iterator_t)rbtree_rend(hashmap->__tree);
}

hashmap_iterator_t
hashmap_next(hashmap_iterator_t iterator) {
  return (hashmap_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

hashmap_iterator_t
hashmap_prev(hashmap_iterator_t iterator) {
  return (hashmap_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

hashmap_iterator_t
hashmap_begin(hashmap_t *hashmap) {
  return (hashmap_iterator_t)rbtree_begin(hashmap->__tree);
}

hashmap_iterator_t
hashmap_end(hashmap_t *hashmap) {
  return (hashmap_iterator_t)rbtree_end(hashmap->__tree);
}

void *
hashmap_key(hashmap_iterator_t iterator) {
  return ((iterator) ? ((struct hashmap_node *)rbtree_value((rbtree_iterator_t)iterator))->key.ptr : NULL);
}

size_t
hashmap_key_size(hashmap_iterator_t iterator) {
  return ((iterator) ? ((struct hashmap_node *)rbtree_value((rbtree_iterator_t)iterator))->key.size : 0);
}

void *
hashmap_value(hashmap_iterator_t iterator) {
  return ((iterator) ? ((struct hashmap_node *)rbtree_value((rbtree_iterator_t)iterator))->value.ptr : NULL);
}

size_t
hashmap_value_size(hashmap_iterator_t iterator) {
  return ((iterator) ? ((struct hashmap_node *)rbtree_value((rbtree_iterator_t)iterator))->value.size : 0);
}

/*
static struct hashmap_node *
hashmap_search(hashmap_t *hashmap, const void *key, size_t key_size) {
  hashmap_iterator_t iterator = NULL;
  struct hashmap_node *node = NULL;

  uint64_t hash = fnv1a_hash((const uint8_t *)key, key_size);

  for (iterator = rbtree_begin(hashmap->__tree); iterator; iterator = rbtree_next(iterator)) {
    node = (struct hashmap_node *)rbtree_value(iterator);

    if (node->hash == hash)
      return node;
  }

  return NULL;
}
*/

static int
hashmap_search_node(void *current_node, va_list args) {
  struct hashmap_node *node = (struct hashmap_node *)current_node;
  uint64_t hash = *va_arg(args, uint64_t *);

  if (hash < node->hash)
    return -1;
  else if (hash > node->hash)
    return 1;

  return 0;
}

static struct hashmap_node *
hashmap_search(hashmap_t *hashmap, const void *key, size_t key_size) {
  uint64_t hash = fnv1a_hash((const uint8_t *)key, key_size);

  return (struct hashmap_node *)rbtree_findif(hashmap->__tree, hashmap_search_node, &hash);
}

void *
hashmap_findif(hashmap_t *hashmap, hashmap_predicate_t predicate, ...) {
  rbtree_iterator_t iterator = NULL;
  va_list args;

  for (iterator = rbtree_begin(hashmap->__tree); iterator; iterator = rbtree_next(iterator)) {
    struct hashmap_node *node = (struct hashmap_node *)rbtree_value(iterator);

    va_start(args, predicate);

    if (predicate(node->key.ptr, node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

int
hashmap_insert(hashmap_t *hashmap, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct hashmap_node *node = NULL;

  if ((node = hashmap_search(hashmap, key, key_size))) {
    hashmap_update_node(hashmap, node, value, value_size, value_copy, value_free); 
    return 1;
  }

  if (!(node = hashmap_node_new(key, key_size, key_copy, key_free, value, value_size, value_copy, value_free)))
    return -1;

  return rbtree_insert(hashmap->__tree, rbtree_rvalue(node, sizeof(struct hashmap_node *)));
}

int
hashmap_remove(hashmap_t *hashmap, const void *key, size_t key_size) {
  struct hashmap_node *node = NULL;

  if (!(node = hashmap_search(hashmap, key, key_size)))
    return -1;

  return hashmap_node_remove(hashmap, node);
}

void *
hashmap_get(hashmap_t *hashmap, const void *key, size_t key_size) {
  struct hashmap_node *node = NULL;

  if (!(node = hashmap_search(hashmap, key, key_size)))
    return NULL;

  return node->value.ptr;
}

void *
hashmap_getor(hashmap_t *hashmap, const void *key, size_t key_size, void *value_default) {
  void *value = hashmap_get(hashmap, key, key_size);
  return ((value) ? value : value_default);
}

size_t
hashmap_size(hashmap_t *hashmap) {
  return rbtree_size(hashmap->__tree);
}

bool
hashmap_empty(hashmap_t *hashmap) {
  return rbtree_empty(hashmap->__tree);
}

collection_value_free_t
hashmap_key_free(hashmap_t *hashmap) {
  return hashmap->key_free;
}

collection_value_free_t
hashmap_value_free(hashmap_t *hashmap) {
  return hashmap->value_free;
}

hashmap_t *
hashmap_new(collection_value_free_t key_free, collection_value_free_t value_free) {
  hashmap_t *hashmap = NULL;

  if (!(hashmap = (hashmap_t *)calloc(1, sizeof(hashmap_t))))
    goto _return;

  if (!(hashmap->__tree = rbtree_new(hashmap_node_compare, NULL)))
    goto _return;

  hashmap->key_free = key_free;
  hashmap->value_free = value_free;

  return hashmap;

_return:
  hashmap_free(hashmap);
  return NULL;
}

void
hashmap_free(hashmap_t *hashmap) {
  if (hashmap) {
    if (hashmap->__tree) {
      rbtree_iterator_t iterator = NULL;

      for (iterator = rbtree_begin(hashmap->__tree); iterator; iterator = rbtree_next(iterator))
        hashmap_node_free(hashmap, (struct hashmap_node *)rbtree_value(iterator));

      rbtree_free(hashmap->__tree);
    }

    free(hashmap);
  }
}
