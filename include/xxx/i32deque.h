#ifndef XXX_I32DEQUE_H
#define XXX_I32DEQUE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "xxx/config.h"
#include "xxx/allocator.h"
#include "xxx/assert.h"

#ifndef XXX_I32DEQUE_DEBUG
#  define XXX_I32DEQUE_DEBUG XXX_DEBUG
#endif

#ifndef XXX_I32DEQUE_ASSERT
#  define XXX_I32DEQUE_ASSERT XXX_ASSERT
#endif

#ifndef XXX_I32DEQUE_ALLOCATOR
#  define XXX_I32DEQUE_FREE    XXX_FREE
#  define XXX_I32DEQUE_MALLOC  XXX_MALLOC
#endif

#define XXX_I32DEQUE_CAPACITY_MAX (((size_t)1) << 30)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32deque xxx_i32deque_t;

static inline int xxx_i32deque_init(xxx_i32deque_t *self);
static inline void xxx_i32deque_deinit(xxx_i32deque_t *self);
static inline int xxx_i32deque_copy(xxx_i32deque_t *dst, const xxx_i32deque_t *src);
static inline void xxx_i32deque_move(xxx_i32deque_t *dst, xxx_i32deque_t *src);
static inline size_t xxx_i32deque_size(const xxx_i32deque_t *self);
static inline size_t xxx_i32deque_capacity(const xxx_i32deque_t *self);
static inline bool xxx_i32deque_empty(const xxx_i32deque_t *self);
static inline int *xxx_i32deque_at(xxx_i32deque_t *self, size_t pos);
static inline const int *xxx_i32deque_at_const(const xxx_i32deque_t *self, size_t pos);
static inline int *xxx_i32deque_front(xxx_i32deque_t *self);
static inline const int *xxx_i32deque_front_const(const xxx_i32deque_t *self);
static inline int *xxx_i32deque_back(xxx_i32deque_t *self);
static inline const int *xxx_i32deque_back_const(const xxx_i32deque_t *self);
static inline int xxx_i32deque_reserve(xxx_i32deque_t *self, size_t n);
static inline int xxx_i32deque_push_front(xxx_i32deque_t *self, int x);
static inline int xxx_i32deque_push_back(xxx_i32deque_t *self, int x);
static inline void xxx_i32deque_pop_front(xxx_i32deque_t *self);
static inline void xxx_i32deque_pop_back(xxx_i32deque_t *self);
static inline void xxx_i32deque_clear(xxx_i32deque_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32deque {
    int *buf;
    size_t cap;
    size_t head;
    size_t tail;
};

static inline
bool xxx_i32deque_is_pow2(size_t n) {
    return (n & (n - 1)) == 0;
}

static inline
size_t xxx_i32deque_ceil_pow2(size_t n) {
    if (xxx_i32deque_is_pow2(n)) {
        return n;
    }
    return (size_t)1 << (8 * sizeof(size_t) - __builtin_clzl(n));
}

/*
 * Undefined behavior if dst overlaps the contents of src.
 */
static inline
void xxx_i32deque_copy_linear(int *dst, const xxx_i32deque_t *src) {
    size_t size = src->tail - src->head;
    if (size == 0) {
        return;
    }
    size_t head = src->head & (src->cap - 1);
    size_t size1 = src->cap - head;
    if (size1 > size) {
        size1 = size;
    }
    memcpy(dst, src->buf + head, size1 * sizeof(int));
    size_t size2 = size - size1;
    if (size2 > 0) {
        memcpy(dst + size1, src->buf, size2 * sizeof(int));
    }
}

static inline
int xxx_i32deque_grow(xxx_i32deque_t *self, size_t new_cap) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(
        xxx_i32deque_is_pow2(new_cap),
        "new capacity %zu must be a power of 2", new_cap);

    XXX_I32DEQUE_ASSERT(
        new_cap > self->cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->cap);

    XXX_I32DEQUE_ASSERT(
        new_cap <= XXX_I32DEQUE_CAPACITY_MAX,
        "new capacity %zu exceeds maximum capacity %zu", new_cap, XXX_I32DEQUE_CAPACITY_MAX);
#endif

    int *new_buf = (int *)XXX_I32DEQUE_MALLOC(new_cap * sizeof(int));
    if (new_buf == NULL) {
        return -1;
    }
    xxx_i32deque_copy_linear(new_buf, self);
    XXX_I32DEQUE_FREE(self->buf);
    self->buf = new_buf;
    self->cap = new_cap;
    self->tail = self->tail - self->head;
    self->head = 0;
    return 0;
}

static inline
int xxx_i32deque_init(xxx_i32deque_t *self) {
    self->buf = NULL;
    self->cap = 0;
    self->head = 0;
    self->tail = 0;
    return 0;
}

