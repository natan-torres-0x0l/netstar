#include "hashtable.h"

#include <fnv1a/fnv1a.h>

#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdint.h>

#define HASHTABLE_INITIAL_CAPACITY 10

struct hashtable_bucket {
  struct hashtable_bucket *left, *right;
  hashtable_t *hashtable;

  collection_value_t key, value;
  uint64_t hash;
  size_t index;
};

struct hashtable {
  struct hashtable_bucket *begin, *end;
  struct hashtable_bucket **buckets;

  size_t capacity, size;

  collection_value_free_t key_free, value_free;
  hashtable_hasher_t hasher;
};


static inline bool
hashtable_requires_reallocation(hashtable_t *hashtable) {
  return hashtable->size == hashtable->capacity;
}

static inline size_t
hashtable_capacity_growth(size_t size) {
  return size*2;
}

static inline int
hashtable_reallocate(hashtable_t *hashtable, size_t capacity) {
  struct hashtable_bucket **buckets = hashtable->buckets;

  if (!(buckets = (struct hashtable_bucket **)calloc(capacity, sizeof(struct hashtable_bucket *))))
    return -1;
  memset(buckets+hashtable->capacity, 0, capacity*sizeof(struct hashtable_bucket *));
  hashtable->buckets = buckets;

  hashtable->capacity = capacity;
  return 1;
}

static inline int
hashtable_reallocate_capacity(hashtable_t *hashtable) {
  return hashtable_reallocate(hashtable, hashtable_capacity_growth(hashtable->size));
}

struct hashtable_bucket *
hashtable_bucket_new(hashtable_t *hashtable, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct hashtable_bucket *node = NULL;

  if (!(node = (struct hashtable_bucket *)calloc(1, sizeof(struct hashtable_bucket))))
    return NULL;

  node->key.ptr = ((node->key.copy = key_copy) ? collection_value_copy(key, key_size) : key);
  node->key.size = key_size;
  node->key.free = key_free;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;

  node->hash = hashtable->hasher((const uint8_t *)key, key_size);

  node->hashtable = hashtable;

  return node;
}

static inline void
hashtable_bucket_update(hashtable_t *hashtable, struct hashtable_bucket *node, void *value, size_t value_size, bool value_copy, bool value_free) {
  if (hashtable->value_free && node->value.free && node->value.ptr)
    hashtable->value_free(node->value.ptr), node->value.ptr = NULL;

  node->value.ptr = ((node->value.copy = value_copy) ? collection_value_copy(value, value_size) : value);
  node->value.size = value_size;
  node->value.free = value_free;
}

static inline void
hashtable_bucket_free(hashtable_t *hashtable, struct hashtable_bucket *node) {
  if (node) {
    if (hashtable->key_free && node->key.free && node->key.ptr)
      hashtable->key_free(node->key.ptr), node->key.ptr = NULL;

    if (hashtable->value_free && node->value.free && node->value.ptr)
      hashtable->value_free(node->value.ptr), node->value.ptr = NULL;

    free(node);
  }
}

static inline int
hashtable_bucket_remove(hashtable_t *hashtable, struct hashtable_bucket *node) {
  size_t index = node->index;

  hashtable_bucket_free(hashtable, node);
  hashtable->buckets[index] = NULL;

  return 1;
}

static struct hashtable_bucket *
hashtable_bucket_at_right(hashtable_t *hashtable, size_t nbucket) {
  for (; nbucket < hashtable->capacity; nbucket++)
    if (hashtable->buckets[nbucket])
      return hashtable->buckets[nbucket];

  return NULL;
}

static struct hashtable_bucket *
hashtable_bucket_at_left(hashtable_t *hashtable, size_t nbucket) {
  for (; nbucket; nbucket--)
    if (hashtable->buckets[nbucket])
      return hashtable->buckets[nbucket];

  return NULL;
}

hashtable_iterator_t
hashtable_next(hashtable_iterator_t iterator) {
/*
  hashtable_t *hashtable = ((struct hashtable_bucket *)(iterator))->hashtable;
  size_t nbucket = ((struct hashtable_bucket *)(iterator))->index;

  for (; index < hashtable->capacity; index++)
    if (hashtable->buckets[index])
      return (hashtable_iterator_t)hashtable->buckets[index];

  return NULL;
*/
  return ((iterator) ? (hashtable_iterator_t)hashtable_bucket_at_right(((struct hashtable_bucket *)iterator)->hashtable, ((struct hashtable_bucket *)iterator)->index+1) : NULL);
}

