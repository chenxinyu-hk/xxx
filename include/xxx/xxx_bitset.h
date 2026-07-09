#ifndef XXX_BITSET_H
#define XXX_BITSET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "xxx_config.h"
#include "xxx_allocator.h"
#include "xxx_assert.h"

#ifndef XXX_BITSET_DEBUG
#  define XXX_BITSET_DEBUG XXX_DEBUG
#endif

#ifndef XXX_BITSET_ASSERT
#  define XXX_BITSET_ASSERT XXX_ASSERT
#endif

#ifndef XXX_BITSET_ALLOCATOR
#  define XXX_BITSET_FREE    XXX_FREE
#  define XXX_BITSET_MALLOC  XXX_MALLOC
#  define XXX_BITSET_REALLOC XXX_REALLOC
#endif

#define XXX_BITSET_CAPACITY_MAX ((size_t)0x7fffffc0)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_bitset xxx_bitset_t;

static inline int xxx_bitset_init(xxx_bitset_t *self);
static inline void xxx_bitset_deinit(xxx_bitset_t *self);
static inline int xxx_bitset_copy(xxx_bitset_t *dst, const xxx_bitset_t *src);
static inline void xxx_bitset_move(xxx_bitset_t *dst, xxx_bitset_t *src);
static inline size_t xxx_bitset_capacity(const xxx_bitset_t *self);
static inline size_t xxx_bitset_count(const xxx_bitset_t *self);
static inline bool xxx_bitset_empty(const xxx_bitset_t *self);
static inline int xxx_bitset_reserve(xxx_bitset_t *self, size_t n);
static inline bool xxx_bitset_test(const xxx_bitset_t *self, size_t pos);
static inline int xxx_bitset_set(xxx_bitset_t *self, size_t pos);
static inline int xxx_bitset_reset(xxx_bitset_t *self, size_t pos);
static inline int xxx_bitset_flip(xxx_bitset_t *self, size_t pos);
static inline void xxx_bitset_clear(xxx_bitset_t *self);

#ifdef __cplusplus
}
#endif

typedef struct {
    uint64_t *words;
    size_t cap;
} xxx_long_bitset_t;

typedef struct {
    uint64_t words[sizeof(xxx_long_bitset_t) / sizeof(uint64_t)];
} xxx_short_bitset_t;

#define XXX_SHORT_BITSET_FLAG (1ULL << 63)
#define XXX_SHORT_BITSET_CAPACITY ((size_t)127)

struct xxx_bitset {
    union {
        xxx_long_bitset_t l;
        xxx_short_bitset_t s;
    };
};

#define XXX_BITSET_ALIGN(n) ((n + 63) & ~(size_t)63)

static inline
bool xxx_bitset_isshort(const xxx_bitset_t *self) {
    return (self->s.words[1] & XXX_SHORT_BITSET_FLAG) != 0;
}

