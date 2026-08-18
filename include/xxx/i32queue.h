#ifndef XXX_I32QUEUE_H
#define XXX_I32QUEUE_H

#include "xxx/i32deque.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32queue xxx_i32queue_t;

static inline int xxx_i32queue_init(xxx_i32queue_t *self);
static inline void xxx_i32queue_deinit(xxx_i32queue_t *self);
static inline int xxx_i32queue_copy(xxx_i32queue_t *dst, const xxx_i32queue_t *src);
static inline void xxx_i32queue_move(xxx_i32queue_t *dst, xxx_i32queue_t *src);
static inline size_t xxx_i32queue_size(const xxx_i32queue_t *self);
static inline size_t xxx_i32queue_capacity(const xxx_i32queue_t *self);
static inline bool xxx_i32queue_empty(const xxx_i32queue_t *self);
static inline int *xxx_i32queue_front(xxx_i32queue_t *self);
static inline const int *xxx_i32queue_front_const(const xxx_i32queue_t *self);
static inline int *xxx_i32queue_back(xxx_i32queue_t *self);
static inline const int *xxx_i32queue_back_const(const xxx_i32queue_t *self);
static inline int xxx_i32queue_reserve(xxx_i32queue_t *self, size_t n);
static inline int xxx_i32queue_push(xxx_i32queue_t *self, int x);
static inline void xxx_i32queue_pop(xxx_i32queue_t *self);
static inline void xxx_i32queue_clear(xxx_i32queue_t *self);

#ifdef __cplusplus
}
#endif

struct xxx_i32queue {
    xxx_i32deque_t c;
};

static inline
int xxx_i32queue_init(xxx_i32queue_t *self) {
    return xxx_i32deque_init(&self->c);
}

static inline
void xxx_i32queue_deinit(xxx_i32queue_t *self) {
    xxx_i32deque_deinit(&self->c);
}

static inline
int xxx_i32queue_copy(xxx_i32queue_t *dst, const xxx_i32queue_t *src) {
    return xxx_i32deque_copy(&dst->c, &src->c);
}

static inline
void xxx_i32queue_move(xxx_i32queue_t *dst, xxx_i32queue_t *src) {
    xxx_i32deque_move(&dst->c, &src->c);
}

static inline
size_t xxx_i32queue_size(const xxx_i32queue_t *self) {
    return xxx_i32deque_size(&self->c);
}

static inline
size_t xxx_i32queue_capacity(const xxx_i32queue_t *self) {
    return xxx_i32deque_capacity(&self->c);
}

static inline
bool xxx_i32queue_empty(const xxx_i32queue_t *self) {
    return xxx_i32deque_empty(&self->c);
}

static inline
int *xxx_i32queue_front(xxx_i32queue_t *self) {
    return xxx_i32deque_front(&self->c);
}

static inline
const int *xxx_i32queue_front_const(const xxx_i32queue_t *self) {
    return xxx_i32deque_front_const(&self->c);
}

static inline
int *xxx_i32queue_back(xxx_i32queue_t *self) {
    return xxx_i32deque_back(&self->c);
}

static inline
const int *xxx_i32queue_back_const(const xxx_i32queue_t *self) {
    return xxx_i32deque_back_const(&self->c);
}

static inline
int xxx_i32queue_reserve(xxx_i32queue_t *self, size_t n) {
    return xxx_i32deque_reserve(&self->c, n);
}

static inline
int xxx_i32queue_push(xxx_i32queue_t *self, int x) {
    return xxx_i32deque_push_back(&self->c, x);
}

static inline
void xxx_i32queue_pop(xxx_i32queue_t *self) {
    xxx_i32deque_pop_front(&self->c);
}

static inline
void xxx_i32queue_clear(xxx_i32queue_t *self) {
    xxx_i32deque_clear(&self->c);
}

#endif
