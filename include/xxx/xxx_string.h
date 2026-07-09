#ifndef XXX_STRING_H
#define XXX_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "xxx_config.h"
#include "xxx_allocator.h"
#include "xxx_assert.h"

#ifndef XXX_STRING_DEBUG
#  define XXX_STRING_DEBUG XXX_DEBUG
#endif

#ifndef XXX_STRING_ASSERT
#  define XXX_STRING_ASSERT XXX_ASSERT
#endif

#ifndef XXX_STRING_ALLOCATOR
#  define XXX_STRING_FREE    XXX_FREE
#  define XXX_STRING_MALLOC  XXX_MALLOC
#  define XXX_STRING_REALLOC XXX_REALLOC
#endif

#define XXX_STRING_CAPACITY_MAX ((size_t)0x7fffffff)

// #define XXX_STRING_INITIALIZER {{0}, 0x80}

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_string xxx_string_t;

static inline int xxx_string_init(xxx_string_t *self);
static inline void xxx_string_deinit(xxx_string_t *self);
static inline int xxx_string_copy(xxx_string_t *dst, const xxx_string_t *src);
static inline void xxx_string_move(xxx_string_t *dst, xxx_string_t *src);
static inline size_t xxx_string_length(const xxx_string_t *self);
static inline size_t xxx_string_capacity(const xxx_string_t *self);
static inline bool xxx_string_empty(const xxx_string_t *self);
static inline char *xxx_string_c_str(xxx_string_t *self);
static inline char *xxx_string_data(xxx_string_t *self);
static inline const char *xxx_string_data_const(const xxx_string_t *self);
static inline char *xxx_string_at(xxx_string_t *self, size_t pos);
static inline const char *xxx_string_at_const(const xxx_string_t *self, size_t pos);
static inline char *xxx_string_front(xxx_string_t *self);
static inline const char *xxx_string_front_const(const xxx_string_t *self);
static inline char *xxx_string_back(xxx_string_t *self);
static inline const char *xxx_string_back_const(const xxx_string_t *self);
static inline int xxx_string_reserve(xxx_string_t *self, size_t n);
static inline int xxx_string_assign(xxx_string_t *self, const char *str, size_t len);
static inline int xxx_string_append(xxx_string_t *self, const char *str, size_t len);
static inline int xxx_string_push_back(xxx_string_t *self, char c);
static inline void xxx_string_pop_back(xxx_string_t *self);
static inline void xxx_string_clear(xxx_string_t *self);

#ifdef __cplusplus
}
#endif

typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} xxx_long_string_t;

typedef struct {
    char buf[sizeof(xxx_long_string_t) - 1];
    uint8_t len;
} xxx_short_string_t;

#define XXX_SHORT_STRING_FLAG       0x80
#define XXX_SHORT_STRING_LEN_MASK   0x7f
#define XXX_SHORT_STRING_CAPACITY   ((size_t)22)

struct xxx_string {
    union {
        xxx_long_string_t l;
        xxx_short_string_t s;
    };
};

static inline
bool xxx_string_isshort(const xxx_string_t *self) {
    return (self->s.len & XXX_SHORT_STRING_FLAG) != 0;
}

static inline
int xxx_long_string_grow(xxx_string_t *self, size_t new_cap) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_isshort(self), "string must be in long representation");

    XXX_STRING_ASSERT(
        new_cap > self->l.cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->l.cap);

    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum capacity %zu", new_cap, XXX_STRING_CAPACITY_MAX);
#endif

    char *new_buf = (char *)XXX_STRING_REALLOC(self->l.buf, new_cap + 1);
    if (new_buf == NULL) {
        return -1;
    }
    self->l.buf = new_buf;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_short_string_grow(xxx_string_t *self, size_t new_cap) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(xxx_string_isshort(self), "string must be in short representation");

    XXX_STRING_ASSERT(
        new_cap > XXX_SHORT_STRING_CAPACITY,
        "new capacity %zu must be greater than current capacity %zu", new_cap, XXX_SHORT_STRING_CAPACITY);

    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum capacity %zu", new_cap, XXX_STRING_CAPACITY_MAX);
#endif

    char *new_buf = (char *)XXX_STRING_MALLOC(new_cap + 1);
    if (new_buf == NULL) {
        return -1;
    }
    size_t len = self->s.len & XXX_SHORT_STRING_LEN_MASK;
    memcpy(new_buf, self->s.buf, len);
    self->l.buf = new_buf;
    self->l.len = len;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_string_init(xxx_string_t *self) {
    self->s.len = XXX_SHORT_STRING_FLAG;
    return 0;
}

