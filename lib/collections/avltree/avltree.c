#include "avltree.h"

#include <stdlib.h>
#include <string.h>

struct avltree_node {
  struct avltree_node *parent, *left, *right;
  collection_value_t value;

  size_t height;
};

struct avltree {
  struct avltree_node *root, *begin, *end;
  size_t size;

  collection_compare_t compare;
  collection_value_free_t free;
};


static struct avltree_node *
avltree_node_new(void *value, size_t size, bool copy, bool free) {
  struct avltree_node *node = NULL;

  if (!(node = (struct avltree_node *)calloc(1, sizeof(struct avltree_node))))
    return NULL;

  node->value.ptr  = ((node->value.copy = copy) ? collection_value_copy(value, size) : value);
  node->value.size = size;
  node->value.free = free;

  return node;
}

static inline void
avltree_node_value_free(avltree_t *avltree, struct avltree_node *node) {
  if (node) {
    if (avltree->free && node->value.free && node->value.ptr)
      avltree->free(node->value.ptr), node->value.ptr = NULL;
  }
}

static inline void
avltree_node_free(avltree_t *avltree, struct avltree_node *node) {
  if (node) {
    avltree_node_value_free(avltree, node);
    free(node);
  }
}

static inline size_t
avltree_node_height(struct avltree_node *__x) {
  return ((__x) ? __x->height : 0);
}

static inline long
avltree_node_balance_factor(struct avltree_node *__x) {
  return ((__x) ? (long)(avltree_node_height(__x->left)-avltree_node_height(__x->right)) : 0);
}

static inline void
avltree_node_update_height(struct avltree_node *__x) {
  size_t lheight = avltree_node_height(__x->left), rheight = avltree_node_height(__x->right);

  __x->height = ((lheight > rheight) ? lheight : rheight)+1;
}

static struct avltree_node *
avltree_node_rotate_right(avltree_t *avltree, struct avltree_node *__y) {
  struct avltree_node *__x = __y->left, *__z = __x->right;

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
  if (__z)
    __z->parent = __y;

  avltree_node_update_height(__y);
  avltree_node_update_height(__x);

  if (__y == avltree->root)
    avltree->root = __x;

  return __x;
}

struct avltree_node *
avltree_node_rotate_left(avltree_t *avltree, struct avltree_node * __x) {
  struct avltree_node *__y = __x->right, *__z = __y->left;

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
  if (__z)
    __z->parent = __x;

  avltree_node_update_height(__x);
  avltree_node_update_height(__y);

  if (__x == avltree->root)
    avltree->root = __y;

  return __y;
}