static inline
void xxx_i32deque_deinit(xxx_i32deque_t *self) {
    XXX_I32DEQUE_FREE(self->buf);
    self->buf = NULL;
    self->cap = 0;
    self->head = 0;
    self->tail = 0;
}

static inline
int xxx_i32deque_copy(xxx_i32deque_t *dst, const xxx_i32deque_t *src) {
    if (dst == src) {
        return 0;
    }
    size_t size = src->tail - src->head;
    if (dst->cap < size) {
        size_t new_cap = xxx_i32deque_ceil_pow2(size);
        int *new_buf = (int *)XXX_I32DEQUE_MALLOC(new_cap * sizeof(int));
        if (new_buf == NULL) {
            return -1;
        }
        XXX_I32DEQUE_FREE(dst->buf);
        dst->buf = new_buf;
        dst->cap = new_cap;
    }
    xxx_i32deque_copy_linear(dst->buf, src);
    dst->head = 0;
    dst->tail = size;
    return 0;
}

static inline
void xxx_i32deque_move(xxx_i32deque_t *dst, xxx_i32deque_t *src) {
    if (dst == src) {
        return;
    }
    XXX_I32DEQUE_FREE(dst->buf);
    *dst = *src;
    src->buf = NULL;
    src->cap = 0;
    src->head = 0;
    src->tail = 0;
}

static inline
size_t xxx_i32deque_size(const xxx_i32deque_t *self) {
    return self->tail - self->head;
}

static inline
size_t xxx_i32deque_capacity(const xxx_i32deque_t *self) {
    return self->cap;
}

static inline
bool xxx_i32deque_empty(const xxx_i32deque_t *self) {
    return self->head == self->tail;
}

static inline
int *xxx_i32deque_at(xxx_i32deque_t *self, size_t pos) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(
        pos < self->tail - self->head,
        "index %zu out of range [0, %zu)", pos, self->tail - self->head);
#endif

    return &self->buf[(self->head + pos) & (self->cap - 1)];
}

static inline
const int *xxx_i32deque_at_const(const xxx_i32deque_t *self, size_t pos) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(
        pos < self->tail - self->head,
        "index %zu out of range [0, %zu)", pos, self->tail - self->head);
#endif

    return &self->buf[(self->head + pos) & (self->cap - 1)];
}

static inline
int *xxx_i32deque_front(xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    return &self->buf[self->head & (self->cap - 1)];
}

static inline
const int *xxx_i32deque_front_const(const xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    return &self->buf[self->head & (self->cap - 1)];
}

static inline
int *xxx_i32deque_back(xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    return &self->buf[(self->tail - 1) & (self->cap - 1)];
}

static inline
const int *xxx_i32deque_back_const(const xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    return &self->buf[(self->tail - 1) & (self->cap - 1)];
}

static inline
int xxx_i32deque_reserve(xxx_i32deque_t *self, size_t n) {
    if (n <= self->cap) {
        return 0;
    }
    if (n > XXX_I32DEQUE_CAPACITY_MAX) {
        return -1;
    }
    size_t new_cap = n > 16 ? xxx_i32deque_ceil_pow2(n) : 16;
    return xxx_i32deque_grow(self, new_cap);
}

static inline
int xxx_i32deque_push_front(xxx_i32deque_t *self, int x) {
    if (self->tail - self->head == self->cap) {
        if (self->cap == XXX_I32DEQUE_CAPACITY_MAX) {
            return -1;
        }
        size_t new_cap = self->cap > 8 ? self->cap * 2 : 16;
        if (new_cap > XXX_I32DEQUE_CAPACITY_MAX) {
            new_cap = XXX_I32DEQUE_CAPACITY_MAX;
        }
        if (xxx_i32deque_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    --self->head;
    self->buf[self->head & (self->cap - 1)] = x;
    return 0;
}

static inline
int xxx_i32deque_push_back(xxx_i32deque_t *self, int x) {
    if (self->tail - self->head == self->cap) {
        if (self->cap == XXX_I32DEQUE_CAPACITY_MAX) {
            return -1;
        }
        size_t new_cap = self->cap > 8 ? self->cap * 2 : 16;
        if (new_cap > XXX_I32DEQUE_CAPACITY_MAX) {
            new_cap = XXX_I32DEQUE_CAPACITY_MAX;
        }
        if (xxx_i32deque_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    self->buf[self->tail & (self->cap - 1)] = x;
    ++self->tail;
    return 0;
}

static inline void xxx_i32deque_pop_front(xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    ++self->head;
}

static inline
void xxx_i32deque_pop_back(xxx_i32deque_t *self) {
#if XXX_I32DEQUE_DEBUG
    XXX_I32DEQUE_ASSERT(self->tail - self->head > 0, "deque is empty");
#endif

    --self->tail;
}

static inline
void xxx_i32deque_clear(xxx_i32deque_t *self) {
    self->head = 0;
    self->tail = 0;
}

#endif