static inline
void xxx_string_deinit(xxx_string_t *self) {
    if (!xxx_string_isshort(self)) {
        XXX_STRING_FREE(self->l.buf);
        self->l.buf = NULL;
    }
}

static inline
int xxx_string_copy(xxx_string_t *dst, const xxx_string_t *src) {
    if (dst == src) {
        return 0;
    }
    if (xxx_string_isshort(src)) {
        if (xxx_string_isshort(dst)) {
            *dst = *src;
            return 0;
        }
        size_t len = src->s.len & XXX_SHORT_STRING_LEN_MASK;
        memcpy(dst->l.buf, src->s.buf, len);
        dst->l.len = len;
        return 0;
    }
    char *old_buf = NULL;
    size_t len = src->l.len;
    if (!xxx_string_isshort(dst)) {
        if (dst->l.cap >= len) {
            memcpy(dst->l.buf, src->l.buf, len);
            dst->l.len = len;
            return 0;
        }
        old_buf = dst->l.buf;
    }
    char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
    if (new_buf == NULL) {
        return -1;
    }
    memcpy(new_buf, src->l.buf, len);
    XXX_STRING_FREE(old_buf);
    dst->l.buf = new_buf;
    dst->l.len = len;
    dst->l.cap = len;
    return 0;
}

static inline
void xxx_string_move(xxx_string_t *dst, xxx_string_t *src) {
    if (!xxx_string_isshort(dst)) {
        XXX_STRING_FREE(dst->l.buf);
    }
    *dst = *src;
    src->l.buf = NULL;
}

static inline
size_t xxx_string_length(const xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        return self->s.len & XXX_SHORT_STRING_LEN_MASK;
    }
    return self->l.len;
}

static inline
size_t xxx_string_capacity(const xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        return XXX_SHORT_STRING_CAPACITY;
    }
    return self->l.cap;
}

static inline
bool xxx_string_empty(const xxx_string_t *self) {
    return xxx_string_length(self) == 0;
}

static inline
char *xxx_string_c_str(xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        self->s.buf[self->s.len & XXX_SHORT_STRING_LEN_MASK] = '\0';
        return self->s.buf;
    }
    self->l.buf[self->l.len] = '\0';
    return self->l.buf;
}

static inline
char *xxx_string_data(xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        return self->s.buf;
    }
    return self->l.buf;
}

static inline
const char *xxx_string_data_const(const xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        return self->s.buf;
    }
    return self->l.buf;
}

static inline
char *xxx_string_at(xxx_string_t *self, size_t pos) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(
        pos < xxx_string_length(self),
        "index %zu out of range [0, %zu)", pos, xxx_string_length(self));
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[pos];
    }
    return &self->l.buf[pos];
}

static inline
const char *xxx_string_at_const(const xxx_string_t *self, size_t pos) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(
        pos < xxx_string_length(self),
        "index %zu out of range [0, %zu)", pos, xxx_string_length(self));
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[pos];
    }
    return &self->l.buf[pos]; 
}

static inline
char *xxx_string_front(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[0];
    }
    return &self->l.buf[0];
}

static inline
const char *xxx_string_front_const(const xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[0];
    }
    return &self->l.buf[0];   
}

static inline
char *xxx_string_back(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[(self->s.len & XXX_SHORT_STRING_LEN_MASK) - 1];
    }
    return &self->l.buf[self->l.len - 1];
}

static inline
const char *xxx_string_back_const(const xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif

    if (xxx_string_isshort(self)) {
        return &self->s.buf[(self->s.len & XXX_SHORT_STRING_LEN_MASK) - 1];
    }
    return &self->l.buf[self->l.len - 1];
}

static inline
int xxx_string_reserve(xxx_string_t *self, size_t n) {
    if (n > XXX_STRING_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_string_isshort(self)) {
        if (n <= XXX_SHORT_STRING_CAPACITY) {
            return 0;
        }
        return xxx_short_string_grow(self, n);
    }
    if (n <= self->l.cap) {
        return 0;
    }
    return xxx_long_string_grow(self, n);
}

/*
 * Undefined behavior if str overlaps the contents of self.
 */
