#include "collections.h"

#ifdef COLLECTIONS_EXTENDED
#include <collections/unordered-set/unordered-set.h>
#include <collections/multiset/multiset.h>
#include <collections/hashset/hashset.h>
#include <collections/set/set.h>

#include <collections/hashtable/hashtable.h>

#include <collections/unordered-map/unordered-map.h>
#include <collections/multimap/multimap.h>
#include <collections/hashmap/hashmap.h>
#include <collections/map/map.h>

#include <collections/priority-queue/priority-queue.h>
#include <collections/queue/queue.h>

#include <collections/deque/deque.h>
#include <collections/list/list.h>

#include <collections/binary-heap/binary-heap.h>
#include <collections/stack/stack.h>

#include <collections/avltree/avltree.h>
#include <collections/bstree/bstree.h>
#include <collections/rbtree/rbtree.h>

#endif

#include <stdlib.h>
#include <string.h>


#ifdef COLLECTIONS_EXTENDED
#define collection_iterator_traits(rbegin, rend, begin, end, rnext, rprev, next, prev, key, value) \
  (collection_iterator_instance_t)rbegin, (collection_iterator_instance_t)rend,                    \
  (collection_iterator_instance_t)begin , (collection_iterator_instance_t)end,                     \
  (collection_iterator_operator_t)rnext , (collection_iterator_operator_t)rprev,                   \
  (collection_iterator_operator_t)next  , (collection_iterator_operator_t)prev,                    \
  (collection_iterator_dereference_t)key, (collection_iterator_dereference_t)value

collection_iterator_traits_t collections_iterator_traits[] = {
  { collection_iterator_traits(avltree_rbegin       , avltree_rend       , avltree_begin       , avltree_end       , avltree_rnext       , avltree_rprev       , avltree_next       , avltree_prev       , NULL             , avltree_value)        },
  { collection_iterator_traits(binary_heap_rbegin   , binary_heap_rend   , binary_heap_begin   , binary_heap_end   , binary_heap_rnext   , binary_heap_rprev   , binary_heap_next   , binary_heap_prev   , NULL             , binary_heap_value)    },
  { collection_iterator_traits(bstree_rbegin        , bstree_rend        , bstree_begin        , bstree_end        , bstree_rnext        , bstree_rprev        , bstree_next        , bstree_prev        , NULL             , bstree_value)         },
  { collection_iterator_traits(deque_rbegin         , deque_rend         , deque_begin         , deque_end         , deque_rnext         , deque_rprev         , deque_next         , deque_prev         , NULL             , deque_value)          },
  { collection_iterator_traits(hashmap_rbegin       , hashmap_rend       , hashmap_begin       , hashmap_end       , hashmap_rnext       , hashmap_rprev       , hashmap_next       , hashmap_prev       , hashmap_key      , hashmap_value)        },
  { collection_iterator_traits(hashset_rbegin       , hashset_rend       , hashset_begin       , hashset_end       , hashset_rnext       , hashset_rprev       , hashset_next       , hashset_prev       , NULL             , hashset_value)        },
  { collection_iterator_traits(hashtable_rbegin     , hashtable_rend     , hashtable_begin     , hashtable_end     , hashtable_rnext     , hashtable_rprev     , hashtable_next     , hashtable_prev     , NULL             , hashtable_value)      },
  { collection_iterator_traits(list_rbegin          , list_rend          , list_begin          , list_end          , list_rnext          , list_rprev          , list_next          , list_prev          , NULL             , list_value)           },
  { collection_iterator_traits(map_rbegin           , map_rend           , map_begin           , map_end           , map_rnext           , map_rprev           , map_next           , map_prev           , map_key          , map_value)            },
  { collection_iterator_traits(multimap_rbegin      , multimap_rend      , multimap_begin      , multimap_end      , multimap_rnext      , multimap_rprev      , multimap_next      , multimap_prev      , multimap_key     , multimap_value)       },
  { collection_iterator_traits(multiset_rbegin      , multiset_rend      , multiset_begin      , multiset_end      , multiset_rnext      , multiset_rprev      , multiset_next      , multiset_prev      , NULL             , multiset_value)       },
  { collection_iterator_traits(priority_queue_rbegin, priority_queue_rend, priority_queue_begin, priority_queue_end, priority_queue_rnext, priority_queue_rprev, priority_queue_next, priority_queue_prev, NULL             , priority_queue_value) },
  { collection_iterator_traits(queue_rbegin         , queue_rend         , queue_begin         , queue_end         , queue_rnext         , queue_rprev         , queue_next         , queue_prev         , NULL             , queue_value)          },
  { collection_iterator_traits(rbtree_rbegin        , rbtree_rend        , rbtree_begin        , rbtree_end        , rbtree_rnext        , rbtree_rprev        , rbtree_next        , rbtree_prev        , NULL             , rbtree_value)         },
  { collection_iterator_traits(set_rbegin           , set_rend           , set_begin           , set_end           , set_rnext           , set_rprev           , set_next           , set_prev           , NULL             , set_value)            },
  { collection_iterator_traits(stack_rbegin         , stack_rend         , stack_begin         , stack_end         , stack_rnext         , stack_rprev         , stack_next         , stack_prev         , NULL             , stack_value)          },
  { collection_iterator_traits(unordered_map_rbegin , unordered_map_rend , unordered_map_begin , unordered_map_end , unordered_map_rnext , unordered_map_rprev , unordered_map_next , unordered_map_prev , unordered_map_key, unordered_map_value)  },
  { collection_iterator_traits(unordered_set_rbegin , unordered_set_rend , unordered_set_begin , unordered_set_end , unordered_set_rnext , unordered_set_rprev , unordered_set_next , unordered_set_prev , NULL             , unordered_set_value)  },
};

