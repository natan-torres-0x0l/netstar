#include "fnv1.h"


uint64_t
fnv1_hash(const uint8_t *bytes, size_t length) {
  uint64_t hash = FNV1_OFFSET;

  for (; length; bytes++, length--)
    hash *= FNV1_PRIME, hash ^= (uint64_t)*bytes;

  return hash;
}
