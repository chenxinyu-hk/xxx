#ifndef XXX_I32HEAP_H
#define XXX_I32HEAP_H

#include <stdbool.h>

#include "xxx_config.h"
#include "xxx_assert.h"
#include "xxx_i32array.h"

#ifndef XXX_I32HEAP_DEBUG
#  define XXX_I32HEAP_DEBUG XXX_DEBUG
#endif

#ifndef XXX_I32HEAP_ASSERT
#  define XXX_I32HEAP_ASSERT XXX_ASSERT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32maxheap xxx_i32maxheap_t;

static inline int xxx_i32maxheap_init(xxx_i32maxheap_t *self);
static inline void xxx_i32maxheap_deinit(xxx_i32maxheap_t *self);
static inline int xxx_i32maxheap_copy(xxx_i32maxheap_t *dst, const xxx_i32maxheap_t *src);
static inline void xxx_i32maxheap_move(xxx_i32maxheap_t *dst, xxx_i32maxheap_t *src);
static inline bool xxx_i32maxheap_empty(const xxx_i32maxheap_t *self);
static inline size_t xxx_i32maxheap_size(const xxx_i32maxheap_t *self);
static inline int xxx_i32maxheap_reserve(xxx_i32maxheap_t *self, size_t n);
static inline int xxx_i32maxheap_assign(xxx_i32maxheap_t *self, const int *arr, size_t len);
static inline const int *xxx_i32maxheap_top(const xxx_i32maxheap_t *self);
static inline int xxx_i32maxheap_push(xxx_i32maxheap_t *self, int x);
static inline void xxx_i32maxheap_pop(xxx_i32maxheap_t *self);
static inline void xxx_i32maxheap_clear(xxx_i32maxheap_t *self);

static inline void xxx_i32maxheap_build(int *arr, size_t len);
static inline void xxx_i32maxheap_sift_up(int *arr, size_t len, size_t i);
static inline void xxx_i32maxheap_sift_down(int *arr, size_t len, size_t i);

typedef struct xxx_i32minheap xxx_i32minheap_t;

static inline int xxx_i32minheap_init(xxx_i32minheap_t *self);
static inline void xxx_i32minheap_deinit(xxx_i32minheap_t *self);
static inline int xxx_i32minheap_copy(xxx_i32minheap_t *dst, const xxx_i32minheap_t *src);
static inline void xxx_i32minheap_move(xxx_i32minheap_t *dst, xxx_i32minheap_t *src);
static inline bool xxx_i32minheap_empty(const xxx_i32minheap_t *self);
static inline size_t xxx_i32minheap_size(const xxx_i32minheap_t *self);
static inline int xxx_i32minheap_reserve(xxx_i32minheap_t *self, size_t n);
static inline int xxx_i32minheap_assign(xxx_i32minheap_t *self, const int *arr, size_t len);
static inline const int *xxx_i32minheap_top(const xxx_i32minheap_t *self);
static inline int xxx_i32minheap_push(xxx_i32minheap_t *self, int x);
static inline void xxx_i32minheap_pop(xxx_i32minheap_t *self);
static inline void xxx_i32minheap_clear(xxx_i32minheap_t *self);

static inline void xxx_i32minheap_build(int *arr, size_t len);
static inline void xxx_i32minheap_sift_up(int *arr, size_t len, size_t i);
static inline void xxx_i32minheap_sift_down(int *arr, size_t len, size_t i);

typedef xxx_i32maxheap_t xxx_i32heap_t;

#ifdef __cplusplus
}
#endif

struct xxx_i32maxheap {
    xxx_i32array_t arr;
};

static inline
void xxx_i32maxheap_build(int *arr, size_t len) {
    size_t i = len / 2;
    while (i > 0) {
        xxx_i32maxheap_sift_down(arr, len, --i);
    }
}

