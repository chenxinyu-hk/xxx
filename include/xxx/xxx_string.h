#ifndef XXX_STRING_H
#define XXX_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "xxx_allocator.h"
#include "xxx_assert.h"

#ifndef XXX_STRING_DEBUG
#  ifdef DEBUG
#    define XXX_STRING_DEBUG 1
#  else
#    define XXX_STRING_DEBUG 0
#  endif
#endif

#ifndef XXX_STRING_ASSERT
#  define XXX_STRING_ASSERT XXX_ASSERT
#endif

#ifndef XXX_STRING_ALLOCATOR
#  define XXX_STRING_FREE    XXX_FREE
#  define XXX_STRING_MALLOC  XXX_MALLOC
#  define XXX_STRING_REALLOC XXX_REALLOC
#endif

#define XXX_STRING_CAPACITY_MAX ((size_t)0x7ffffffe)

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
static inline char *xxx_string_at(xxx_string_t *self, size_t pos);
static inline const char *xxx_string_at_const(const xxx_string_t *self, size_t pos);
static inline char *xxx_string_front(xxx_string_t *self);
static inline const char *xxx_string_front(const xxx_string_t *self);
static inline char *xxx_string_back(xxx_string_t *self);
static inline const char *xxx_string_back(const xxx_string_t *self);
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

struct xxx_string {
    union {
        xxx_long_string_t l;
        xxx_short_string_t s;
    };
};

static inline
int xxx_string_isshort(const xxx_string_t *self) {
    return (self->s.len & 0x80) != 0;
}

static inline
int xxx_long_string_grow(xxx_string_t *self, size_t new_cap) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_isshort(self), "string must be long");
    XXX_STRING_ASSERT(
        new_cap > self->l.cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->l.cap);
    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_STRING_CAPACITY_MAX);
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
    XXX_STRING_ASSERT(xxx_string_isshort(self), "string must be short");
    XXX_STRING_ASSERT(
        new_cap > 22,
        "new capacity %zu must be greater than current capacity 22", new_cap);
    XXX_STRING_ASSERT(
        new_cap <= XXX_STRING_CAPACITY_MAX,
        "new capacity %zu exceeds maximum %zu", new_cap, XXX_STRING_CAPACITY_MAX);
#endif
    char *new_buf = (char *)XXX_STRING_MALLOC(new_cap + 1);
    if (new_buf == NULL) {
        return -1;
    }
    size_t len = self->s.len & 0x7f;
    memcpy(new_buf, self->s.buf, len);
    self->l.buf = new_buf;
    self->l.len = len;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_string_init(xxx_string_t *self) {
    self->s.len = 0x80;
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
        size_t len = src->s.len & 0x7f;
        memcpy(dst->l.buf, src->s.buf, len);
        dst->l.len = len;
        return 0;
    }
    size_t len = src->l.len;
    if (xxx_string_isshort(dst)) {
        char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
        if (new_buf == NULL) {
            return -1;
        }
        dst->l.buf = new_buf;
        dst->l.cap = len;
    } else if (dst->l.cap < len) {
        char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
        if (new_buf == NULL) {
            return -1;
        }
        XXX_STRING_FREE(dst->l.buf);
        dst->l.buf = new_buf;
        dst->l.cap = len;
    }
    memcpy(dst->l.buf, src->l.buf, len);
    dst->l.len = len;
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
        return self->s.len & 0x7f;
    }
    return self->l.len;
}

static inline
size_t xxx_string_capacity(const xxx_string_t *self) {
    if (xxx_string_isshort(self)) {
        return 22;
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
        self->s.buf[self->s.len & 0x7f] = '\0';
        return self->s.buf;
    }
    self->l.buf[self->l.len] = '\0';
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
const char *xxx_string_front(const xxx_string_t *self) {
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
        return &self->s.buf[(self->s.len & 0x7f) - 1];
    }
    return &self->l.buf[self->l.len - 1];
}

