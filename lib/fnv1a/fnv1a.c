#include "fnv1a.h"


uint64_t
fnv1a_hash(const uint8_t *bytes, size_t length) {
  uint64_t hash = FNV1A_OFFSET;

  for (; length; bytes++, length--)
    hash ^= (uint64_t)*bytes, hash *= FNV1A_PRIME;

  return hash;
}
