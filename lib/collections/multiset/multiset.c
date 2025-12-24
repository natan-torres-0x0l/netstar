#include "multiset.h"

#include <collections/rbtree/rbtree.h>

#include <stdlib.h>

struct multiset_node {
  multiset_compare_t compare;
  collection_value_t value;
};

struct multiset {
  multiset_compare_t compare;
  collection_value_free_t value_free;
  rbtree_t *__tree;
};


static int
multiset_node_compare(const void *value1, const void *value2) {
  struct multiset_node *node1 = (struct multiset_node *)value1;
  struct multiset_node *node2 = (struct multiset_node *)value2;

  multiset_compare_t compare = node1->compare;

  if (compare(node1->value.ptr, node2->value.ptr))
    return -1;

  return 1;
}

static struct multiset_node *
multiset_node_new(multiset_t *multiset, void *value, size_t size, bool copy, bool free) {
  struct multiset_node *node = NULL;

  if (!(node = (struct multiset_node *)calloc(1, sizeof(struct multiset_node))))
    return NULL;

  node->value.ptr = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  node->compare = multiset->compare;

  return node;
}

static inline void
multiset_node_free(multiset_t *multiset, struct multiset_node *node) {
  if (node) {
    if (multiset->value_free && node->value.free && node->value.ptr)
      multiset->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

multiset_reverse_iterator_t
multiset_rnext(multiset_reverse_iterator_t iterator) {
  return (multiset_reverse_iterator_t)rbtree_rnext((rbtree_reverse_iterator_t)iterator);
}

multiset_reverse_iterator_t
multiset_rprev(multiset_reverse_iterator_t iterator) {
  return (multiset_reverse_iterator_t)rbtree_rprev((rbtree_reverse_iterator_t)iterator);
}

multiset_reverse_iterator_t
multiset_rbegin(multiset_t *multiset) {
  return (multiset_reverse_iterator_t)rbtree_rbegin(multiset->__tree);
}

multiset_reverse_iterator_t
multiset_rend(multiset_t *multiset) {
  return (multiset_reverse_iterator_t)rbtree_rend(multiset->__tree);
}

multiset_iterator_t
multiset_next(multiset_iterator_t iterator) {
  return (multiset_iterator_t)rbtree_next((rbtree_iterator_t)iterator);
}

multiset_iterator_t
multiset_prev(multiset_iterator_t iterator) {
  return (multiset_iterator_t)rbtree_prev((rbtree_iterator_t)iterator);
}

multiset_iterator_t
multiset_begin(multiset_t *multiset) {
  return (multiset_iterator_t)rbtree_begin(multiset->__tree);
}

multiset_iterator_t
multiset_end(multiset_t *multiset) {
  return (multiset_iterator_t)rbtree_end(multiset->__tree);
}

void *
multiset_value(multiset_iterator_t iterator) {
  return ((iterator) ? ((struct multiset_node *)rbtree_value((rbtree_iterator_t)iterator))->value.ptr : NULL);
}

size_t
multiset_value_size(multiset_iterator_t iterator) {
  return ((iterator) ? ((struct multiset_node *)rbtree_value((rbtree_iterator_t)iterator))->value.size : 0);
}

int
multiset_insert(multiset_t *multiset, void *value, size_t size, bool copy, bool free) {
  struct multiset_node *node = NULL;
  
  if (!(node = multiset_node_new(multiset, value, size, copy, free)))
    return -1;

  return rbtree_insert(multiset->__tree, rbtree_rvalue(node, sizeof(struct multiset_node *)));
}

size_t
multiset_size(multiset_t *multiset) {
  return rbtree_size(multiset->__tree);
}

bool
multiset_empty(multiset_t *multiset) {
  return rbtree_empty(multiset->__tree);
}

multiset_compare_t
multiset_value_compare(multiset_t *multiset) {
  return multiset->compare;
}

collection_value_free_t
multiset_value_free(multiset_t *multiset) {
  return multiset->value_free;
}

multiset_t *
multiset_new(multiset_compare_t compare, collection_value_free_t value_free) {
  multiset_t *multiset = NULL;

  if (!(multiset = (multiset_t *)calloc(1, sizeof(multiset_t))))
    goto _return;

  if (!(multiset->__tree = rbtree_new(multiset_node_compare, value_free)))
    goto _return;

  multiset->compare = compare;
  multiset->value_free = value_free;

  return multiset;

_return:
  multiset_free(multiset);
  return NULL;
}

void
multiset_free(multiset_t *multiset) {
  if (multiset) {
    if (multiset->__tree) {
      rbtree_iterator_t iterator = NULL;

      for (iterator = rbtree_begin(multiset->__tree); iterator; iterator = rbtree_next(iterator))
        multiset_node_free(multiset, (struct multiset_node *)rbtree_value(iterator));

      rbtree_free(multiset->__tree);
    }

    free(multiset);
  }
}
