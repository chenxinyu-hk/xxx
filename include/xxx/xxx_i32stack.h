#ifndef XXX_I32STACK_H
#define XXX_I32STACK_H

#include "xxx_i32array.h"

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
static inline size_t xxx_i32stack_capacity(const xxx_i32stack_t *self);
static inline bool xxx_i32stack_empty(const xxx_i32stack_t *self);
static inline int xxx_i32stack_reserve(xxx_i32stack_t *self, size_t n);
static inline int *xxx_i32stack_top(xxx_i32stack_t *self);
static inline int xxx_i32stack_push(xxx_i32stack_t *self, int x);
static inline void xxx_i32stack_pop(xxx_i32stack_t *self);
static inline void xxx_i32stack_clear(xxx_i32stack_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32stack {
    xxx_i32array_t c;
};

static inline
int xxx_i32stack_init(xxx_i32stack_t *self) {
    return xxx_i32array_init(&self->c);
}

static inline
void xxx_i32stack_deinit(xxx_i32stack_t *self) {
    xxx_i32array_deinit(&self->c);
}

static inline
int xxx_i32stack_copy(xxx_i32stack_t *dst, const xxx_i32stack_t *src) {
    return xxx_i32array_copy(&dst->c, &src->c);
}

static inline
void xxx_i32stack_move(xxx_i32stack_t *dst, xxx_i32stack_t *src) {
    xxx_i32array_move(&dst->c, &src->c);
}

static inline
size_t xxx_i32stack_size(const xxx_i32stack_t *self) {
    return xxx_i32array_length(&self->c);
}

static inline
size_t xxx_i32stack_capacity(const xxx_i32stack_t *self) {
    return xxx_i32array_capacity(&self->c);
}

static inline
bool xxx_i32stack_empty(const xxx_i32stack_t *self) {
    return xxx_i32array_empty(&self->c);
}

static inline
int xxx_i32stack_reserve(xxx_i32stack_t *self, size_t n) {
    return xxx_i32array_reserve(&self->c, n);
}

static inline
int *xxx_i32stack_top(xxx_i32stack_t *self) {
    return xxx_i32array_back(&self->c);
}

static inline
int xxx_i32stack_push(xxx_i32stack_t *self, int x) {
    return xxx_i32array_push_back(&self->c, x);
}

static inline
void xxx_i32stack_pop(xxx_i32stack_t *self) {
    xxx_i32array_pop_back(&self->c);
}

static inline
void xxx_i32stack_clear(xxx_i32stack_t *self) {
    xxx_i32array_clear(&self->c);
}

#endif
