#ifndef _COLLECTIONS_COLLECTIONS_H
#define _COLLECTIONS_COLLECTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include <stdarg.h>


typedef void * collection_t;

typedef enum {
   COLLECTION_AVLTREE, COLLECTION_BINARY_HEAP, COLLECTION_BSTREE, COLLECTION_DEQUE, COLLECTION_HASHSET, COLLECTION_HASHMAP,
   COLLECTION_HASHTABLE, COLLECTION_LIST, COLLECTION_MAP, COLLECTION_MULTIMAP, COLLECTION_MULTISET, COLLECTION_PRIORITY_QUEUE,
   COLLECTION_QUEUE, COLLECTION_RBTREE, COLLECTION_SET, COLLECTION_STACK, COLLECTION_UNORDERED_MAP, COLLECTION_UNORDERED_SET
} collection_container_t;


typedef int (*collection_compare_t)(const void *, const void *);

typedef bool (*collection_predicate_t)(const void *, va_list);

// typedef void (*collection_operation_t)(void *);


#define collection_value(value, size, copy, free) (void *)value, (size_t)size, (bool)copy, (bool)free

// @ rvalue  : not deallocatable value reference - use the value reference and do not free it when destroying the collection
#define collection_rvalue(value, size)  collection_value(value, size, false, false)
// @ crvalue : not deallocatable value copy -  use a copy of the value and do not free it when destroying the collection
#define collection_crvalue(value, size) collection_value(value, size, true, false)

// @ lvalue  : deallocatable value reference - use the value reference and free it when destroying the collection
#define collection_lvalue(value, size)  collection_value(value, size, false, true)
// @ lvalue  : deallocatable value copy - use a copy of the value and free it when destroying the collection
#define collection_clvalue(value, size) collection_value(value, size, true, true)

typedef enum {
  COLLECTION_VALUE_COPY = 0x01, COLLECTION_VALUE_FREE = 0x02
} collection_value_traits_t;

#define collection_value_traits(value_traits) (value_traits & COLLECTION_VALUE_COPY), (value_traits & COLLECTION_VALUE_FREE)

typedef struct {
// @ value pointer
// @ treat an existing reference or a new allocated memory address
  void *ptr;

// @ value size
  size_t size;

// @ value copy
// @ the value is an allocated copy
  bool copy;

// @ free value
// @ the value needs to be freed
  bool free;
} collection_value_t;

// typedef void *(*collection_value_allocate_t)(void *);
typedef void (*collection_value_free_t)(void *);


extern void *collection_value_copy(const void *, size_t);
extern void collection_value_free(void *);


typedef void * collection_reverse_iterator_t;

typedef void * collection_iterator_t;


typedef collection_iterator_t (*collection_iterator_instance_t)(collection_t);
typedef collection_iterator_t (*collection_iterator_operator_t)(collection_iterator_t);
typedef void *(*collection_iterator_dereference_t)(collection_iterator_t);


typedef struct {
  collection_iterator_instance_t rbegin, rend;
  collection_iterator_operator_t rnext, rprev;
  collection_iterator_dereference_t key, value;
} collection_reverse_iterator_traits_t;

#define collection_reverse_iterator_traits(rbegin, rend, rnext, rprev) (collection_iterator_instance_t)rbegin, (collection_iterator_instance_t)rend, (collection_iterator_operator_t)rnext, (collection_iterator_operator_t)rprev


typedef struct {
  collection_iterator_instance_t begin, end;
  collection_iterator_operator_t next, prev;
  collection_iterator_dereference_t key, value;
} collection_iterator_traits_t;

#define collection_iterator_traits(begin, end, next, prev) (collection_iterator_instance_t)begin, (collection_iterator_instance_t)end, (collection_iterator_operator_t)next, (collection_iterator_operator_t)prev


struct collection_iterator_pair {
  void *first, *second;
};


typedef struct collection_cyclic_iterator_traits {
  collection_iterator_instance_t begin, end;
  collection_iterator_operator_t next, prev;
} collection_cyclic_iterator_traits_t;

#define collection_cyclic_iterator_traits(begin, end, next, prev) (collection_iterator_instance_t)begin, (collection_iterator_instance_t)end, (collection_iterator_operator_t)next, (collection_iterator_operator_t)prev

typedef struct collection_cyclic_iterator {
  collection_cyclic_iterator_traits_t traits;
  collection_iterator_t self;

  collection_t collection;
} *collection_cyclic_iterator_t;


extern collection_cyclic_iterator_t collection_cyclic_iterator_next(collection_cyclic_iterator_t);
extern collection_cyclic_iterator_t collection_cyclic_iterator_prev(collection_cyclic_iterator_t);

extern collection_cyclic_iterator_t collection_cyclic_iterator_begin(collection_cyclic_iterator_t);
extern collection_cyclic_iterator_t collection_cyclic_iterator_end(collection_cyclic_iterator_t);

extern collection_cyclic_iterator_t collection_cyclic_iterator(collection_t, collection_iterator_instance_t, collection_iterator_instance_t, collection_iterator_operator_t, collection_iterator_operator_t);
extern void collection_cyclic_iterator_free(collection_cyclic_iterator_t);


extern collection_t collection_filter(collection_t, collection_container_t, collection_predicate_t, collection_value_traits_t, ...);
//
// extern collection_t collection_map(collection_t, collection_container_t, collection_operation_t, collection_value_traits_t);
//
// extern void collection_transform(collection_t, collection_container_t, collection_operation_t);
//
// extern collection_t collection_reverse(collection_t, collection_container_t, collection_value_traits_t);
//
// extern collection_t collection_merge(collection_t, collection_t, collection_container_t, collection_value_traits_t);
//
extern size_t collection_count(collection_t collection, collection_container_t container, collection_predicate_t predicate, ...);
//
// extern void *collection_findif(collection_t, collection_predicate_t, ...);
//

#ifdef __cplusplus
}
#endif

#endif