static inline
void xxx_i32maxheap_sift_up(int *arr, size_t len, size_t i) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(i < len, "index %zu out of range [0, %zu)", i, len);
#endif

    (void)len;
    int target = arr[i];
    size_t curr = i;
    while (curr > 0) {
        size_t prev = (curr - 1) / 2;
        if (arr[prev] >= target) {
            break;
        }
        arr[curr] = arr[prev];
        curr = prev;
    }
    arr[curr] = target;
}

static inline
void xxx_i32maxheap_sift_down(int *arr, size_t len, size_t i) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(i < len, "index %zu out of range [0, %zu)", i, len);
#endif

    int target = arr[i];
    size_t curr = i;
    size_t half = len / 2;
    while (curr < half) {
        size_t left = curr * 2 + 1;
        size_t right = left + 1;
        size_t largest = left;
        if (right < len && arr[right] > arr[left]) {
            largest = right;
        }
        if (arr[largest] <= target) {
            break;
        }
        arr[curr] = arr[largest];
        curr = largest;
    }
    arr[curr] = target;
}

static inline
int xxx_i32maxheap_init(xxx_i32maxheap_t *self) {
    return xxx_i32array_init(&self->arr);
}

static inline
void xxx_i32maxheap_deinit(xxx_i32maxheap_t *self) {
    xxx_i32array_deinit(&self->arr);
}

static inline
int xxx_i32maxheap_copy(xxx_i32maxheap_t *dst, const xxx_i32maxheap_t *src) {
    return xxx_i32array_copy(&dst->arr, &src->arr);
}

static inline
void xxx_i32maxheap_move(xxx_i32maxheap_t *dst, xxx_i32maxheap_t *src) {
    xxx_i32array_move(&dst->arr, &src->arr);
}

static inline
bool xxx_i32maxheap_empty(const xxx_i32maxheap_t *self) {
    return xxx_i32array_empty(&self->arr);
}

static inline
size_t xxx_i32maxheap_size(const xxx_i32maxheap_t *self) {
    return xxx_i32array_length(&self->arr);
}

static inline
int xxx_i32maxheap_reserve(xxx_i32maxheap_t *self, size_t n) {
    return xxx_i32array_reserve(&self->arr, n);
}

static inline
int xxx_i32maxheap_assign(xxx_i32maxheap_t *self, const int *arr, size_t len) {
    if (xxx_i32array_assign(&self->arr, arr, len) != 0) {
        return -1;
    }
    xxx_i32maxheap_build(
        xxx_i32array_data(&self->arr), xxx_i32array_length(&self->arr));
    return 0;
}

static inline
const int *xxx_i32maxheap_top(const xxx_i32maxheap_t *self) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(!xxx_i32maxheap_empty(self), "heap is empty");
#endif

    return xxx_i32array_front_const(&self->arr);
}

static inline
int xxx_i32maxheap_push(xxx_i32maxheap_t *self, int x) {
    if (xxx_i32array_push_back(&self->arr, x) != 0) {
        return -1;
    }
    int *arr = xxx_i32array_data(&self->arr);
    size_t len = xxx_i32array_length(&self->arr);
    xxx_i32maxheap_sift_up(arr, len, len - 1);
    return 0;
}

static inline
void xxx_i32maxheap_pop(xxx_i32maxheap_t *self) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(!xxx_i32maxheap_empty(self), "heap is empty");
#endif

    *xxx_i32array_front(&self->arr) = *xxx_i32array_back(&self->arr);
    xxx_i32array_pop_back(&self->arr);
    if (!xxx_i32array_empty(&self->arr)) {
        xxx_i32maxheap_sift_down(
            xxx_i32array_data(&self->arr), xxx_i32array_length(&self->arr), 0);
    }
}

static inline
void xxx_i32maxheap_clear(xxx_i32maxheap_t *self) {
    xxx_i32array_clear(&self->arr);
}

struct xxx_i32minheap {
    xxx_i32array_t arr;
};

static inline
void xxx_i32minheap_build(int *arr, size_t len) {
    size_t i = len / 2;
    while (i > 0) {
        xxx_i32minheap_sift_down(arr, len, --i);
    }
}

