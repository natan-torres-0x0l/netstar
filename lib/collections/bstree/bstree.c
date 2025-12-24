#include "bstree.h"

#include <stdlib.h>

struct bstree_node {
  struct bstree_node *parent, *left, *right;
  collection_value_t value;
};

struct bstree {
  struct bstree_node *root, *begin, *end;
  size_t size;

  collection_compare_t compare;
  collection_value_free_t value_free;
};


static struct bstree_node *
bstree_node_new(void *value, size_t size, bool copy, bool free) {
  struct bstree_node *node = NULL;

  if (!(node = (struct bstree_node *)calloc(1, sizeof(struct bstree_node))))
    return NULL;

  node->value.ptr  = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  return node;
}

static inline void
bstree_node_free(bstree_t *bstree, struct bstree_node *node) {
  if (node) {
    if (bstree->value_free && node->value.free && node->value.ptr)
      bstree->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

static struct bstree_node *
bstree_node_right_most_up(struct bstree_node *__x) {
  struct bstree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->right;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct bstree_node *
bstree_node_left_most_up(struct bstree_node *__x) {
  struct bstree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->left;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct bstree_node *
bstree_node_right_most(struct bstree_node *__x) {
  struct bstree_node *__y = NULL;

  for (__y = __x; __y && __y->right;)
    __y = __y->right;

  return ((__y) ? __y : NULL);
}

static struct bstree_node *
bstree_node_left_most(struct bstree_node *__x) {
  struct bstree_node *__y = NULL;

  for (__y = __x; __y && __y->left;)
    __y = __y->left;

  return ((__y) ? __y : NULL);
}

bstree_reverse_iterator_t
bstree_rnext(bstree_reverse_iterator_t iterator) {
  struct bstree_node *node = (struct bstree_node *)iterator;
  return (bstree_reverse_iterator_t)((node) ? ((node->left) ? bstree_node_right_most(node->left) : bstree_node_left_most_up(node)) : NULL);
}

bstree_reverse_iterator_t
bstree_rprev(bstree_reverse_iterator_t iterator) {
  struct bstree_node *node = (struct bstree_node *)iterator;
  return (bstree_reverse_iterator_t)((node) ? ((node->right) ? bstree_node_left_most(node->right) : bstree_node_right_most_up(node)) : NULL);
}

bstree_reverse_iterator_t
bstree_rbegin(bstree_t *bstree) {
  return (bstree_reverse_iterator_t)bstree->end;
}

bstree_reverse_iterator_t
bstree_rend(bstree_t *bstree) {
  return (bstree_reverse_iterator_t)bstree->begin;
}

bstree_iterator_t
bstree_next(bstree_iterator_t iterator) {
  struct bstree_node *node = (struct bstree_node *)iterator;
  return (bstree_iterator_t)((node) ? ((node->right) ? bstree_node_left_most(node->right) : bstree_node_right_most_up(node)) : NULL);
}

bstree_iterator_t
bstree_prev(bstree_iterator_t iterator) {
  struct bstree_node *node = (struct bstree_node *)iterator;
  return (bstree_iterator_t)((node) ? ((node->left) ? bstree_node_right_most(node->left) : bstree_node_left_most_up(node)) : NULL);
}

bstree_iterator_t
bstree_begin(bstree_t *bstree) {
  return (bstree_iterator_t)bstree->begin;
}

bstree_iterator_t
bstree_end(bstree_t *bstree) {
  return (bstree_iterator_t)bstree->end;
}

void *
bstree_value(bstree_iterator_t iterator) {
  return ((iterator) ? ((struct bstree_node *)iterator)->value.ptr : NULL);
}

size_t
bstree_value_size(bstree_iterator_t iterator) {
  return ((iterator) ? ((struct bstree_node *)iterator)->value.size : 0);
}

static struct bstree_node *
bstree_node_search(bstree_t *bstree, const void *value, size_t size) {
  struct bstree_node *__x = NULL;
  int compare;

  for (__x = bstree->root; __x;) {
    if ((compare = bstree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return __x;
  }

  return NULL;
}

void *
bstree_findif(bstree_t *bstree, bstree_predicate_t predicate, ...) {
  struct bstree_node *__x = NULL;
  int compare;

  va_list args;

  for (__x = bstree->root; __x;) {
    va_start(args, predicate);

    if ((compare = predicate(__x->value.ptr, args)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return __x->value.ptr;

    va_end(args);
  }

  return NULL;
}

void *
bstree_find(bstree_t *bstree, const void *value, size_t size) {
  struct bstree_node *node = NULL;

  if (!(node = bstree_node_search(bstree, value, size)))
    return NULL;

  return node->value.ptr;
}

static struct bstree_node *
bstree_node_search_leaf(bstree_t *bstree, const void *value, size_t size) {
  struct bstree_node *__x = NULL, *__y = NULL;
  int compare;

  for (__x = bstree->root; __x;) {
    __y = __x;

    if ((compare = bstree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return NULL;
  }

  return __y;
}

int
bstree_insert(bstree_t *bstree, void *value, size_t size, bool copy, bool free) {
  struct bstree_node *__x = NULL, *__y = NULL;

  if (bstree->root && !(__y = bstree_node_search_leaf(bstree, value, size)))
    return -1;

  bstree->size++;

  if (!bstree->root) {
    if (!(__x = bstree_node_new(value, size, copy, free)))
      return -1;
    bstree->root = bstree->begin = bstree->end = __x;

    return 1;
  }

  if (!(__x = bstree_node_new(value, size, copy, free)))
    return -1;

  if (bstree->begin && bstree->compare(value, bstree->begin->value.ptr) == -1)
    bstree->begin = __x;
  if (bstree->end && bstree->compare(bstree->end->value.ptr, value) == -1)
    bstree->end = __x;

  __x->parent = __y;
  if (bstree->compare(value, __y->value.ptr) == -1)
    __y->left = __x;
  else
    __y->right = __x;

  return 1;
}

static inline void
bstree_node_transfer(bstree_t *bstree, struct bstree_node *__x, struct bstree_node *__y) {
  if (__y == bstree->root)
    bstree->root = __x;
  else if (__y == __y->parent->left)
    __y->parent->left = __x;
  else
    __y->parent->right = __x;

  if (__x)
    __x->parent = __y->parent;
}

int
bstree_remove(bstree_t *bstree, const void *value, size_t size) {
  struct bstree_node *__w = NULL, *__x = NULL, *__y = NULL;

  if (!(__x = bstree_node_search(bstree, value, size)))
    return -1;

  if (__x->left && __x->right)
    __w = bstree_node_right_most(__x->left);
  else
    __w = __x;

  __y = ((__w->left) ? __w->left : __w->right);

  bstree_node_transfer(bstree, __y, __w);

  if (__x != __w) {
    if (__w->parent != __x) {
      bstree_node_transfer(bstree, __w->left, __w);
      __w->left = __x->left;
      __w->left->parent = __w;
    } else
      __y->parent = __w;

    bstree_node_transfer(bstree, __w, __x);

    __w->right = __x->right;
    __w->right->parent = __w;
  }

  if (__x == bstree->begin)
    bstree->begin = bstree_node_left_most(bstree->root);
  if (__x == bstree->end)
    bstree->end = bstree_node_right_most(bstree->root);

  bstree_node_free(bstree, __x);
  bstree->size--;

  return 1;
}

size_t
bstree_size(bstree_t *bstree) {
  return bstree->size;
}

bool
bstree_empty(bstree_t *bstree) {
  return !bstree->begin;
}

collection_compare_t
bstree_value_compare(bstree_t *bstree) {
  return bstree->compare;
}

collection_value_free_t
bstree_value_free(bstree_t *bstree) {
  return bstree->value_free;
}

bstree_t *
bstree_new(collection_compare_t compare, collection_value_free_t value_free) {
  bstree_t *bstree = NULL;

  if (!(bstree = (bstree_t *)calloc(1, sizeof(bstree_t))))
    return NULL;

  bstree->compare = compare;
  bstree->value_free = value_free;

  return bstree;
}

void
bstree_free(bstree_t *bstree) {
  if (bstree) {
    struct bstree_node *__x = NULL, *__y = NULL;

    for (__x = bstree->root; bstree->size;) {
      if (!__x->left && !__x->right) {
        __y = __x->parent;

        if (__y) {
          if (__y->left == __x)
            __y->left = NULL;
          else
            __y->right = NULL;
        }

        bstree_node_free(bstree, __x);
        bstree->size--;
        __x = __y;
      } else if (__x->left)
        __x = __x->left;
      else if (__x->right)
        __x = __x->right;
    }

    free(bstree);
  }
}
