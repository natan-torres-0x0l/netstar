#include "hashset.h"

#include <collections/rbtree/rbtree.h>
#include <fnv1a/fnv1a.h>

#include <stdlib.h>

struct hashset_node {
  collection_value_t value;
  uint64_t hash;
};

struct hashset {
  collection_value_free_t value_free;
  rbtree_t *__tree;
};


static int
hashset_node_compare(const void *value1, const void *value2) {
  struct hashset_node *node1 = (struct hashset_node *)value1;
  struct hashset_node *node2 = (struct hashset_node *)value2;

  if (node1->hash < node2->hash)
    return -1;
  if (node1->hash > node2->hash)
    return 1;

  return 0;
}

static struct hashset_node *
hashset_node_new(hashset_t *hashset, void *value, size_t size, bool copy, bool free) {
  struct hashset_node *node = NULL;

// @ custom hasher function
  (void)hashset;

  if (!(node = (struct hashset_node *)calloc(1, sizeof(struct hashset_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  node->hash = fnv1a_hash((const uint8_t *)value, size);

  return node;
}

static void
hashset_node_free(hashset_t *hashset, struct hashset_node *node) {
  if (hashset->value_free && node->value.free && node->value.ptr)
    hashset->value_free(node->value.ptr), node->value.ptr = NULL;

  free(node);
}

hashset_reverse_iterator_t
hashset_rnext(hashset_reverse_iterator_t iterator) {
  return (hashset_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

hashset_reverse_iterator_t
hashset_rprev(hashset_reverse_iterator_t iterator) {
  return (hashset_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

hashset_reverse_iterator_t
hashset_rbegin(hashset_t *hashset) {
  return (hashset_reverse_iterator_t)rbtree_rbegin(hashset->__tree);
}

hashset_reverse_iterator_t
hashset_rend(hashset_t *hashset) {
  return (hashset_reverse_iterator_t)rbtree_rend(hashset->__tree);
}

hashset_iterator_t
hashset_next(hashset_iterator_t iterator) {
  return (hashset_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

hashset_iterator_t
hashset_prev(hashset_iterator_t iterator) {
  return (hashset_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

hashset_iterator_t
hashset_begin(hashset_t *hashset) {
  return (hashset_iterator_t)rbtree_begin(hashset->__tree);
}

hashset_iterator_t
hashset_end(hashset_t *hashset) {
  return (hashset_iterator_t)rbtree_end(hashset->__tree);
}

void *
hashset_value(hashset_iterator_t iterator) {
  return ((iterator) ? ((struct hashset_node *)rbtree_value((rbtree_iterator_t)iterator))->value.ptr : NULL);
}

size_t
hashset_value_size(hashset_iterator_t iterator) {
  return ((iterator) ? ((struct hashset_node *)rbtree_value((rbtree_iterator_t)iterator))->value.size : 0);
}

/*
static struct hashset_node *
hashset_search(hashset_t *hashset, const void *value, size_t size) {
  hashset_iterator_t iterator = NULL;
  struct hashset_node *node = NULL;

  uint64_t hash = fnv1a_hash((const uint8_t *)value, size);

  for (iterator = rbtree_begin(hashset->__tree); iterator; iterator = rbtree_next(iterator)) {
    node = (struct hashset_node *)rbtree_value(iterator);

    if (node->hash == hash)
      return node;
  }

  return NULL;
}
*/

static int
hashset_search_node(void *current_node, va_list args) {
  struct hashset_node *node = (struct hashset_node *)current_node;
  uint64_t hash = *va_arg(args, uint64_t *);

  if (hash < node->hash)
    return -1;
  else if (hash > node->hash)
    return 1;

  return 0;
}

static struct hashset_node *
hashset_search(hashset_t *hashset, const void *value, size_t size) {
  uint64_t hash = fnv1a_hash((const uint8_t *)value, size);

  return (struct hashset_node *)rbtree_findif(hashset->__tree, hashset_search_node, &hash);
}

void *
hashset_findif(hashset_t *hashset, collection_predicate_t predicate, ...) {
  rbtree_iterator_t iterator = NULL;
  va_list args;

  for (iterator = rbtree_begin(hashset->__tree); iterator; iterator = rbtree_next(iterator)) {
    struct hashset_node *node = (struct hashset_node *)rbtree_value(iterator);

    va_start(args, predicate);

    if (predicate(node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

void *
hashset_find(hashset_t *hashset, const void *value, size_t size) {
  struct hashset_node *node = NULL;

  if (!(node = hashset_search(hashset, value, size)))
    return NULL;

  return node->value.ptr; // rbtree_findif(hashset->__tree, hashset_node_find, fnv1a_hash((const uint8_t *)value, size));
}

int
hashset_insert(hashset_t *hashset, void *value, size_t size, bool copy, bool free) {
  struct hashset_node *node = NULL;
  
  if (!(node = hashset_node_new(hashset, value, size, copy, free)))
    return -1;

  return rbtree_insert(hashset->__tree, rbtree_rvalue(node, sizeof(struct hashset_node *)));
}

static int
hashset_node_remove(hashset_t *hashset, struct hashset_node *node) {
  int remove;

  remove = rbtree_remove(hashset->__tree, node, sizeof(struct hashset_node));

  hashset_node_free(hashset, node);

  return remove;
}

int
hashset_remove(hashset_t *hashset, const void *value, size_t size) {
  struct hashset_node *node = NULL;

  if (!(node = hashset_search(hashset, value, size)))
    return -1;

  return hashset_node_remove(hashset, node);
}

size_t
hashset_size(hashset_t *hashset) {
  return rbtree_size(hashset->__tree);
}

bool
hashset_empty(hashset_t *hashset) {
  return rbtree_empty(hashset->__tree);
}

collection_value_free_t
hashset_value_free(hashset_t *hashset) {
  return hashset->value_free;
}

hashset_t *
hashset_new(collection_value_free_t value_free) {
  hashset_t *hashset = NULL;

  if (!(hashset = (hashset_t *)calloc(1, sizeof(hashset_t))))
    goto _return;

  if (!(hashset->__tree = rbtree_new(hashset_node_compare, NULL)))
    goto _return;

  hashset->value_free = value_free;

  return hashset;

_return:
  hashset_free(hashset);
  return NULL;
}

void
hashset_free(hashset_t *hashset) {
  if (hashset) {
    if (hashset->__tree) {
      rbtree_iterator_t iterator = NULL;

      for (iterator = rbtree_begin(hashset->__tree); iterator; iterator = rbtree_next(iterator))
        hashset_node_free(hashset, (struct hashset_node *)rbtree_value(iterator));

      rbtree_free(hashset->__tree);
    }

    free(hashset);
  }
}