#endif

/*
int
collection_insert(collection_t collection, collection_container_t collection_container, collection_iterator_t iterator, collection_value_traits_t value_traits) {
  switch (collection_container) {
    case COLLECTION_DEQUE: {
      void *value = deque_value(iterator);
      size_t value_size = deque_value_size(iterator);

      return deque_pushfront((deque_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_HASHSET: {
      void *value = hashset_value(iterator);
      size_t value_size = hashset_value_size(iterator);

      return hashset_insert((hashset_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_LIST: {
      void *value = list_value(iterator);
      size_t value_size = list_value_size(iterator);

      return list_pushfront((list_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_MAP: {
      void *key = map_key(iterator);
      size_t key_size = map_key_size(iterator);

      void *value = map_value(iterator);
      size_t value_size = map_value_size(iterator);

      return map_insert((map_t *)collection, key, key_size, collection_value_traits(value_traits), value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_QUEUE: {
      void *value = queue_value(iterator);
      size_t value_size = queue_value_size(iterator);

      return queue_push((queue_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_RBTREE: {
      void *value = rbtree_value(iterator);
      size_t value_size = rbtree_value_size(iterator);

      return rbtree_insert((rbtree_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_SET: {
      void *value = set_value(iterator);
      size_t value_size = set_value_size(iterator);

      return set_insert((set_t *)collection, value, value_size, collection_value_traits(value_traits));
    }

    case COLLECTION_STACK: {
      void *value = stack_value(iterator);
      size_t value_size = stack_value_size(iterator);

      return stack_push((stack_t *)collection, value, value_size, collection_value_traits(value_traits));
    }
  }

  return -1;
}

int
collection_remove(collection_t collection, collection_iterator_t iterator) {
  return -1;
}

collection_t
collection_new(collection_t collection, collection_container_t collection_container) {
  switch (collection_container) {
    case COLLECTION_DEQUE:
      return deque_new(deque_value_free((deque_t *)collection));

    case COLLECTION_HASHSET:
      return hashset_new(hashset_value_free((hashset_t *)collection));

    case COLLECTION_LIST:
      return list_new(list_value_free((list_t *)collection));

    case COLLECTION_MAP:
      return map_new(map_key_free((map_t *)collection), map_value_free((map_t *)collection));

    case COLLECTION_QUEUE:
      return queue_new(queue_value_free((queue_t *)collection));

    case COLLECTION_RBTREE:
      return rbtree_new(rbtree_value_compare((rbtree_t *)collection), rbtree_value_free((rbtree_t *)collection));

    case COLLECTION_SET:
      return set_new(set_value_compare((set_t *)collection), set_value_free((set_t *)collection));

    case COLLECTION_STACK:
      return stack_new(stack_value_free((stack_t *)collection));
  }

  return NULL;
}
*/


void *
collection_value_copy(const void *ptr, size_t size) {
  void *memory = NULL;

  if (!(memory = calloc(1, size)))
    return NULL;

  return memcpy(memory, ptr, size);
}

void
collection_value_free(void *ptr) {
  free(ptr);
}