static inline
void xxx_i32minheap_sift_up(int *arr, size_t len, size_t i) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(i < len, "index %zu out of range [0, %zu)", i, len);
#endif

    (void)len;
    int target = arr[i];
    size_t curr = i;
    while (curr > 0) {
        size_t prev = (curr - 1) / 2;
        if (arr[prev] <= target) {
            break;
        }
        arr[curr] = arr[prev];
        curr = prev;
    }
    arr[curr] = target;
}

static inline
void xxx_i32minheap_sift_down(int *arr, size_t len, size_t i) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(i < len, "index %zu out of range [0, %zu)", i, len);
#endif

    int target = arr[i];
    size_t curr = i;
    size_t half = len / 2;
    while (curr < half) {
        size_t left = curr * 2 + 1;
        size_t right = left + 1;
        size_t largest = left;
        if (right < len && arr[right] < arr[left]) {
            largest = right;
        }
        if (arr[largest] >= target) {
            break;
        }
        arr[curr] = arr[largest];
        curr = largest;
    }
    arr[curr] = target;
}

static inline
int xxx_i32minheap_init(xxx_i32minheap_t *self) {
    return xxx_i32array_init(&self->arr);
}

static inline
void xxx_i32minheap_deinit(xxx_i32minheap_t *self) {
    xxx_i32array_deinit(&self->arr);
}

static inline
int xxx_i32minheap_copy(xxx_i32minheap_t *dst, const xxx_i32minheap_t *src) {
    return xxx_i32array_copy(&dst->arr, &src->arr);
}

static inline
void xxx_i32minheap_move(xxx_i32minheap_t *dst, xxx_i32minheap_t *src) {
    xxx_i32array_move(&dst->arr, &src->arr);
}

static inline
bool xxx_i32minheap_empty(const xxx_i32minheap_t *self) {
    return xxx_i32array_empty(&self->arr);
}

static inline
size_t xxx_i32minheap_size(const xxx_i32minheap_t *self) {
    return xxx_i32array_length(&self->arr);
}

static inline
int xxx_i32minheap_reserve(xxx_i32minheap_t *self, size_t n) {
    return xxx_i32array_reserve(&self->arr, n);
}

static inline
int xxx_i32minheap_assign(xxx_i32minheap_t *self, const int *arr, size_t len) {
    if (xxx_i32array_assign(&self->arr, arr, len) != 0) {
        return -1;
    }
    xxx_i32minheap_build(
        xxx_i32array_data(&self->arr), xxx_i32array_length(&self->arr));
    return 0;
}

static inline
const int *xxx_i32minheap_top(const xxx_i32minheap_t *self) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(!xxx_i32minheap_empty(self), "heap is empty");
#endif

    return xxx_i32array_front_const(&self->arr);
}

static inline
int xxx_i32minheap_push(xxx_i32minheap_t *self, int x) {
    if (xxx_i32array_push_back(&self->arr, x) != 0) {
        return -1;
    }
    int *arr = xxx_i32array_data(&self->arr);
    size_t len = xxx_i32array_length(&self->arr);
    xxx_i32minheap_sift_up(arr, len, len - 1);
    return 0;
}

static inline
void xxx_i32minheap_pop(xxx_i32minheap_t *self) {
#if XXX_I32HEAP_DEBUG
    XXX_I32HEAP_ASSERT(!xxx_i32minheap_empty(self), "heap is empty");
#endif

    *xxx_i32array_front(&self->arr) = *xxx_i32array_back(&self->arr);
    xxx_i32array_pop_back(&self->arr);
    if (!xxx_i32array_empty(&self->arr)) {
        xxx_i32minheap_sift_down(
            xxx_i32array_data(&self->arr), xxx_i32array_length(&self->arr), 0);
    }
}

static inline
void xxx_i32minheap_clear(xxx_i32minheap_t *self) {
    xxx_i32array_clear(&self->arr);
}

#endif
