#include "collections.h"

#include <stdlib.h>


collection_cyclic_iterator_t
collection_cyclic_iterator(collection_t collection, collection_iterator_instance_t begin, collection_iterator_instance_t end, collection_iterator_operator_t next, collection_iterator_operator_t prev) {
  collection_cyclic_iterator_t iterator = NULL;

  if (!(iterator = (collection_cyclic_iterator_t)calloc(1, sizeof(struct collection_cyclic_iterator))))
    return NULL;

  iterator->collection = collection;

  iterator->traits.begin = begin;
  iterator->traits.end   = end;
  iterator->traits.next  = next;
  iterator->traits.prev  = prev;

  iterator->self = iterator->traits.begin(collection);

  return iterator;
}

collection_cyclic_iterator_t
collection_cyclic_iterator_next(collection_cyclic_iterator_t iterator) {
  if ((iterator->self == iterator->traits.end(iterator->collection) || !iterator->traits.next(iterator->self)))
    iterator->self = iterator->traits.begin(iterator->collection);
  else
    iterator->self = iterator->traits.next(iterator->self);

  return iterator;
}

collection_cyclic_iterator_t
collection_cyclic_iterator_prev(collection_cyclic_iterator_t iterator) {
  if (iterator->self == iterator->traits.begin(iterator->collection))
    iterator->self = iterator->traits.end(iterator->collection);
  else
    iterator->self = iterator->traits.prev(iterator->self);

  return iterator;
}

collection_cyclic_iterator_t
collection_cyclic_iterator_begin(collection_cyclic_iterator_t iterator) {
  iterator->self = iterator->traits.begin(iterator->collection);
  return iterator;
}

collection_cyclic_iterator_t
collection_cyclic_iterator_end(collection_cyclic_iterator_t iterator) {
  iterator->self = iterator->traits.end(iterator->collection);
  return iterator;
}

void
collection_cyclic_iterator_free(collection_cyclic_iterator_t iterator) {
  if (iterator) {
    free(iterator);
  }
}