hashtable_iterator_t
hashtable_prev(hashtable_iterator_t iterator) {
/*
  hashtable_t *hashtable = ((struct hashtable_bucket *)(iterator))->hashtable;
  size_t nbucket = ((struct hashtable_bucket *)(iterator))->index;

  for (; index; index--)
    if (hashtable->buckets[index])
      return (hashtable_iterator_t)hashtable->buckets[index];

  return NULL;
*/
  return ((iterator) ? (hashtable_iterator_t)hashtable_bucket_at_left(((struct hashtable_bucket *)iterator)->hashtable, ((struct hashtable_bucket *)iterator)->index-1) : NULL);
}

hashtable_iterator_t
hashtable_begin(hashtable_t *hashtable) {
  return (!hashtable_empty(hashtable) ? (hashtable_iterator_t)hashtable_bucket_at_right(hashtable, 0) : NULL);
}

hashtable_iterator_t
hashtable_end(hashtable_t *hashtable) {
  return (!hashtable_empty(hashtable) ? (hashtable_iterator_t)hashtable_bucket_at_left(hashtable, hashtable->capacity) : NULL);
}


/*
 * @ experimental
 */

hashtable_reverse_iterator_t
hashtable_rnext(hashtable_reverse_iterator_t iterator) {
  return ((iterator) ? (hashtable_reverse_iterator_t)((struct hashtable_bucket *)iterator)->left : NULL);
}

hashtable_reverse_iterator_t
hashtable_rprev(hashtable_reverse_iterator_t iterator) {
  return ((iterator) ? (hashtable_reverse_iterator_t)((struct hashtable_bucket *)iterator)->right : NULL);
}

hashtable_reverse_iterator_t
hashtable_rbegin(hashtable_t *hashtable) {
  return (hashtable_reverse_iterator_t)hashtable->end;
}

hashtable_reverse_iterator_t
hashtable_rend(hashtable_t *hashtable) {
  return (hashtable_reverse_iterator_t)hashtable->begin;
}

/*
hashtable_iterator_t
hashtable_next(hashtable_iterator_t iterator) {
  return ((iterator) ? (hashtable_iterator_t)((struct hashtable_bucket *)iterator)->right : NULL);
}

hashtable_iterator_t
hashtable_prev(hashtable_iterator_t iterator) {
  return ((iterator) ? (hashtable_iterator_t)((struct hashtable_bucket *)iterator)->left : NULL);
}

hashtable_iterator_t
hashtable_begin(hashtable_t *hashtable) {
  return (hashtable_iterator_t)hashtable->begin;
}

hashtable_iterator_t
hashtable_end(hashtable_t *hashtable) {
  return (hashtable_iterator_t)hashtable->end;
}
*/

void *
hashtable_key(hashtable_iterator_t iterator) {
  return ((iterator) ? ((struct hashtable_bucket *)iterator)->key.ptr : NULL);
}

size_t
hashtable_key_size(hashtable_iterator_t iterator) {
  return ((iterator) ? ((struct hashtable_bucket *)iterator)->key.size : 0);
}

void *
hashtable_value(hashtable_iterator_t iterator) {
  return ((iterator) ? ((struct hashtable_bucket *)iterator)->value.ptr : NULL);
}

size_t
hashtable_value_size(hashtable_iterator_t iterator) {
  return ((iterator) ? ((struct hashtable_bucket *)iterator)->value.size : 0);
}

static inline bool
hashtable_bucket_key_compare(struct hashtable_bucket *node, const void *key, size_t key_size) {
  return (node->key.size == key_size && !memcmp(node->key.ptr, key, node->key.size));
}

static struct hashtable_bucket *
hashtable_bucket_search(hashtable_t *hashtable, const void *key, size_t key_size) {
  uint64_t hash = fnv1a_hash((const uint8_t *)key, key_size) % hashtable->capacity;
  uint64_t source_hash = hash;

  while (hashtable->buckets[hash]) {
    if (hashtable_bucket_key_compare(hashtable->buckets[hash], key, key_size))
      return hashtable->buckets[hash];

    hash = (hash+1) % hashtable->capacity;
    if (hash == source_hash)
      break;
  }

  return NULL;
}

static void
hashtable_bucket_linear_probing_insert(hashtable_t *hashtable, struct hashtable_bucket *node) {
  uint64_t hash = node->hash % hashtable->capacity;

  while (hashtable->buckets[hash])
    hash = (hash+1) % hashtable->capacity;

  node->index = (size_t)hash;
  hashtable->buckets[hash] = node;
}

