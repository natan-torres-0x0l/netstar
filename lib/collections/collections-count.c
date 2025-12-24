#include "collections.h"


extern collection_iterator_traits_t collections_iterator_traits[];


size_t
collection_count(collection_t collection, collection_container_t container, collection_predicate_t predicate, ...) {
  collection_iterator_traits_t iterator_traits = collections_iterator_traits[container];
  collection_iterator_t iterator = NULL;

  va_list args;

  size_t count = 0;

  for (iterator = iterator_traits.begin(collection); iterator; iterator = iterator_traits.next(iterator)) {
    va_start(args, predicate);

    if (predicate(iterator_traits.value(iterator), args))
      count++;

    va_end(args);
  }

  return count;
}