static inline
int xxx_long_bitset_grow(xxx_bitset_t *self, size_t new_cap) {
#if XXX_BITSET_DEBUG
    XXX_BITSET_ASSERT(
        !xxx_bitset_isshort(self),
        "bitset must be in long representation");

    XXX_BITSET_ASSERT(
        new_cap == XXX_BITSET_ALIGN(new_cap),
        "new capacity %zu must be 64-bit aligned", new_cap);

    XXX_BITSET_ASSERT(
        new_cap > self->l.cap,
        "new capacity %zu must be greater than current capacity %zu", new_cap, self->l.cap);

    XXX_BITSET_ASSERT(
        new_cap <= XXX_BITSET_CAPACITY_MAX,
        "new capacity %zu exceeds maximum capacity %zu", new_cap, XXX_BITSET_CAPACITY_MAX);
#endif

    size_t old_nwords = self->l.cap >> 6;
    size_t new_nwords = new_cap >> 6;
    uint64_t *new_words = (uint64_t *)XXX_BITSET_REALLOC(self->l.words, new_nwords * sizeof(uint64_t));
    if (new_words == NULL) {
        return -1;
    }
    memset(new_words + old_nwords, 0, (new_nwords - old_nwords) * sizeof(uint64_t));
    self->l.words = new_words;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_short_bitset_grow(xxx_bitset_t *self, size_t new_cap) {
#if XXX_BITSET_DEBUG
    XXX_BITSET_ASSERT(
        xxx_bitset_isshort(self),
        "bitset must be in short representation");

    XXX_BITSET_ASSERT(
        new_cap == XXX_BITSET_ALIGN(new_cap),
        "new capacity %zu must be 64-bit aligned", new_cap);

    XXX_BITSET_ASSERT(
        new_cap > XXX_SHORT_BITSET_CAPACITY,
        "new capacity %zu must be greater than current capacity %zu", new_cap, XXX_SHORT_BITSET_CAPACITY);

    XXX_BITSET_ASSERT(
        new_cap <= XXX_BITSET_CAPACITY_MAX,
        "new capacity %zu exceeds maximum capacity %zu", new_cap, XXX_BITSET_CAPACITY_MAX);
#endif

    size_t nwords = new_cap >> 6;
    uint64_t *new_words = (uint64_t *)XXX_BITSET_MALLOC(nwords * sizeof(uint64_t));
    if (new_words == NULL) {
        return -1;
    }
    new_words[0] = self->s.words[0];
    new_words[1] = self->s.words[1] & ~XXX_SHORT_BITSET_FLAG;
    memset(new_words + 2, 0, (nwords - 2) * sizeof(uint64_t));
    self->l.words = new_words;
    self->l.cap = new_cap;
    return 0;
}

static inline
int xxx_bitset_init(xxx_bitset_t *self) {
    self->s.words[0] = 0;
    self->s.words[1] = XXX_SHORT_BITSET_FLAG;
    return 0;
}

static inline
void xxx_bitset_deinit(xxx_bitset_t *self) {
    if (!xxx_bitset_isshort(self)) {
        XXX_BITSET_FREE(self->l.words);
        self->l.words = NULL;
    }
}

static inline
int xxx_bitset_copy(xxx_bitset_t *dst, const xxx_bitset_t *src) {
    if (dst == src) {
        return 0;
    }
    if (xxx_bitset_isshort(src)) {
        if (xxx_bitset_isshort(dst)) {
            *dst = *src;
            return 0;
        }
        dst->l.words[0] = src->s.words[0];
        dst->l.words[1] = src->s.words[1] & ~XXX_SHORT_BITSET_FLAG;
        size_t src_nwords = 2;
        size_t dst_nwords = dst->l.cap >> 6;
        memset(dst->l.words + src_nwords, 0, (dst_nwords - src_nwords) * sizeof(uint64_t));
        return 0;
    }
    uint64_t *old_words = NULL;
    size_t src_nwords = src->l.cap >> 6;
    if (!xxx_bitset_isshort(dst)) {
        if (dst->l.cap >= src->l.cap) {
            size_t dst_nwords = dst->l.cap >> 6;
            memcpy(dst->l.words, src->l.words, src_nwords * sizeof(uint64_t));
            memset(dst->l.words + src_nwords, 0, (dst_nwords - src_nwords) * sizeof(uint64_t));
            return 0;
        }
        old_words = dst->l.words;
    }
    uint64_t *new_words = (uint64_t *)XXX_BITSET_MALLOC(src_nwords * sizeof(uint64_t));
    if (new_words == NULL) {
        return -1;
    }
    memcpy(new_words, src->l.words, src_nwords * sizeof(uint64_t));
    XXX_BITSET_FREE(old_words);
    dst->l.words = new_words;
    dst->l.cap = src->l.cap;
    return 0;
}

static inline
void xxx_bitset_move(xxx_bitset_t *dst, xxx_bitset_t *src) {
    if (!xxx_bitset_isshort(dst)) {
        XXX_BITSET_FREE(dst->l.words);
    }
    *dst = *src;
    src->l.words = NULL;
}

static inline
size_t xxx_bitset_capacity(const xxx_bitset_t *self) {
    if (xxx_bitset_isshort(self)) {
        return XXX_SHORT_BITSET_CAPACITY;
    }
    return self->l.cap;
}

static inline
size_t xxx_bitset_count(const xxx_bitset_t *self) {
    if (xxx_bitset_isshort(self)) {
        return __builtin_popcountll(self->s.words[0])
            + __builtin_popcountll(self->s.words[1] & ~XXX_SHORT_BITSET_FLAG);
    }
    size_t count = 0;
    size_t nwords = self->l.cap >> 6;
    for (size_t i = 0; i < nwords; ++i) {
        count += __builtin_popcountll(self->l.words[i]);
    }
    return count;
}

static inline
bool xxx_bitset_empty(const xxx_bitset_t *self) {
    if (xxx_bitset_isshort(self)) {
        return (self->s.words[0] == 0) && ((self->s.words[1] & ~XXX_SHORT_BITSET_FLAG) == 0);
    }
    size_t nwords = self->l.cap >> 6;
    for (size_t i = 0; i < nwords; ++i) {
        if (self->l.words[i] != 0) {
            return false;
        }
    }
    return true;
}

static inline
int xxx_bitset_reserve(xxx_bitset_t *self, size_t n) {
    if (n > XXX_BITSET_CAPACITY_MAX) {
        return -1;
    }
    size_t new_cap = XXX_BITSET_ALIGN(n);
    if (xxx_bitset_isshort(self)) {
        if (n <= XXX_SHORT_BITSET_CAPACITY) {
            return 0;
        }
        return xxx_short_bitset_grow(self, new_cap);
    }
    if (n <= self->l.cap) {
        return 0;
    }
    return xxx_long_bitset_grow(self, new_cap);
}

static inline
bool xxx_bitset_test(const xxx_bitset_t *self, size_t pos) {
    if (pos >= XXX_BITSET_CAPACITY_MAX) {
        return false;
    }
    if (xxx_bitset_isshort(self)) {
        if (pos >= XXX_SHORT_BITSET_CAPACITY) {
            return false;
        }
        return self->s.words[pos >> 6] & (1ULL << (pos & 63));
    }
    if (pos >= self->l.cap) {
        return false;
    }
    return (self->l.words[pos >> 6] & (1ULL << (pos & 63))) != 0;
}

static inline
int xxx_bitset_set(xxx_bitset_t *self, size_t pos) {
    if (pos >= XXX_BITSET_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_bitset_isshort(self)) {
        if (pos < XXX_SHORT_BITSET_CAPACITY) {
            self->s.words[pos >> 6] |= 1ULL << (pos & 63);
            return 0;
        }
        size_t new_cap = XXX_BITSET_ALIGN(pos + 1);
        if (xxx_short_bitset_grow(self, new_cap) != 0) {
            return -1;
        }
        self->l.words[pos >> 6] |= 1ULL << (pos & 63);
        return 0;
    }
    if (pos >= self->l.cap) {
        size_t new_cap = self->l.cap * 2;
        if (new_cap <= pos) {
            new_cap = XXX_BITSET_ALIGN(pos + 1);
        }
        if (new_cap > XXX_BITSET_CAPACITY_MAX) {
            new_cap = XXX_BITSET_CAPACITY_MAX;
        }
        if (xxx_long_bitset_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    self->l.words[pos >> 6] |= 1ULL << (pos & 63);
    return 0;
}

static inline
int xxx_bitset_reset(xxx_bitset_t *self, size_t pos) {
    if (pos >= XXX_BITSET_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_bitset_isshort(self)) {
        if (pos < XXX_SHORT_BITSET_CAPACITY) {
            self->s.words[pos >> 6] &= ~(1ULL << (pos & 63));
        }
        return 0;
    }
    if (pos < self->l.cap) {
        self->l.words[pos >> 6] &= ~(1ULL << (pos & 63));
    }
    return 0;
}

static inline
int xxx_bitset_flip(xxx_bitset_t *self, size_t pos) {
    if (pos >= XXX_BITSET_CAPACITY_MAX) {
        return -1;
    }
    if (xxx_bitset_isshort(self)) {
        if (pos < XXX_SHORT_BITSET_CAPACITY) {
            self->s.words[pos >> 6] ^= 1ULL << (pos & 63);
            return 0;
        }
        size_t new_cap = XXX_BITSET_ALIGN(pos + 1);
        if (xxx_short_bitset_grow(self, new_cap) != 0) {
            return -1;
        }
        self->l.words[pos >> 6] ^= 1ULL << (pos & 63);
        return 0;
    }
    if (pos >= self->l.cap) {
        size_t new_cap = self->l.cap * 2;
        if (new_cap <= pos) {
            new_cap = XXX_BITSET_ALIGN(pos + 1);
        }
        if (new_cap > XXX_BITSET_CAPACITY_MAX) {
            new_cap = XXX_BITSET_CAPACITY_MAX;
        }
        if (xxx_long_bitset_grow(self, new_cap) != 0) {
            return -1;
        }
    }
    self->l.words[pos >> 6] ^= 1ULL << (pos & 63);
    return 0;
}

static inline
void xxx_bitset_clear(xxx_bitset_t *self) {
    if (xxx_bitset_isshort(self)) {
        self->s.words[0] = 0;
        self->s.words[1] = XXX_SHORT_BITSET_FLAG;
        return;
    }
    memset(self->l.words, 0, (self->l.cap >> 6) * sizeof(uint64_t));
}

#endif
