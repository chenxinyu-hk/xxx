#ifndef XXX_I32STACK_H
#define XXX_I32STACK_H

#include <stdbool.h>

#include "xxx_assert.h"
#include "xxx_i32array.h"

#ifndef XXX_I32STACK_DEBUG
#  ifdef DEBUG
#    define XXX_I32STACK_DEBUG 1
#  else
#    define XXX_I32STACK_DEBUG 0
#  endif
#endif

#ifndef XXX_I32STACK_ASSERT
#  define XXX_I32STACK_ASSERT XXX_ASSERT
#endif

// #define XXX_I32STACK_INITIALIZER { XXX_I32ARRAY_INITIALIZER }

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32stack xxx_i32stack_t;

static inline int xxx_i32stack_init(xxx_i32stack_t *self);
static inline void xxx_i32stack_deinit(xxx_i32stack_t *self);
static inline int xxx_i32stack_copy(xxx_i32stack_t *dst, const xxx_i32stack_t *src);
static inline void xxx_i32stack_move(xxx_i32stack_t *dst, xxx_i32stack_t *src);
static inline size_t xxx_i32stack_size(const xxx_i32stack_t *self);
static inline bool xxx_i32stack_empty(const xxx_i32stack_t *self);
static inline int *xxx_i32stack_top(xxx_i32stack_t *self);
static inline int xxx_i32stack_push(xxx_i32stack_t *self, int x);
static inline void xxx_i32stack_pop(xxx_i32stack_t *self);
static inline void xxx_i32stack_clear(xxx_i32stack_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32stack {
    xxx_i32array_t arr;
};

static inline
int xxx_i32stack_init(xxx_i32stack_t *self) {
    return xxx_i32array_init(&self->arr);
}

static inline
void xxx_i32stack_deinit(xxx_i32stack_t *self) {
    xxx_i32array_deinit(&self->arr);
}

static inline
int xxx_i32stack_copy(xxx_i32stack_t *dst, const xxx_i32stack_t *src) {
    return xxx_i32array_copy(&dst->arr, &src->arr);
}

static inline
void xxx_i32stack_move(xxx_i32stack_t *dst, xxx_i32stack_t *src) {
    xxx_i32array_move(&dst->arr, &src->arr);
}

static inline
size_t xxx_i32stack_size(const xxx_i32stack_t *self) {
    return xxx_i32array_length(&self->arr);
}

static inline
bool xxx_i32stack_empty(const xxx_i32stack_t *self) {
    return xxx_i32array_empty(&self->arr);
}

static inline
int *xxx_i32stack_top(xxx_i32stack_t *self) {
#if XXX_I32STACK_DEBUG
    XXX_I32STACK_ASSERT(!xxx_i32stack_empty(self), "stack is empty");
#endif

    return xxx_i32array_back(&self->arr);
}

static inline
int xxx_i32stack_push(xxx_i32stack_t *self, int x) {
    return xxx_i32array_push_back(&self->arr, x);
}

static inline
void xxx_i32stack_pop(xxx_i32stack_t *self) {
#if XXX_I32STACK_DEBUG
    XXX_I32STACK_ASSERT(!xxx_i32stack_empty(self), "stack underflow");
#endif

    xxx_i32array_pop_back(&self->arr);
}

static inline
void xxx_i32stack_clear(xxx_i32stack_t *self) {
    xxx_i32array_clear(&self->arr);
}

#endif
