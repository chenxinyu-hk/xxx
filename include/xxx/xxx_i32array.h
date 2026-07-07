#ifndef XXX_I32ARRAY_H
#define XXX_I32ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "xxx_allocator.h"
#include "xxx_assert.h"

#ifndef XXX_I32ARRAY_DEBUG
#  ifdef DEBUG
#    define XXX_I32ARRAY_DEBUG 1
#  else
#    define XXX_I32ARRAY_DEBUG 0
#  endif
#endif

#ifndef XXX_I32ARRAY_ASSERT
#  define XXX_I32ARRAY_ASSERT XXX_ASSERT
#endif

#ifndef XXX_I32ARRAY_ALLOCATOR
#  define XXX_I32ARRAY_FREE    XXX_FREE
#  define XXX_I32ARRAY_MALLOC  XXX_MALLOC
#  define XXX_I32ARRAY_REALLOC XXX_REALLOC
#endif

#define XXX_I32ARRAY_CAPACITY_MAX ((size_t)0x7fffffff)

// #define XXX_I32ARRAY_INITIALIZER {0, 0, 0}

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32array xxx_i32array_t;

static inline int xxx_i32array_init(xxx_i32array_t *self);
static inline void xxx_i32array_deinit(xxx_i32array_t *self);
static inline int xxx_i32array_copy(xxx_i32array_t *dst, const xxx_i32array_t *src);
static inline void xxx_i32array_move(xxx_i32array_t *dst, xxx_i32array_t *src);
static inline size_t xxx_i32array_length(const xxx_i32array_t *self);
static inline size_t xxx_i32array_capacity(const xxx_i32array_t *self);
static inline bool xxx_i32array_empty(const xxx_i32array_t *self);
static inline int *xxx_i32array_data(xxx_i32array_t *self);
static inline const int *xxx_i32array_data_const(const xxx_i32array_t *self);
static inline int *xxx_i32array_at(xxx_i32array_t *self, size_t pos);
static inline const int *xxx_i32array_at_const(const xxx_i32array_t *self, size_t pos);
static inline int *xxx_i32array_front(xxx_i32array_t *self);
static inline const int *xxx_i32array_front_const(const xxx_i32array_t *self);
static inline int *xxx_i32array_back(xxx_i32array_t *self);
static inline const int *xxx_i32array_back_const(const xxx_i32array_t *self);
static inline int xxx_i32array_reserve(xxx_i32array_t *self, size_t n);
static inline int xxx_i32array_assign(xxx_i32array_t *self, const int *arr, size_t len);
static inline int xxx_i32array_push_back(xxx_i32array_t *self, int x);
static inline void xxx_i32array_pop_back(xxx_i32array_t *self);
static inline void xxx_i32array_clear(xxx_i32array_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32array {
    int *buf;
    size_t len;
    size_t cap;
};

static inline
int xxx_i32array_grow(xxx_i32array_t *self, size_t new_cap) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(
        new_cap > self->cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->cap);
    XXX_I32ARRAY_ASSERT(
        new_cap <= XXX_I32ARRAY_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_I32ARRAY_CAPACITY_MAX);
#endif
    int *new_buf = (int *)XXX_I32ARRAY_REALLOC(self->buf, new_cap * sizeof(int));
    if (new_buf == NULL) {
        return -1;
    }
    self->buf = new_buf;
    self->cap = new_cap;
    return 0;
}

static inline
int xxx_i32array_init(xxx_i32array_t *self) {
    self->buf = NULL;
    self->len = 0;
    self->cap = 0;
    return 0;
}

static inline
void xxx_i32array_deinit(xxx_i32array_t *self) {
    XXX_I32ARRAY_FREE(self->buf);
    self->buf = NULL;
}

static inline
int xxx_i32array_copy(xxx_i32array_t *dst, const xxx_i32array_t *src) {
    if (dst == src) {
        return 0;
    }
    return xxx_i32array_assign(dst, src->buf, src->len);
}

static inline
void xxx_i32array_move(xxx_i32array_t *dst, xxx_i32array_t *src) {
    XXX_I32ARRAY_FREE(dst->buf);
    *dst = *src;
    src->buf = NULL;
}

static inline
size_t xxx_i32array_length(const xxx_i32array_t *self) {
    return self->len;
}

static inline
size_t xxx_i32array_capacity(const xxx_i32array_t *self) {
    return self->cap;
}

static inline
bool xxx_i32array_empty(const xxx_i32array_t *self) {
    return self->len == 0;
}

static inline
int *xxx_i32array_data(xxx_i32array_t *self) {
    return self->buf;
}

static inline
const int *xxx_i32array_data_const(const xxx_i32array_t *self) {
    return self->buf;
}

static inline
int *xxx_i32array_at(xxx_i32array_t *self, size_t pos) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[pos];
}

static inline
const int *xxx_i32array_at_const(const xxx_i32array_t *self, size_t pos) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[pos];
}

static inline
int *xxx_i32array_front(xxx_i32array_t *self) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[0];
}

static inline
const int *xxx_i32array_front_const(const xxx_i32array_t *self) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[0];
}

static inline
int *xxx_i32array_back(xxx_i32array_t *self) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[self->len - 1];
}

static inline
const int *xxx_i32array_back_const(const xxx_i32array_t *self) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    return &self->buf[self->len - 1];   
}

static inline
int xxx_i32array_reserve(xxx_i32array_t *self, size_t n) {
    if (n <= self->cap) {
        return 0;
    }
    if (n > XXX_I32ARRAY_CAPACITY_MAX) {
        return -1;
    }
    return xxx_i32array_grow(self, n);
}

static inline
int xxx_i32array_assign(xxx_i32array_t *self, const int *arr, size_t len) {
    if (len > XXX_I32ARRAY_CAPACITY_MAX) {
        return -1;
    }
    if (len <= self->cap) {
        memcpy(self->buf, arr, len * sizeof(int));
        self->len = len;
        return 0;
    }
    int *new_buf = (int *)XXX_I32ARRAY_MALLOC(len * sizeof(int));
    if (new_buf == NULL) {
        return -1;
    }
    memcpy(new_buf, arr, len * sizeof(int));
    XXX_I32ARRAY_FREE(self->buf);
    self->buf = new_buf;
    self->len = len;
    self->cap = len;
    return 0;
}

static inline
int xxx_i32array_push_back(xxx_i32array_t *self, int x) {
    if (self->len == self->cap) {
        if (self->cap == XXX_I32ARRAY_CAPACITY_MAX) {
            return -1;
        }
        size_t new_cap = self->cap > 8 ? self->cap << 1 : 16;
        if (self->cap > XXX_I32ARRAY_CAPACITY_MAX) {
            new_cap = XXX_I32ARRAY_CAPACITY_MAX;
        }
        if (xxx_i32array_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    self->buf[self->len++] = x;
    return 0;
}

static inline
void xxx_i32array_pop_back(xxx_i32array_t *self) {
#if XXX_I32ARRAY_DEBUG
    XXX_I32ARRAY_ASSERT(self->len > 0, "array is empty");
#endif
    --self->len;
}

static inline
void xxx_i32array_clear(xxx_i32array_t *self) {
    self->len = 0;
}

#endif
