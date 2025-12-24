#include "buffers.h"

#include <stdlib.h>
#include <string.h>

#include <stdint.h>

struct buffer {
  size_t shift, length, capacity;
  void *buffer;
};


static inline bool
buffer_requires_reallocation(buffer_t *buffer, size_t length) {
  return length > buffer->capacity;
}

static inline size_t
buffer_capacity_growth(size_t capacity) {
  return capacity*2;
}

static inline int
buffer_allocate(buffer_t *buffer, size_t length, size_t capacity) {
  buffer->capacity = ((length > capacity) ? buffer_capacity_growth(length) : capacity);
  buffer->length = length;

  if ((buffer->buffer = calloc(capacity, sizeof(uint8_t))))
    return 1;

  buffer_free(buffer);
  return -1;
}

static inline int
buffer_reallocate(buffer_t *buffer, size_t capacity) {
  void *bufferptr = buffer->buffer;

  if (!(bufferptr = realloc(buffer->buffer, capacity*sizeof(uint8_t))))
    return -1;

  if (buffer->capacity < capacity)
    memset((uint8_t *)bufferptr+buffer->capacity, 0, capacity-buffer->capacity);

  buffer->buffer = bufferptr;
  buffer->capacity = capacity;

  return 1;
}

static inline int
buffer_reallocate_capacity(buffer_t *buffer, size_t length) {
  return buffer_reallocate(buffer, buffer_capacity_growth(buffer->length+length));
}

void
buffer_append(buffer_t *buffer, const void *bytes, size_t length) {
  if (buffer_requires_reallocation(buffer, buffer->length+length) && buffer_reallocate_capacity(buffer, length) == -1)
    return;

  memcpy((uint8_t *)buffer->buffer+buffer->length, bytes, length);
  buffer->length += length;
}

void
buffer_write(buffer_t *buffer, const void *bytes, size_t length) {
  if (buffer_requires_reallocation(buffer, buffer->shift+length) && buffer_reallocate_capacity(buffer, length) == -1)
    return;

  memcpy((uint8_t *)buffer->buffer+buffer->shift, bytes, length);
  buffer->shift += length;

  buffer->length = ((buffer->length < buffer->shift) ? buffer->shift : buffer->length);
}

static inline bool
buffer_length_overflow(buffer_t *buffer, size_t length) {
  return length > buffer->length;
}

void
buffer_splice(buffer_t *buffer, size_t begin, size_t end, const void *bytes, size_t length) {
  uint8_t *slice_begin = NULL, *slice_end = NULL;
  size_t slice_length, slice_copy;

  if (begin > end || buffer_length_overflow(buffer, (slice_length = end-begin)))
    return;

  slice_copy = buffer->length-slice_length+length;

  if (buffer_requires_reallocation(buffer, slice_copy) && buffer_reallocate_capacity(buffer, slice_copy) == -1)
    return;

  slice_begin = (uint8_t *)buffer->buffer+begin;
  slice_end = (uint8_t *)buffer->buffer+end;

  memmove(slice_begin+length, slice_end, (size_t)(((uint8_t *)buffer->buffer+buffer->length)-slice_end));
  memcpy(slice_begin, bytes, length);

  buffer->length = buffer->length+(length-slice_length);
}

void
buffer_truncate(buffer_t *buffer, size_t length) {
  if (buffer_length_overflow(buffer, length))
    return;

  memset((uint8_t *)buffer->buffer+length, 0, buffer->length-length);
  buffer->length = length;
  buffer->shift = length;
}

void
buffer_zero(buffer_t *buffer) {
  memset(buffer->buffer, 0, buffer->capacity);
  buffer->shift = 0;
}

int
buffer_reserve(buffer_t *buffer, size_t length) {
  if (length > buffer->capacity && buffer_reallocate(buffer, length) < 0)
    return -1;
  if (length < buffer->capacity)
    return 1;

  memset((uint8_t *)buffer->buffer+buffer->length, 0, buffer->capacity-buffer->length);
  return 1;
}

int
buffer_resize(buffer_t *buffer, size_t length) {
  if (length > buffer->capacity)
    return -1;

  buffer->length = length;
  return 1;
}

size_t
buffer_length(buffer_t *buffer) {
  return buffer->length;
}

size_t
buffer_capacity(buffer_t *buffer) {
  return buffer->capacity;
}

size_t
buffer_available(buffer_t *buffer) {
  return buffer->capacity-buffer->length;
}

void *
buffer_ptr(buffer_t *buffer) {
  return buffer->buffer;
}

buffer_t *
buffer_make(size_t length, size_t capacity) {
  buffer_t *buffer = NULL;

  if (!(buffer = buffer_new()) || buffer_allocate(buffer, length, capacity) < 0)
    return NULL;

  return buffer;
}

buffer_t *
buffer_new(void) {
  buffer_t *buffer = NULL;

  if (!(buffer = (buffer_t *)calloc(1, sizeof(buffer_t))))
    return NULL;

  return buffer;
}

void
buffer_free(buffer_t *buffer) {
  if (buffer) {
    free(buffer->buffer), buffer->buffer = NULL;
    free(buffer);
  }
}
