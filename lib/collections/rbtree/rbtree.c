#include "rbtree.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
  BLACK, RED
} rbtree_color_t;

struct rbtree_node {
  struct rbtree_node *parent, *left, *right;
  rbtree_color_t color;

  collection_value_t value;
};


#define RBTREE_NULL_LEAF &rbtree_null_leaf

static struct rbtree_node rbtree_null_leaf = { NULL, NULL, NULL, BLACK, { (void *)"(null)", 0, false, false } };


struct rbtree {
  struct rbtree_node *root, *begin, *end;
  size_t size;

  collection_compare_t compare;
  collection_value_free_t value_free;
};


static struct rbtree_node *
rbtree_node_new(void *value, size_t size, bool copy, bool free, rbtree_color_t color) {
  struct rbtree_node *node = NULL;

  if (!(node = (struct rbtree_node *)calloc(1, sizeof(struct rbtree_node))))
    return NULL;
  node->left  = RBTREE_NULL_LEAF;
  node->right = RBTREE_NULL_LEAF;
  node->color = color;

  node->value.ptr  = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  return node;
}

static inline void
rbtree_node_free(rbtree_t *rbtree, struct rbtree_node *node) {
  if (node) {
    if (rbtree->value_free && node->value.free && node->value.ptr)
      rbtree->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

static struct rbtree_node *
rbtree_node_rotate_right(rbtree_t *rbtree, struct rbtree_node *__y) {
  struct rbtree_node *__x = __y->left, *__z = __x->right;

  __x->parent = __y->parent;
  if (__y->parent) {
    if (__y->parent->left == __y)
      __y->parent->left = __x;
    else
      __y->parent->right = __x;
  }

  __x->right = __y;
  __y->left = __z;

  __y->parent = __x;

  if (__z && __z != RBTREE_NULL_LEAF)
    __z->parent = __y;

  if (__y == rbtree->root)
    rbtree->root = __x;

  return __x;
}

static struct rbtree_node *
rbtree_node_rotate_left(rbtree_t *rbtree, struct rbtree_node * __x) {
  struct rbtree_node *__y = __x->right, *__z = __y->left;

  __y->parent = __x->parent;
  if (__x->parent) {
    if (__x->parent->left == __x)
      __x->parent->left = __y;
    else
      __x->parent->right = __y;
  }

  __y->left = __x;
  __x->right = __z;

  __x->parent = __y;

  if (__z && __z != RBTREE_NULL_LEAF)
    __z->parent = __x;

  if (__x == rbtree->root)
    rbtree->root = __y;

  return __y;
}

static struct rbtree_node *
rbtree_node_right_most_up(struct rbtree_node *__x) {
  struct rbtree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->right;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct rbtree_node *
rbtree_node_left_most_up(struct rbtree_node *__x) {
  struct rbtree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->left;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct rbtree_node *
rbtree_node_right_most(struct rbtree_node *__x) {
  struct rbtree_node *__y = NULL;

  for (__y = __x; __y != RBTREE_NULL_LEAF && __y->right != RBTREE_NULL_LEAF;)
    __y = __y->right;

  return ((__y != RBTREE_NULL_LEAF) ? __y : NULL);
}

static struct rbtree_node *
rbtree_node_left_most(struct rbtree_node *__x) {
  struct rbtree_node *__y = NULL;

  for (__y = __x; __y != RBTREE_NULL_LEAF && __y->left != RBTREE_NULL_LEAF;)
    __y = __y->left;

  return ((__y != RBTREE_NULL_LEAF) ? __y : NULL);
}

rbtree_reverse_iterator_t
rbtree_rnext(rbtree_reverse_iterator_t iterator) {
  struct rbtree_node *node = (struct rbtree_node *)iterator;
  return (rbtree_reverse_iterator_t)((node) ? ((node->left != RBTREE_NULL_LEAF) ? rbtree_node_right_most(node->left) : rbtree_node_left_most_up(node)) : NULL);
}

rbtree_reverse_iterator_t
rbtree_rprev(rbtree_reverse_iterator_t iterator) {
  struct rbtree_node *node = (struct rbtree_node *)iterator;
  return (rbtree_reverse_iterator_t)((node) ? ((node->right != RBTREE_NULL_LEAF) ? rbtree_node_left_most(node->right) : rbtree_node_right_most_up(node)) : NULL);
}

rbtree_reverse_iterator_t
rbtree_rbegin(rbtree_t *rbtree) {
  return (rbtree_reverse_iterator_t)rbtree->end;
}

rbtree_reverse_iterator_t
rbtree_rend(rbtree_t *rbtree) {
  return (rbtree_reverse_iterator_t)rbtree->begin;
}

rbtree_iterator_t
rbtree_next(rbtree_iterator_t iterator) {
  struct rbtree_node *node = (struct rbtree_node *)iterator;
  return (rbtree_iterator_t)((node) ? ((node->right != RBTREE_NULL_LEAF) ? rbtree_node_left_most(node->right) : rbtree_node_right_most_up(node)) : NULL);
}

rbtree_iterator_t
rbtree_prev(rbtree_iterator_t iterator) {
  struct rbtree_node *node = (struct rbtree_node *)iterator;
  return (rbtree_iterator_t)((node) ? ((node->left != RBTREE_NULL_LEAF) ? rbtree_node_right_most(node->left) : rbtree_node_left_most_up(node)) : NULL);
}

rbtree_iterator_t
rbtree_begin(rbtree_t *rbtree) {
  return (rbtree_iterator_t)rbtree->begin;
}

rbtree_iterator_t
rbtree_end(rbtree_t *rbtree) {
  return (rbtree_iterator_t)rbtree->end;
}

void *
rbtree_value(rbtree_iterator_t iterator) {
  return ((iterator) ? ((struct rbtree_node *)iterator)->value.ptr : NULL);
}

size_t
rbtree_value_size(rbtree_iterator_t iterator) {
  return ((iterator) ? ((struct rbtree_node *)iterator)->value.size : 0);
}

static struct rbtree_node *
rbtree_search(rbtree_t *rbtree, const void *value, size_t size) {
  struct rbtree_node *__x = NULL;
  int compare;

  for (__x = rbtree->root; __x && __x != RBTREE_NULL_LEAF;) {
    if ((compare = rbtree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return __x;
  }

  return NULL;
}

/*
void *
rbtree_findif(rbtree_t *rbtree, collection_predicate_t predicate, ...) {
  struct rbtree_node *__x = NULL, *__y = NULL;
  int compare;

  va_list args;

  va_start(args, predicate);

  for (__x = rbtree->root; __x && __x != RBTREE_NULL_LEAF;) {
    if (predicate(__x->value.ptr, args))
      return __x->value.ptr;

    if ((compare = rbtree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
  }

  return NULL;
}
*/

void *
rbtree_findif(rbtree_t *rbtree, rbtree_predicate_t predicate, ...) {
  struct rbtree_node *__x = NULL;
  int compare;

  va_list args;

  for (__x = rbtree->root; __x && __x != RBTREE_NULL_LEAF;) {
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
rbtree_find(rbtree_t *rbtree, const void *value, size_t size) {
  struct rbtree_node *node = NULL;

  if (!(node = rbtree_search(rbtree, value, size)))
    return NULL;

  return node->value.ptr;
}

static struct rbtree_node *
rbtree_node_search_leaf(rbtree_t *rbtree, const void *value, size_t size) {
  struct rbtree_node *__x = NULL, *__y = NULL;
  int compare;

  for (__x = rbtree->root; __x != RBTREE_NULL_LEAF;) {
    __y = __x;

    if ((compare = rbtree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return NULL;
  }

  return __y;
}

static inline void
rbtree_node_balance_after_insert(rbtree_t *rbtree, struct rbtree_node *__x) {
  while (__x != rbtree->root && __x->parent->color != BLACK) { 
    if (__x->parent == __x->parent->parent->left) {
      struct rbtree_node *__y = __x->parent->parent->right;

      if (__y->color == RED) {
        __x->parent->color = BLACK;
        __y->color = BLACK;
        __x->parent->parent->color = RED;
        __x = __x->parent->parent;
      } else {
        if (__x == __x->parent->right) {
          __x = __x->parent;
          rbtree_node_rotate_left(rbtree, __x);
        }

        __x->parent->color = BLACK;
        __x->parent->parent->color = RED;
        rbtree_node_rotate_right(rbtree, __x->parent->parent);
      }
    } else {
      struct rbtree_node *__y = __x->parent->parent->left;

      if (__y->color == RED) {
        __x->parent->color = BLACK;
        __y->color = BLACK;
        __x->parent->parent->color = RED;
        __x = __x->parent->parent;
      } else {
        if (__x == __x->parent->left) {
          __x = __x->parent;
          rbtree_node_rotate_right(rbtree, __x);
        }

        __x->parent->color = BLACK;
        __x->parent->parent->color = RED;
        rbtree_node_rotate_left(rbtree, __x->parent->parent);
      }
    }
  }

  rbtree->root->color = BLACK;
}

int
rbtree_insert(rbtree_t *rbtree, void *value, size_t size, bool copy, bool free) {
  struct rbtree_node *__x = NULL, *__y = NULL;

  if (rbtree->root && !(__y = rbtree_node_search_leaf(rbtree, value, size)))
    return -1;

  rbtree->size++;

  if (!rbtree->root) {
    if (!(__x = rbtree_node_new(value, size, copy, free, BLACK)))
      return -1;
    rbtree->root = rbtree->begin = rbtree->end = __x;

    return 1;
  }

  if (!(__x = rbtree_node_new(value, size, copy, free, RED)))
    return -1;

  if (rbtree->begin && rbtree->compare(value, rbtree->begin->value.ptr) == -1)
    rbtree->begin = __x;
  if (rbtree->end && rbtree->compare(rbtree->end->value.ptr, value) == -1)
    rbtree->end = __x;

  __x->parent = __y;
  if (rbtree->compare(value, __y->value.ptr) == -1)
    __y->left = __x;
  else
    __y->right = __x;

  rbtree_node_balance_after_insert(rbtree, __x);
  return 1;
}

static inline void
rbtree_node_transfer(rbtree_t *rbtree, struct rbtree_node *__x, struct rbtree_node *__y) {
  if (__y == rbtree->root)
    rbtree->root = __x;
  else if (__y == __y->parent->left)
    __y->parent->left = __x;
  else
    __y->parent->right = __x;

  if (__x)
    __x->parent = __y->parent;
}

int
rbtree_remove(rbtree_t *rbtree, const void *value, size_t size) {
  struct rbtree_node *__w = NULL, *__x = NULL, *__y = NULL;
  rbtree_color_t removed_node_color;

  if (!(__x = rbtree_search(rbtree, value, size)))
    return -1;

  if (__x->left != RBTREE_NULL_LEAF && __x->right != RBTREE_NULL_LEAF)
    __w = rbtree_node_right_most(__x->left);
  else
    __w = __x;

  __y = ((__w->left != RBTREE_NULL_LEAF) ? __w->left : __w->right);
  removed_node_color = __w->color;

  rbtree_node_transfer(rbtree, __y, __w);

  if (__x != __w) {
    if (__w->parent != __x) {
      rbtree_node_transfer(rbtree, __w->left, __w);
      __w->left = __x->left;
      __w->left->parent = __w;
    } else
      __y->parent = __w;

    rbtree_node_transfer(rbtree, __w, __x);

    __w->right = __x->right;
    __w->right->parent = __w;
    __w->color = __x->color;
  }

  if (__x == rbtree->begin)
    rbtree->begin = rbtree_node_left_most(rbtree->root);
  if (__x == rbtree->end)
    rbtree->end = rbtree_node_right_most(rbtree->root);
  if (rbtree->root == RBTREE_NULL_LEAF)
    rbtree->root = NULL;

  rbtree_node_free(rbtree, __x);
  rbtree->size--;

  if (removed_node_color == BLACK && rbtree->root) {
    while (__y != rbtree->root && __y->color == BLACK) {
      if (__y == __y->parent->left) {
        __x = __y->parent->right;

        if (__x->color == RED) {
          __x->color = BLACK;
          __y->parent->color = RED;
          rbtree_node_rotate_left(rbtree, __y->parent);
          __x = __y->parent->right;
        } else if (__x->left->color == BLACK && __x->right->color == BLACK) {
          __x->color = RED;
          __y = __y->parent;
        } else {
          if (__x->right->color == BLACK) {
            __x->left->color = BLACK;
            __x->color = RED;
            rbtree_node_rotate_right(rbtree, __x);
            __x = __y->parent->right;
          }

          __x->color = __y->parent->color;
          __y->parent->color = BLACK;
          __x->right->color = BLACK;
          rbtree_node_rotate_left(rbtree, __y->parent);
          __y = rbtree->root;
        }
      } else {
        __x = __y->parent->left;

        if (__x->color == RED) {
          __x->color = BLACK;
          __y->parent->color = RED;
          rbtree_node_rotate_right(rbtree, __y->parent);
          __x = __y->parent->left;
        } else if (__x->right->color == BLACK && __x->left->color == BLACK) {
          __x->color = RED;
          __y = __y->parent;
        } else {
          if (__x->left->color == BLACK) {
            __x->right->color = BLACK;
            __x->color = RED;
            rbtree_node_rotate_left(rbtree, __x);
            __x = __y->parent->left;
          }

          __x->color = __y->parent->color;
          __y->parent->color = BLACK;
          __x->left->color = BLACK;
          rbtree_node_rotate_right(rbtree, __y->parent);
          __y = rbtree->root;
        }
      }
    }

    __y->color = BLACK;
  }

  return 1;
}

size_t
rbtree_size(rbtree_t *rbtree) {
  return rbtree->size;
}

bool
rbtree_empty(rbtree_t *rbtree) {
  return !rbtree->size;
}

collection_compare_t
rbtree_value_compare(rbtree_t *rbtree) {
  return rbtree->compare;
}

collection_value_free_t
rbtree_value_free(rbtree_t *rbtree) {
  return rbtree->value_free;
}

rbtree_t *
rbtree_new(collection_compare_t compare, collection_value_free_t value_free) {
  rbtree_t *rbtree = NULL;

  if (!(rbtree = (rbtree_t *)calloc(1, sizeof(rbtree_t))))
    return NULL;

  rbtree->compare = compare;
  rbtree->value_free = value_free;

  return rbtree;
}

void
rbtree_free(rbtree_t *rbtree) {
  if (rbtree) {
    struct rbtree_node *__x = NULL, *__y = NULL;

    for (__x = rbtree->root; rbtree->size;) {
      if (__x->left == RBTREE_NULL_LEAF && __x->right == RBTREE_NULL_LEAF) {
        __y = __x->parent;

        if (__y) {
          if (__y->left == __x)
            __y->left = RBTREE_NULL_LEAF;
          else
            __y->right = RBTREE_NULL_LEAF;
        }

        rbtree_node_free(rbtree, __x);
        rbtree->size--;
        __x = __y;
      } else if (__x->left != RBTREE_NULL_LEAF)
        __x = __x->left;
      else if (__x->right != RBTREE_NULL_LEAF)
        __x = __x->right;
    }

    free(rbtree);
  }
}