int
hashtable_insert(hashtable_t *hashtable, void *key, size_t key_size, bool key_copy, bool key_free, void *value, size_t value_size, bool value_copy, bool value_free) {
  struct hashtable_bucket *node = NULL;

  if ((node = hashtable_bucket_search(hashtable, key, key_size))) {
    hashtable_bucket_update(hashtable, node, value, value_size, value_copy, value_free);
    return 1;
  }

  if (hashtable_requires_reallocation(hashtable) && hashtable_reallocate_capacity(hashtable) == -1)
    return -1;

  if (!(node = hashtable_bucket_new(hashtable, key, key_size, key_copy, key_free, value, value_size, value_copy, value_free)))
    return -1;

  hashtable_bucket_linear_probing_insert(hashtable, node);
  hashtable->size++;

  if (hashtable->begin) {
    hashtable->end->right = node;
    node->left = hashtable->end;
    hashtable->end = node;

    return 1;
  }

  hashtable->begin = hashtable->end = node;
  return 1;
}

static inline void
hashtable_bucket_readjust(hashtable_t *hashtable, struct hashtable_bucket *node) {
  struct hashtable_bucket *left = NULL, *right = NULL;

  left = node->left;
  right = node->right;

  if (node == hashtable->begin && right) {
    right->left = NULL;
    hashtable->begin = right;
  } else if (node == hashtable->end && left) {
    left->right = NULL;
    hashtable->end = left;
  } else if (left && right) {
    left->right = right;
    right->left = left;
  } else {
    hashtable->begin = NULL;
    hashtable->end = NULL;
  }
}

static void
hashtable_bucket_linear_probing_remove(hashtable_t *hashtable, struct hashtable_bucket *node) {
  uint64_t hash = (node->index+1) % hashtable->capacity;

  hashtable_bucket_remove(hashtable, node);

  while (hashtable->buckets[hash]) {
    node = hashtable->buckets[hash];

    hashtable_bucket_readjust(hashtable, node);

    hashtable_insert(hashtable, node->key.ptr, node->key.size, node->key.copy, node->key.free,
                                node->value.ptr, node->value.size, node->value.copy, node->value.free);

    hashtable_bucket_remove(hashtable, node);
    hashtable->size--;

    hash = (hash+1) % hashtable->capacity;
  }
}

int
hashtable_remove(hashtable_t *hashtable, const void *key, size_t key_size) {
  struct hashtable_bucket *node = NULL;

  if (!(node = hashtable_bucket_search(hashtable, key, key_size)))
    return -1;

  hashtable_bucket_readjust(hashtable, node);

  hashtable_bucket_linear_probing_remove(hashtable, node);
  hashtable->size--;

  return 1;
}

void *
hashtable_get(hashtable_t *hashtable, const void *key, size_t key_size) {
  struct hashtable_bucket *node = NULL;

  if (!(node = hashtable_bucket_search(hashtable, key, key_size)))
    return NULL;

  return node->value.ptr;
}

void *
hashtable_getor(hashtable_t *hashtable, const void *key, size_t key_size, void *value_default) {
  void *value = hashtable_get(hashtable, key, key_size);
  return ((value) ? value : value_default);
}

void *
hashtable_findif(hashtable_t *hashtable, hashtable_predicate_t predicate, ...) {
  struct hashtable_bucket *node = NULL;
  va_list args;

  for (node = hashtable->begin; node; node = node->right) {
    va_start(args, predicate);

    if (predicate(node->key.ptr, node->value.ptr, args))
      return node->value.ptr;

    va_end(args);
  }

  return NULL;
}

size_t
hashtable_size(hashtable_t *hashtable) {
  return hashtable->size;
}

bool
hashtable_empty(hashtable_t *hashtable) {
  return !hashtable->begin;
}

collection_value_free_t
hashtable_key_free(hashtable_t *hashtable) {
  return hashtable->key_free;
}

collection_value_free_t
hashtable_value_free(hashtable_t *hashtable) {
  return hashtable->value_free;
}

hashtable_hasher_t
hashtable_hasher(hashtable_t *hashtable) {
  return hashtable->hasher;
}

hashtable_t *
hashtable_new(hashtable_hasher_t hasher, collection_value_free_t key_free, collection_value_free_t value_free) {
  hashtable_t *hashtable = NULL;

  if (!(hashtable = (hashtable_t *)calloc(1, sizeof(struct hashtable))))
    goto _return;

  hashtable->capacity = HASHTABLE_INITIAL_CAPACITY;

  if (!(hashtable->buckets = (struct hashtable_bucket **)calloc(hashtable->capacity, sizeof(struct hashtable_bucket *))))
    goto _return;

  hashtable->hasher = hasher;

  hashtable->key_free = key_free;
  hashtable->value_free = value_free;

  return hashtable;

_return:
  hashtable_free(hashtable);
  return NULL;
}

void
hashtable_free(hashtable_t *hashtable) {
  if (hashtable) {
    if (hashtable->buckets) {
      size_t nbucket;

      for (nbucket = 0; nbucket < hashtable->capacity; nbucket++)
        hashtable_bucket_free(hashtable, hashtable->buckets[nbucket]);

      free(hashtable->buckets);
    }

    free(hashtable);
  }
}