static inline
const char *xxx_string_back(const xxx_string_t *self) {
#if XXX_STRING_DEBUG
    XXX_STRING_ASSERT(!xxx_string_empty(self), "string is empty");
#endif
    if (xxx_string_isshort(self)) {
        return &self->s.buf[(self->s.len & 0x7f) - 1];
    }
    return &self->l.buf[self->l.len - 1];
}

static inline
int xxx_string_reserve(xxx_string_t *self, size_t n) {
    if (n > XXX_STRING_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_string_isshort(self)) {
        if (n <= 22) {
            return 0;
        }
        return xxx_short_string_grow(self, n);
    }
    if (n <= self->l.cap) {
        return 0;
    }
    return xxx_long_string_grow(self, n);
}

// static inline
// int xxx_string_assign(xxx_string_t *self, const char *str, size_t len) {
//     bool is_short = xxx_string_isshort(self);
//     if (is_short) {
//         xxx_short_string_t *sstr = &self->s;
//         if (len <= 22) {
//             memcpy(sstr->buf, str, len);
//             sstr->len = len | 0x80;
//             return 0;
//         }
//     } else {
//         xxx_long_string_t *lstr = &self->l;
//         if (len <= lstr->cap) {
//             memcpy(lstr->buf, str, len);
//             lstr->len = len;
//             return 0;
//         }
//     }
//     if (len > XXX_STRING_CAPACITY_MAX) {
//         return -1;
//     }
//     char *new_buf = (char *)XXX_STRING_MALLOC(len + 1);
//     if (new_buf == NULL) {
//         return -1;
//     }
//     memcpy(new_buf, str, len);
//     xxx_long_string_t *lstr = &self->l;
//     if (!is_short) {
//         XXX_STRING_FREE(lstr->buf);
//     }
//     lstr->buf = new_buf;
//     lstr->len = len;
//     lstr->cap = len;
//     return 0;
// }

// static inline
// int xxx_string_append(xxx_string_t *self, const char *str, size_t len) {
//     char *buf;
//     if (xxx_string_isshort(self)) {
//         // xxx_short_string_t *sstr = &self->s;
//         size_t len = self->s.len & 0x7f;
//         if (len > XXX_STRING_CAPACITY_MAX - len) {
//             return -1;
//         }
//         buf = self->s.buf;
//         size_t new_len = len + len;
//         if (new_len <= 22) {
//             memcpy(self->s.buf + len, str, len);
//             self->s.len += len;
//             return 0;
//         }
//         size_t new_cap = 44;
//         if (new_cap < new_len) {
//             new_cap = new_len;
//         }
//         if (xxx_short_string_grow(self, new_cap) != 0) {
//             return -1;
//         }
//     } else {
//         xxx_long_string_t *lstr = &self->l;
//         if (len > XXX_STRING_CAPACITY_MAX - lstr->len) {
//             return -1;
//         }
//         buf = lstr->buf;
//         size_t new_len = lstr->len + len;
//         if (new_len > lstr->cap) {
//             size_t new_cap = lstr->cap << 1;
//             if (new_cap < new_len || new_cap > XXX_STRING_CAPACITY_MAX) {
//                 new_cap = new_len;
//             }
//             if (xxx_long_string_grow(lstr, new_cap) != 0) {
//                 return -1;
//             }
//         }
//     }
//     xxx_long_string_t *lstr = &self->l;
//     if (str >= buf && str < buf + lstr->len) {
//         str = lstr->buf + (str - buf);
//     }
//     memcpy(lstr->buf + lstr->len, str, len);
//     lstr->len += len;
//     return 0;
// }

static inline
int xxx_string_push_back(xxx_string_t *self, char c) {
    if (xxx_string_isshort(self)) {
        size_t len = self->s.len & 0x7f;
        if (len < 22) {
            self->s.buf[len] = c;
            ++self->s.len;
            return 0;
        }
        if (xxx_short_string_grow(self, 44) != 0) {
            return -1;
        }
    } else {
        if (self->l.len == self->l.cap) {
            if (self->l.cap == XXX_STRING_CAPACITY_MAX) {
                return -1;
            }
            size_t new_cap = self->l.cap << 1;
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
        self->s.len = 0x80;
        return;
    }
    self->l.len = 0;
}

#endif