static struct avltree_node *
avltree_node_right_most_up(struct avltree_node *__x) {
  struct avltree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->right;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct avltree_node *
avltree_node_left_most_up(struct avltree_node *__x) {
  struct avltree_node *__y = NULL;

  for (__y = __x->parent; __y && __x == __y->left;) {
    __x = __y;
    __y = __y->parent;
  }

  return __y;
}

static struct avltree_node *
avltree_node_right_most(struct avltree_node *node) {
  struct avltree_node *__x = NULL;

  for (__x = node; __x->right;)
    __x = __x->right;

  return __x;
}

static struct avltree_node *
avltree_node_left_most(struct avltree_node *node) {
  struct avltree_node *__x = NULL;

  for (__x = node; __x->left;)
    __x = __x->left;

  return __x;
}

avltree_reverse_iterator_t
avltree_rnext(avltree_reverse_iterator_t iterator) {
  struct avltree_node *node = (struct avltree_node *)iterator;
  return (avltree_reverse_iterator_t)((node) ? ((node->left) ? avltree_node_right_most(node->left) : avltree_node_left_most_up(node)) : NULL);
}

avltree_reverse_iterator_t
avltree_rprev(avltree_reverse_iterator_t iterator) {
  struct avltree_node *node = (struct avltree_node *)iterator;
  return (avltree_reverse_iterator_t)((node) ? ((node->right) ? avltree_node_left_most(node->right) : avltree_node_right_most_up(node)) : NULL);
}

avltree_reverse_iterator_t
avltree_rbegin(avltree_t *avltree) {
  return (avltree_reverse_iterator_t)avltree->end;
}

avltree_reverse_iterator_t
avltree_rend(avltree_t *avltree) {
  return (avltree_reverse_iterator_t)avltree->begin;
}

avltree_iterator_t
avltree_next(avltree_iterator_t iterator) {
  struct avltree_node *node = (struct avltree_node *)iterator;
  return (avltree_iterator_t)((node) ? ((node->right) ? avltree_node_left_most(node->right) : avltree_node_right_most_up(node)) : NULL);
}

avltree_iterator_t
avltree_prev(avltree_iterator_t iterator) {
  struct avltree_node *node = (struct avltree_node *)iterator;
  return (avltree_iterator_t)((node) ? ((node->left) ? avltree_node_right_most(node->left) : avltree_node_left_most_up(node)) : NULL);
}

avltree_iterator_t
avltree_begin(avltree_t *avltree) {
  return (avltree_iterator_t)avltree->begin;
}

avltree_iterator_t
avltree_end(avltree_t *avltree) {
  return (avltree_iterator_t)avltree->end;
}

void *
avltree_value(avltree_iterator_t iterator) {
  return ((iterator) ? ((struct avltree_node *)iterator)->value.ptr : NULL);
}

size_t
avltree_value_size(avltree_iterator_t iterator) {
  return ((iterator) ? ((struct avltree_node *)iterator)->value.size : 0);
}

static struct avltree_node *
avltree_node_search(avltree_t *avltree, const void *value, size_t size) {
  struct avltree_node *__x = NULL;
  int compare = -1;

  for (__x = avltree->root; __x;)
    if ((compare = avltree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return __x;

  return NULL;
}

void *
avltree_findif(avltree_t *avltree, avltree_predicate_t predicate, ...) {
  struct avltree_node *__x = NULL;
  int compare;

  va_list args;

  for (__x = avltree->root; __x;) {
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
avltree_find(avltree_t *avltree, const void *value, size_t size) {
  struct avltree_node *node = NULL;

  if (!(node = avltree_node_search(avltree, value, size)))
    return NULL;

  return node->value.ptr;
}

static struct avltree_node *
avltree_node_search_leaf(avltree_t *avltree, const void *value, size_t size) {
  struct avltree_node *__x = NULL, *__y = NULL;
  int compare = -1;

  for (__x = avltree->root; __x;) {
    __y = __x;

    if ((compare = avltree->compare(value, __x->value.ptr)) == -1)
      __x = __x->left;
    else if (compare == 1)
      __x = __x->right;
    else
      return NULL;
  }

  return __y;
}

int
avltree_insert(avltree_t *avltree, void *value, size_t size, bool copy, bool free) {
  struct avltree_node *__x = NULL, *__y = NULL;
  long balance;

  if (avltree->root && !(__y = avltree_node_search_leaf(avltree, value, size)))
    return -1;

  avltree->size++;

  if (!avltree->root) {
    if (!(__x = avltree_node_new(value, size, copy, free)))
      return -1;
    avltree->root = avltree->begin = avltree->end = __x;
    __x->height = 1;

    return 1;
  }

  if (!(__x = avltree_node_new(value, size, copy, free)))
    return -1;

  if (avltree->begin && avltree->compare(value, avltree->begin->value.ptr) == -1)
    avltree->begin = __x;
  if (avltree->end && avltree->compare(avltree->end->value.ptr, value) == -1)
    avltree->end = __x;

  __x->parent = __y;
  if (avltree->compare(value, __y->value.ptr) == -1)
    __y->left = __x;
  else
    __y->right = __x;

  avltree_node_update_height(__x);

  while (__y) {
    avltree_node_update_height(__y);

    balance = avltree_node_balance_factor(__y);

    if (balance > 1 && avltree->compare(value, __y->left->value.ptr) == -1) {
      __y = avltree_node_rotate_right(avltree, __y);
    } else if (balance < -1 && avltree->compare(value, __y->right->value.ptr) == 1) {
      __y = avltree_node_rotate_left(avltree, __y);
    } else if (balance > 1 && avltree->compare(value, __y->left->value.ptr) == 1) {
      __y->left = avltree_node_rotate_left(avltree, __y->left);
      __y = avltree_node_rotate_right(avltree, __y);
    } else if (balance < -1 && avltree->compare(value, __y->right->value.ptr) == -1) {
      __y->right = avltree_node_rotate_right(avltree, __y->right);
      __y = avltree_node_rotate_left(avltree, __y);
    }

    __y = __y->parent;
  }

  return 1;
}

/*
static inline void
avltree_node_copy_value(struct avltree_node *__x, struct avltree_node *__y) {
  __x->value.ptr  = __y->value.ptr;
  __x->value.size = __y->value.size;
  __x->value.copy = __y->value.copy;
  __x->value.free = __y->value.free;
}
*/

static inline void
avltree_node_transfer(avltree_t *avltree, struct avltree_node *__x, struct avltree_node *__y) {
/*
  avltree_node_value_free(avltree, __y);

  avltree_node_copy_value(__y, __x);

  if (__x->parent->left == __x)
    __x->parent->left = __x->right;
  else
    __x->parent->right = __x->right;

// free(__x);
*/

  if (__y == avltree->root)
    avltree->root = __x;
  else if (__y == __y->parent->left)
    __y->parent->left = __x;
  else
    __y->parent->right = __x;

  if (__x)
    __x->parent = __y->parent;
}

/*
static struct avltree_node *
avltree_node_balance(avltree_t *avltree, struct avltree_node *__x) {
  long balance = avltree_node_balance_factor(__x);

  avltree_node_update_height(__x);

  if (balance > 1) {
    if (avltree_node_balance_factor(__x->left) < 0)
      __x->left = avltree_node_rotate_left(avltree, __x->left);
    
    return avltree_node_rotate_right(avltree, __x);
  }

  if (balance < -1) {
    if (avltree_node_balance_factor(__x->right) > 0)
      __x->right = avltree_node_rotate_right(avltree, __x->right);

    return avltree_node_rotate_left(avltree, __x);
  }

  return __x;
}

static void
avltree_node_balance_after_remove(avltree_t *avltree, struct avltree_node *__x) {
  while (__x != NULL) {
    __x = avltree_node_balance(avltree, __x);
    __x = __x->parent;
  }
}
*/

static void
avltree_node_balance_after_remove(avltree_t *avltree, struct avltree_node *__x) {
  long balance;

  while (__x != NULL) {
    balance = avltree_node_balance_factor(__x);
    avltree_node_update_height(__x);

    if (balance > 1) {
      if (avltree_node_balance_factor(__x->left) < 0)
        __x->left = avltree_node_rotate_left(avltree, __x->left);
    
      __x = avltree_node_rotate_right(avltree, __x);
    }

    if (balance < -1) {
      if (avltree_node_balance_factor(__x->right) > 0)
        __x->right = avltree_node_rotate_right(avltree, __x->right);

      __x = avltree_node_rotate_left(avltree, __x);
    }

    __x = __x->parent;
  }
}

int
avltree_remove(avltree_t *avltree, const void *value, size_t size) {
  struct avltree_node *__x = NULL, *__y = NULL, *__z = NULL;

  if (!(__x = avltree_node_search(avltree, value, size)))
    return -1;
  __y = __x->parent;

  if (!__x->left && !__x->right) {
    if (__x == avltree->root)
      avltree->root = avltree->begin = avltree->end = NULL;
    else if (__x == __y->left)
      __y->left = NULL;
    else
      __y->right = NULL;

    if (__x == avltree->begin)
      avltree->begin = avltree_node_left_most(avltree->root);
    if (__x == avltree->end)
      avltree->end = avltree_node_right_most(avltree->root);

    avltree_node_balance_after_remove(avltree, __x);
 // avltree_node_free(avltree, __x);
  } else if (!__x->left || !__x->right) {
    __z = ((__x->left) ? __x->left : __x->right);

    if (__x == avltree->root)
      avltree->root = __z, __z->parent = NULL;
    else if (__x == __y->left) {
      __y->left = __z;
      __z->parent = __y;
    } else {
      __y->right = __z;
      __z->parent = __y;
    }

    if (__x == avltree->begin)
      avltree->begin = avltree_node_left_most(avltree->root);
    if (__x == avltree->end)
      avltree->end = avltree_node_right_most(avltree->root);

    avltree_node_balance_after_remove(avltree, __x);
 // avltree_node_free(avltree, __x);
  } else {
    __y = avltree_node_left_most(__x->right);

    avltree_node_balance_after_remove(avltree, __y);

 // avltree_node_transfer(avltree, __y, __x);
    avltree_node_transfer(avltree, __x, __y);

    if (__y == avltree->begin)
      avltree->begin = __x;
    if (__y == avltree->end)
      avltree->end = __x;

    __x = __y;
  }

  avltree_node_free(avltree, __x);
  avltree->size--;

  return 1;
}

size_t
avltree_size(avltree_t *avltree) {
  return avltree->size;
}

bool
avltree_empty(avltree_t *avltree) {
  return !avltree->root;
}

avltree_t *
avltree_new(collection_compare_t compare, collection_value_free_t free) {
  avltree_t *avltree = NULL;

  if (!(avltree = (avltree_t *)calloc(1, sizeof(avltree_t))))
    return NULL;

  avltree->compare = compare;
  avltree->free = free;

  return avltree;
}

void
avltree_free(avltree_t *avltree) {
  if (avltree) {
    struct avltree_node *__x = NULL, *__y = NULL;

    for (__x = avltree->root; __x; avltree->size--) {
      if (!__x->left && !__x->right) {
        __y = __x->parent;

        if (__y) {
          if (__y->left == __x)
            __y->left = NULL;
          else
            __y->right = NULL;
        }

        avltree_node_free(avltree, __x);

        __x = __y;
      } else if (__x->left)
        __x = __x->left;
      else if (__x->right)
        __x = __x->right;
    }

    free(avltree);
  }
}
