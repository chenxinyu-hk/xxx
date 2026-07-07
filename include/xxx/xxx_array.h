#ifndef XXX_ARRAY_H
#define XXX_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "xxx_allocator.h"
#include "xxx_assert.h"

#ifndef XXX_ARRAY_DEBUG
#  ifdef DEBUG
#    define XXX_ARRAY_DEBUG 1
#  else
#    define XXX_ARRAY_DEBUG 0
#  endif
#endif

#ifndef XXX_ARRAY_ASSERT
#  define XXX_ARRAY_ASSERT XXX_ASSERT
#endif

#ifndef XXX_ARRAY_ALLOCATOR
#  define XXX_ARRAY_FREE    XXX_FREE
#  define XXX_ARRAY_REALLOC XXX_REALLOC
#endif

#define XXX_ARRAY_CAPACITY_MAX ((size_t)0x7fffffff)

// #define XXX_ARRAY_INITIALIZER {0, 0, 0}

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_array xxx_array_t;

static inline int xxx_array_init(xxx_array_t *self);
static inline void xxx_array_deinit(xxx_array_t *self);
static inline int xxx_array_copy(xxx_array_t *dst, const xxx_array_t *src);
static inline void xxx_array_move(xxx_array_t *dst, xxx_array_t *src);
static inline size_t xxx_array_length(const xxx_array_t *self);
static inline size_t xxx_array_capacity(const xxx_array_t *self);
static inline bool xxx_array_empty(const xxx_array_t *self);
static inline void **xxx_array_data(xxx_array_t *self);
static inline void **xxx_array_at(xxx_array_t *self, size_t pos);
static inline void **xxx_array_front(xxx_array_t *self);
static inline void **xxx_array_back(xxx_array_t *self);
static inline int xxx_array_reserve(xxx_array_t *self, size_t n);
static inline int xxx_array_push_back(xxx_array_t *self, void *x);
static inline void xxx_array_pop_back(xxx_array_t *self);
static inline void xxx_array_clear(xxx_array_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_array {
    void **buf;
    size_t len;
    size_t cap;
};

static inline
int xxx_array_grow(xxx_array_t *self, size_t new_cap) {
#if XXX_ARRAY_DEBUG
    XXX_ARRAY_ASSERT(
        new_cap > self->cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->cap);
    XXX_ARRAY_ASSERT(
        new_cap <= XXX_ARRAY_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_ARRAY_CAPACITY_MAX);
#endif
    void **new_buf = (void **)XXX_ARRAY_REALLOC(self->buf, new_cap * sizeof(void *));
    if (new_buf == NULL) {
        return -1;
    }
    self->buf = new_buf;
    self->cap = new_cap;
    return 0;
}

static inline
int xxx_array_init(xxx_array_t *self) {
    self->buf = NULL;
    self->len = 0;
    self->cap = 0;
    return 0;
}

static inline
void xxx_array_deinit(xxx_array_t *self) {
    XXX_ARRAY_FREE(self->buf);
    self->buf = NULL;
}

static inline
int xxx_array_copy(xxx_array_t *dst, const xxx_array_t *src) {
    if (dst == src) {
        return 0;
    }
    if (xxx_array_reserve(dst, src->len) != 0) {
        return -1;
    }
    memcpy(dst->buf, src->buf, src->len * sizeof(void *));
    dst->len = src->len;
    return 0;
}

static inline
void xxx_array_move(xxx_array_t *dst, xxx_array_t *src) {
    XXX_ARRAY_FREE(dst->buf);
    *dst = *src;
    src->buf = NULL;
}

static inline
size_t xxx_array_length(const xxx_array_t *self) {
    return self->len;
}

static inline
size_t xxx_array_capacity(const xxx_array_t *self) {
    return self->cap;
}

static inline
bool xxx_array_empty(const xxx_array_t *self) {
    return self->len == 0;
}

static inline
void **xxx_array_data(xxx_array_t *self) {
    return self->buf;
}

static inline
void **xxx_array_at(xxx_array_t *self, size_t pos) {
#if XXX_ARRAY_DEBUG
    XXX_ARRAY_ASSERT(
        i < self->len,
        "index %zu out of range [0, %zu)", i, self->len);
#endif
    return &self->buf[pos];
}

static inline
void **xxx_array_front(xxx_array_t *self) {
#if XXX_ARRAY_DEBUG
    XXX_ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[0];
}

static inline
void **xxx_array_back(xxx_array_t *self) {
#if XXX_ARRAY_DEBUG
    XXX_ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[self->len - 1];
}

static inline
int xxx_array_reserve(xxx_array_t *self, size_t n) {
    if (n <= self->cap) {
        return 0;
    }
    if (n > XXX_ARRAY_CAPACITY_MAX) {
        return -1;
    }
    return xxx_array_grow(self, n);
}

static inline
int xxx_array_push_back(xxx_array_t *self, void *x) {
    if (self->len == self->cap) {
        if (self->cap == XXX_ARRAY_CAPACITY_MAX) {
            return -1;
        }
        size_t new_cap = self->cap > 8 ? self->cap << 1 : 16;
        if (new_cap > XXX_ARRAY_CAPACITY_MAX) {
            new_cap = XXX_ARRAY_CAPACITY_MAX;
        }
        if (xxx_array_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    self->buf[self->len++] = x;
    return 0;
}

static inline
void xxx_array_pop_back(xxx_array_t *self) {
#if XXX_ARRAY_DEBUG
    XXX_ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    --self->len;
}

static inline
void xxx_array_clear(xxx_array_t *self) {
    self->len = 0;
}

#endif
