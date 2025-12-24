#ifndef _COLLECTIONS_HASHTABLE_HASHTABLE_H
#define _COLLECTIONS_HASHTABLE_HASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <collections/collections.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct hashtable hashtable_t;

typedef struct hashtable_iterator * hashtable_iterator_t;
typedef hashtable_iterator_t hashtable_reverse_iterator_t;

typedef uint64_t (*hashtable_hasher_t)(const uint8_t *, size_t);

typedef bool (*hashtable_predicate_t)(const void *, const void *, va_list);


extern hashtable_reverse_iterator_t hashtable_rnext(hashtable_reverse_iterator_t);
extern hashtable_reverse_iterator_t hashtable_rprev(hashtable_reverse_iterator_t);

extern hashtable_reverse_iterator_t hashtable_rbegin(hashtable_t *);
extern hashtable_reverse_iterator_t hashtable_rend(hashtable_t *);

extern hashtable_iterator_t hashtable_next(hashtable_iterator_t);
extern hashtable_iterator_t hashtable_prev(hashtable_iterator_t);

extern hashtable_iterator_t hashtable_begin(hashtable_t *);
extern hashtable_iterator_t hashtable_end(hashtable_t *);

extern collection_value_traits_t hashtable_key_traits(hashtable_iterator_t);
extern void *hashtable_key(hashtable_iterator_t);
extern size_t hashtable_key_size(hashtable_iterator_t);

extern collection_value_traits_t hashtable_value_traits(hashtable_iterator_t);
extern void *hashtable_value(hashtable_iterator_t);
extern size_t hashtable_value_size(hashtable_iterator_t);

#define hashtable_lkey    collection_lvalue
#define hashtable_clkey   collection_clvalue

#define hashtable_rkey    collection_rvalue
#define hashtable_crkey   collection_crvalue

#define hashtable_lvalue  collection_lvalue
#define hashtable_clvalue collection_clvalue

#define hashtable_rvalue  collection_rvalue
#define hashtable_crvalue collection_crvalue

extern int hashtable_insert(hashtable_t *, void *, size_t, bool, bool, void *, size_t, bool, bool);
extern int hashtable_remove(hashtable_t *, const void *, size_t);

extern void *hashtable_get(hashtable_t *, const void *, size_t);
extern void *hashtable_getor(hashtable_t *, const void *, size_t, void *);

extern void *hashtable_findif(hashtable_t *, hashtable_predicate_t, ...);

extern size_t hashtable_size(hashtable_t *);
extern bool hashtable_empty(hashtable_t *);

extern collection_value_free_t hashtable_key_free(hashtable_t *);
extern collection_value_free_t hashtable_value_free(hashtable_t *);

extern hashtable_hasher_t hashtable_hasher(hashtable_t *);

extern hashtable_t *hashtable_new(hashtable_hasher_t, collection_value_free_t, collection_value_free_t);
extern void hashtable_free(hashtable_t *);

#ifdef __cplusplus
}
#endif

#endif
