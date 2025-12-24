#include "collections.h"

#include <collections/hashset/hashset.h>
#include <collections/set/set.h>

#include <collections/rbtree/rbtree.h>

#include <collections/map/map.h>

#include <collections/stack/stack.h>
#include <collections/queue/queue.h>

#include <collections/deque/deque.h>
#include <collections/list/list.h>


extern int collection_insert(collection_t, collection_container_t, collection_iterator_t, collection_value_traits_t);
extern collection_t collection_new(collection_t, collection_container_t);

extern collection_iterator_traits_t collections_iterator_traits[];


collection_t
collection_filter(collection_t collection, collection_container_t collection_container, collection_predicate_t predicate, collection_value_traits_t value_traits, ...) {
  collection_iterator_traits_t iterator_traits = collections_iterator_traits[collection_container];
  collection_iterator_t iterator = NULL;

  collection_t collection_filtered = NULL;
  size_t index;

  va_list args;

  va_start(args, value_traits);

  if (!(collection_filtered = collection_new(collection, collection_container)))
    return NULL;

/*
  for (iterator = iterator_traits.begin(collection), index = 0; iterator; iterator = iterator_traits.next(iterator), index++) {
    struct collection_iterator_pair pair = {
      .first  = ((iterator_traits.key) ? iterator_traits.key(iterator) : &index),
      .second = iterator_traits.value(iterator)
    };

    if (predicate(&pair, args))
      collection_insert(
        collection_filtered,
        collection_container,
        iterator,
        value_traits
      );
  }
*/
  return collection_filtered;
}
