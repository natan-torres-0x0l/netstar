#include "resolv-cache.h"

#include <strings/strings.h>
#include <string.h>

#include <fnv1a/fnv1a.h>

#include <stdlib.h>
#include <stdio.h>


struct resolv_cache_block {
  struct resolv_cache_block *right, *left;

  struct resolv_cache_answer answer;
  uint64_t hash;
};


static inline uint64_t
resolv_cache_hash_new(struct resolv_search *search) {
  char string[DNS_HOSTNAME_LENGTH+14] = {0};
  snprintf(string, sizeof(string), "%s;%x;%x;%x", search->name, search->opcode, search->question.qtype, search->question.qclass);

  return fnv1a_hash((uint8_t *)string_tolower(string), string_length(string));
}

static struct resolv_cache_block *
resolv_cache_block_search(struct resolv_cache_block *blocks, uint64_t hash) {
  struct resolv_cache_block *block = NULL;

  for (block = blocks; block; block = block->right)
    if (block->hash == hash)
      return block;

  return NULL;
}

static struct resolv_cache_block * 
resolv_cache_block_push(struct resolv_cache_block *blocks, struct resolv_search *search, uint64_t hash, uint8_t *response, size_t length) {
  struct resolv_cache_block *block = NULL, *current = NULL;

  if (resolv_cache_block_search(blocks, hash))
    return blocks;

  if (!(block = (struct resolv_cache_block *)calloc(1, sizeof(struct resolv_cache_block))))
    return blocks;

  block->answer.response = (uint8_t *)calloc(length, sizeof(length));
  memcpy(block->answer.response, response, length);
  block->answer.length = length;

  block->answer.timestamp = time(NULL);

  if (blocks) {
    if (block->hash < blocks->hash) {
      block->right = blocks;
      blocks->left = block;

      return block;
    }
    
    for (current = block; current->right && block->hash > current->hash; current = current->right)
      continue;

    block->right = current->right;
    if (current->right)
      current->right->left = block;

    block->left = current;
    current->right = block;

    return blocks;
  }

  return block;
}

static void
resolv_cache_block_free(struct resolv_cache_block *blocks) {
  if (blocks) {
    struct resolv_cache_block *block = NULL, *next = NULL;

    for (block = blocks; block;) {
      next = block->right;

      free(block->answer.response), block->answer.response = NULL;
      memset(block, 0, sizeof( struct resolv_cache_block));

      free(block), block = next;
    }
  }
}


struct resolv_cache {
  struct resolv_cache_block *block;
};


static struct resolv_cache resolv_cache;


__attribute__((__destructor__))
void
resolv_cache_free(void) {
  resolv_cache_block_free(resolv_cache.block);
}

struct resolv_cache_answer *
resolv_cache_search(struct resolv_search *search) {
  struct resolv_cache_block *block = NULL;

  if (!(block = resolv_cache_block_search(resolv_cache.block, resolv_cache_hash_new(search))))
    return NULL;

  return &block->answer;
}

void
resolv_cache_push(struct resolv_search *search, uint8_t *response, size_t length) {
  uint64_t hash = resolv_cache_hash_new(search);
  
  if (resolv_cache_search(search))
    return;

  resolv_cache.block = resolv_cache_block_push(resolv_cache.block, search, hash, response, length);
}

bool
resolv_cache_empty(void) {
  return !resolv_cache.block;
}
