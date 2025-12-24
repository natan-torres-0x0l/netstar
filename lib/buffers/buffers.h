#ifndef _BUFFERS_BUFFERS_H
#define _BUFFERS_BUFFERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>


typedef struct buffer buffer_t;


extern void buffer_append(buffer_t *, const void *, size_t);
extern void buffer_write(buffer_t *, const void *, size_t);

extern void buffer_splice(buffer_t *, size_t, size_t, const void *, size_t);

extern void buffer_truncate(buffer_t *, size_t);

extern int buffer_reserve(buffer_t *, size_t);
extern int buffer_resize(buffer_t *, size_t);

extern void buffer_zero(buffer_t *);

extern void *buffer_ptr(buffer_t *);

extern size_t buffer_capacity(buffer_t *);
extern size_t buffer_length(buffer_t *);
extern size_t buffer_available(buffer_t *);

extern buffer_t *buffer_make(size_t, size_t);
extern buffer_t *buffer_new(void);
extern void buffer_free(buffer_t *);

#ifdef __cplusplus
}
#endif

#endif
