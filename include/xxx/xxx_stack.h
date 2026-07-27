#ifndef XXX_STACK_H
#define XXX_STACK_H

#include <stdbool.h>

#include "xxx_config.h"
#include "xxx_array.h"
#include "xxx_assert.h"

#ifndef XXX_STACK_DEBUG
#  define XXX_STACK_DEBUG XXX_DEBUG
#endif

#ifndef XXX_STACK_ASSERT
#  define XXX_STACK_ASSERT XXX_ASSERT
#endif

// #define XXX_STACK_INITIALIZER { XXX_ARRAY_INITIALIZER }

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_stack xxx_stack_t;

static inline int xxx_stack_init(xxx_stack_t *self);
static inline void xxx_stack_deinit(xxx_stack_t *self);
static inline int xxx_stack_copy(xxx_stack_t *dst, const xxx_stack_t *src);
static inline void xxx_stack_move(xxx_stack_t *dst, xxx_stack_t *src);
static inline size_t xxx_stack_size(const xxx_stack_t *self);
static inline bool xxx_stack_empty(const xxx_stack_t *self);
static inline void **xxx_stack_top(xxx_stack_t *self);
static inline int xxx_stack_push(xxx_stack_t *self, void *x);
static inline void xxx_stack_pop(xxx_stack_t *self);
static inline void xxx_stack_clear(xxx_stack_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_stack {
    xxx_array_t c;
};

static inline
int xxx_stack_init(xxx_stack_t *self) {
    return xxx_array_init(&self->c);
}

static inline
void xxx_stack_deinit(xxx_stack_t *self) {
    xxx_array_deinit(&self->c);
}

static inline
int xxx_stack_copy(xxx_stack_t *dst, const xxx_stack_t *src) {
    return xxx_array_copy(&dst->c, &src->c);
}

static inline
void xxx_stack_move(xxx_stack_t *dst, xxx_stack_t *src) {
    xxx_array_move(&dst->c, &src->c);
}

static inline
size_t xxx_stack_size(const xxx_stack_t *self) {
    return xxx_array_length(&self->c);
}

static inline
bool xxx_stack_empty(const xxx_stack_t *self) {
    return xxx_array_empty(&self->c);
}

static inline
void **xxx_stack_top(xxx_stack_t *self) {
#if XXX_STACK_DEBUG
    XXX_STACK_ASSERT(!xxx_stack_empty(self), "stack is empty");
#endif

    return xxx_array_back(&self->c);
}

static inline
int xxx_stack_push(xxx_stack_t *self, void *x) {
    return xxx_array_push_back(&self->c, x);
}

static inline
void xxx_stack_pop(xxx_stack_t *self) {
#if XXX_STACK_DEBUG
    XXX_STACK_ASSERT(!xxx_stack_empty(self), "stack underflow");
#endif

    xxx_array_pop_back(&self->c);
}

static inline
void xxx_stack_clear(xxx_stack_t *self) {
    xxx_array_clear(&self->c);
}

#endif