static inline
int xxx_string_assign(xxx_string_t *self, const char *str, size_t len) {
    if (len <= XXX_SHORT_STRING_CAPACITY) {
        if (xxx_string_isshort(self)) {
            memcpy(self->s.buf, str, len);
            self->s.len = len | XXX_SHORT_STRING_FLAG;
            return 0;
        }
        memcpy(self->l.buf, str, len);
        self->l.len = len;
        return 0;
    }
    if (len > XXX_STRING_CAPACITY_MAX) {
        return -1;
    }
    char *old_buf = NULL;
    if (!xxx_string_isshort(self)) {
        if (self->l.cap >= len) {
            memcpy(self->l.buf, str, len);
            self->l.len = len;
            return 0;
        }
        old_buf = self->l.buf;
    }
    char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
    if (new_buf == NULL) {
        return -1;
    }
    memcpy(new_buf, str, len);
    XXX_STRING_FREE(old_buf);
    self->l.buf = new_buf;
    self->l.len = len;
    self->l.cap = len;
    return 0;
}

/*
 * Undefined behavior if str overlaps the contents of self.
 */
static inline
int xxx_string_append(xxx_string_t *self, const char *str, size_t len) {
    if (len > XXX_STRING_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_string_isshort(self)) {
        size_t old_len = self->s.len & XXX_SHORT_STRING_LEN_MASK;
        size_t new_len = old_len + len;
        if (new_len <= XXX_SHORT_STRING_CAPACITY) {
            memcpy(self->s.buf + old_len, str, len);
            self->s.len = new_len | XXX_SHORT_STRING_FLAG;
            return 0;
        }
        size_t new_cap = XXX_SHORT_STRING_CAPACITY * 2;
        if (new_cap < new_len) {
            new_cap = new_len;
        }
        char *new_buf = (char *)XXX_STRING_MALLOC(new_cap + 1);
        if (new_buf == NULL) {
            return -1;
        }
        memcpy(new_buf, self->s.buf, old_len);
        memcpy(new_buf + old_len, str, len);
        self->l.buf = new_buf;
        self->l.len = new_len;
        self->l.cap = new_cap;
        return 0;
    }
    size_t new_len = self->l.len + len;
    if (new_len <= self->l.cap) {
        memcpy(self->l.buf + self->l.len, str, len);
        self->l.len = new_len;
        return 0;
    }
    if (new_len > XXX_STRING_CAPACITY_MAX) {
        return -1;
    }
    size_t new_cap = self->l.cap * 2;
    if (new_cap < new_len) {
        new_cap = new_len;
    }
    if (new_cap > XXX_STRING_CAPACITY_MAX) {
        new_cap = XXX_STRING_CAPACITY_MAX;
    }
    char *new_buf = (char *)XXX_STRING_MALLOC(new_cap + 1);
    if (new_buf == NULL) {
        return -1;
    }
    memcpy(new_buf, self->l.buf, self->l.len);
    memcpy(new_buf + self->l.len, str, len);
    XXX_STRING_FREE(self->l.buf);
    self->l.buf = new_buf;
    self->l.len = new_len;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_string_push_back(xxx_string_t *self, char c) {
    if (xxx_string_isshort(self)) {
        size_t len = self->s.len & XXX_SHORT_STRING_LEN_MASK;
        if (len < XXX_SHORT_STRING_CAPACITY) {
            self->s.buf[len] = c;
            ++self->s.len;
            return 0;
        }
        if (xxx_short_string_grow(self, XXX_SHORT_STRING_CAPACITY * 2) != 0) {
            return -1;
        }
    } else {
        if (self->l.len == self->l.cap) {
            if (self->l.cap == XXX_STRING_CAPACITY_MAX) {
                return -1;
            }
            size_t new_cap = self->l.cap * 2;
            if (new_cap > XXX_STRING_CAPACITY_MAX) {
                new_cap = XXX_STRING_CAPACITY_MAX;
            }
            if (xxx_long_string_grow(self, new_cap) != 0) {
                return -1;
            }
        }
    }
    self->l.buf[self->l.len++] = c;
    return 0;
}

static inline
void xxx_string_pop_back(xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif

    if (xxx_string_isshort(self)) {
        --self->s.len;
        return;
    }
    --self->l.len;
}

static inline
void xxx_string_clear(xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        self->s.len = XXX_SHORT_STRING_FLAG;
        return;
    }
    self->l.len = 0;
}

#endif
