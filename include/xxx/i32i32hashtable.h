#ifndef XXX_I32I32HASHTABLE_H
#define XXX_I32I32HASHTABLE_H

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "xxx/config.h"
#include "xxx/allocator.h"
#include "xxx/assert.h"
#include "xxx/hash.h"

#ifndef XXX_I32I32HASHTABLE_DEBUG
#  define XXX_I32I32HASHTABLE_DEBUG XXX_DEBUG
#endif

#ifndef XXX_I32I32HASHTABLE_ASSERT
#  define XXX_I32I32HASHTABLE_ASSERT XXX_ASSERT
#endif

#ifndef XXX_I32I32HASHTABLE_ALLOCATOR
#  define XXX_I32I32HASHTABLE_FREE    XXX_FREE
#  define XXX_I32I32HASHTABLE_MALLOC  XXX_MALLOC
#endif

#ifndef XXX_I32I32HASHTABLE_HASH
#  define XXX_I32I32HASHTABLE_HASH xxx_i32i32hashtable_hash
#endif

// #define XXX_i32i32hashtable_LOAD_FACTOR_MAX 0.75
#define XXX_I32I32HASHTABLE_BUCKET_COUNT_MAX ((size_t)INT_MAX)
#define XXX_I32I32HASHTABLE_SIZE_MAX \
    (XXX_I32I32HASHTABLE_BUCKET_COUNT_MAX - XXX_I32I32HASHTABLE_BUCKET_COUNT_MAX / 4)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32i32hashtable xxx_i32i32hashtable_t;
typedef size_t xxx_i32i32hashtable_iter_t;
    
static inline int xxx_i32i32hashtable_init(xxx_i32i32hashtable_t *self);
static inline void xxx_i32i32hashtable_deinit(xxx_i32i32hashtable_t *self);
static inline int xxx_i32i32hashtable_copy(xxx_i32i32hashtable_t *dst, const xxx_i32i32hashtable_t *src);
static inline void xxx_i32i32hashtable_move(xxx_i32i32hashtable_t *dst, xxx_i32i32hashtable_t *src);
static inline size_t xxx_i32i32hashtable_size(const xxx_i32i32hashtable_t *self);
static inline size_t xxx_i32i32hashtable_bucket_count(const xxx_i32i32hashtable_t *self);
static inline float xxx_i32i32hashtable_load_factor(const xxx_i32i32hashtable_t *self);
static inline bool xxx_i32i32hashtable_empty(const xxx_i32i32hashtable_t *self);
static inline int xxx_i32i32hashtable_reserve(xxx_i32i32hashtable_t *self, size_t n);
static inline int xxx_i32i32hashtable_insert(xxx_i32i32hashtable_t *self, int key, int value);
static inline void xxx_i32i32hashtable_erase(xxx_i32i32hashtable_t *self, int key);
static inline void xxx_i32i32hashtable_clear(xxx_i32i32hashtable_t *self);
static inline bool xxx_i32i32hashtable_contains(const xxx_i32i32hashtable_t *self, int key);
static inline xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_find(const xxx_i32i32hashtable_t *self, int key);
static inline xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_begin(const xxx_i32i32hashtable_t *self);
static inline xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_end(const xxx_i32i32hashtable_t *self);
static inline xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_next(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter);
static inline const int *xxx_i32i32hashtable_key(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter);
static inline int *xxx_i32i32hashtable_value(xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter);
static inline const int *xxx_i32i32hashtable_value_const(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter);

#ifdef __cplusplus
}
#endif

typedef struct xxx_i32i32hashtable_entry xxx_i32i32hashtable_entry_t;

// enum {
//     XXX_I32I32HASHTABLE_STATE_EMPTY, XXX_I32I32HASHTABLE_STATE_OCCUPIED, XXX_I32I32HASHTABLE_STATE_DELETED
// };

struct xxx_i32i32hashtable_entry {
    int key;
    int value;
};

struct xxx_i32i32hashtable {
    xxx_i32i32hashtable_entry_t *entries;
    uint8_t *ctrl;
    size_t size;
    size_t bucket_count;
};

static inline
bool xxx_i32i32hashtable_is_pow2(size_t n) {
    return (n & (n - 1)) == 0;
}

static inline
size_t xxx_i32i32hashtable_ceil_pow2(size_t n) {
    if (xxx_i32i32hashtable_is_pow2(n)) {
        return n;
    }
    return (size_t)1 << (8 * sizeof(size_t) - __builtin_clzl(n));
}

static inline
uint32_t xxx_i32i32hashtable_hash(uint32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6bU;
    x ^= x >> 13;
    x *= 0xc2b2ae35U;
    x ^= x >> 16;
    return x;
}

static inline
int xxx_i32i32hashtable_rehash(xxx_i32i32hashtable_t *self, size_t new_bucket_count) {
    size_t entries_bytes = new_bucket_count * sizeof(xxx_i32i32hashtable_entry_t);
    size_t ctrl_bytes = new_bucket_count;
    void *new_buf = (void *)XXX_I32I32HASHTABLE_MALLOC(entries_bytes + ctrl_bytes);
    if (new_buf == NULL) {
        return -1;
    }
    xxx_i32i32hashtable_entry_t *new_entries = (xxx_i32i32hashtable_entry_t *)new_buf;
    uint8_t *new_ctrl = (uint8_t *)new_buf + entries_bytes;
    memset(new_ctrl, 0, ctrl_bytes);
    if (self->size > 0) {
        size_t mask = new_bucket_count - 1;
        size_t remain = self->size;
        for (size_t i = 0; i < self->bucket_count && remain > 0; ++i) {
            if (self->ctrl[i] & 1) {
                int key = self->entries[i].key;
                uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
                size_t pos = hash & mask;
                while (new_ctrl[pos] & 1) {
                    pos = (pos + 1) & mask;
                }
                new_entries[pos].key = key;
                new_entries[pos].value = self->entries[i].value;
                new_ctrl[pos] = (hash >> 24) | 1;
                --remain;
            }
        }
    }
    XXX_I32I32HASHTABLE_FREE(self->entries);
    self->entries = new_entries;
    self->ctrl = new_ctrl;
    self->bucket_count = new_bucket_count;
    return 0;
}

static inline
int xxx_i32i32hashtable_init(xxx_i32i32hashtable_t *self) {
    self->entries = NULL;
    self->ctrl = NULL;
    self->size = 0;
    self->bucket_count = 0;
    return 0;
}

static inline
void xxx_i32i32hashtable_deinit(xxx_i32i32hashtable_t *self) {
    XXX_I32I32HASHTABLE_FREE(self->entries);
    self->entries = NULL;
    self->ctrl = NULL;
    self->size = 0;
    self->bucket_count = 0;
}

static inline
int xxx_i32i32hashtable_copy(xxx_i32i32hashtable_t *dst, const xxx_i32i32hashtable_t *src) {
    if (dst == src) {
        return 0;
    }
    size_t size = src->size;
    if (size <= (dst->bucket_count - (dst->bucket_count / 4))) {
        xxx_i32i32hashtable_clear(dst);
    } else {
        size_t dst_old_size = dst->size;
        dst->size = 0;
        size_t new_bucket_count = xxx_i32i32hashtable_ceil_pow2((size * 4 + 2) / 3);
        if (new_bucket_count < 16) {
            new_bucket_count = 16;
        }
        if (xxx_i32i32hashtable_rehash(dst, new_bucket_count) != 0) {
            dst->size = dst_old_size;
            return -1;
        }
    }
    size_t mask = dst->bucket_count - 1;
    size_t remain = size;
    for (size_t i = 0; i < src->bucket_count && remain > 0; ++i) {
        if (src->ctrl[i] & 1) {
            int key = src->entries[i].key;
            uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
            size_t pos = hash & mask;
            while (dst->ctrl[pos] & 1) {
                pos = (pos + 1) & mask;
            }
            dst->entries[pos].key = key;
            dst->entries[pos].value = src->entries[i].value;
            dst->ctrl[pos] = (hash >> 24) | 1;
            --remain;
        }
    }
    dst->size = size;
    return 0;
}

static inline
void xxx_i32i32hashtable_move(xxx_i32i32hashtable_t *dst, xxx_i32i32hashtable_t *src) {
    if (dst == src) {
        return;
    }
    XXX_I32I32HASHTABLE_FREE(dst->entries);
    *dst = *src;
    src->entries = NULL;
    src->ctrl = NULL;
    src->size = 0;
    src->bucket_count = 0;
}

static inline
size_t xxx_i32i32hashtable_size(const xxx_i32i32hashtable_t *self) {
    return self->size;
}

static inline
size_t xxx_i32i32hashtable_bucket_count(const xxx_i32i32hashtable_t *self) {
    return self->bucket_count;
}

static inline
float xxx_i32i32hashtable_load_factor(const xxx_i32i32hashtable_t *self) {
    if (self->bucket_count == 0) {
        return 0.0f;
    }
    return (float)self->size / self->bucket_count;
}

static inline
bool xxx_i32i32hashtable_empty(const xxx_i32i32hashtable_t *self) {
    return self->size == 0;
}

static inline
int xxx_i32i32hashtable_reserve(xxx_i32i32hashtable_t *self, size_t n) {
    if (n > XXX_I32I32HASHTABLE_SIZE_MAX) {
        return -1;
    }
    size_t new_bucket_count = xxx_i32i32hashtable_ceil_pow2((n * 4 + 2) / 3);
    if (new_bucket_count < 16) {
        new_bucket_count = 16;
    }
    if (new_bucket_count <= self->bucket_count) {
        return 0;
    }
    return xxx_i32i32hashtable_rehash(self, new_bucket_count);
}

static inline
bool xxx_i32i32hashtable_contains(const xxx_i32i32hashtable_t *self, int key) {
    if (self->size == 0) {
        return false;
    }
    uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
    uint8_t h2 = (uint8_t)(hash >> 25);
    size_t mask = self->bucket_count - 1;
    size_t pos = hash & mask;
    while (1) {
        uint8_t ctrl = self->ctrl[pos];
        if (ctrl == 0) {
            return false;
        }
        if (ctrl & 1) {
            if ((ctrl >> 1) == h2 && self->entries[pos].key == key) {
                return true;
            }
        }
        pos = (pos + 1) & mask;
    }
}

static inline
int xxx_i32i32hashtable_insert(xxx_i32i32hashtable_t *self, int key, int value) {
    if (self->size + 1 > (self->bucket_count - (self->bucket_count / 4))) {
        if (self->bucket_count == XXX_I32I32HASHTABLE_BUCKET_COUNT_MAX) {
            return -1;
        }
        size_t new_bucket_count = self->bucket_count > 0 ? self->bucket_count * 2 : 16;
        if (xxx_i32i32hashtable_rehash(self, new_bucket_count) != 0) {
            return -1;
        }
    }
    uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
    uint8_t h2 = (uint8_t)(hash >> 25);
    uint8_t h2_ctrl = (uint8_t)(hash >> 24 | 1);
    size_t mask = self->bucket_count - 1;
    size_t pos = hash & mask;
    size_t insert_pos = self->bucket_count;
    for (size_t i = 0; i < self->bucket_count; ++i) {
        uint8_t ctrl = self->ctrl[pos];
        if (ctrl == 0) {
            if (insert_pos == self->bucket_count) {
                insert_pos = pos;
            }
            self->entries[insert_pos].key = key;
            self->entries[insert_pos].value = value;
            self->ctrl[insert_pos] = h2_ctrl;
            ++self->size;
            return 0;
        }
        if (ctrl & 1) {
            if ((ctrl >> 1) == h2 && self->entries[pos].key == key) {
                self->entries[pos].value = value;
                return 0;
            }
        } else if (insert_pos == self->bucket_count) {
            insert_pos = pos;
        }
        pos = (pos + 1) & mask;
    }
    self->entries[insert_pos].key = key;
    self->entries[insert_pos].value = value;
    self->ctrl[insert_pos] = h2_ctrl;
    ++self->size;
    return 0;
}

static inline
void xxx_i32i32hashtable_erase(xxx_i32i32hashtable_t *self, int key) {
    if (self->size == 0) {
        return;
    }
    uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
    uint8_t h2 = (uint8_t)(hash >> 25);
    size_t mask = self->bucket_count - 1;
    size_t pos = hash & mask;
    while (1) {
        uint8_t ctrl = self->ctrl[pos];
        if (ctrl == 0) {
            return;
        }
        if (ctrl & 1) {
            if ((ctrl >> 1) == h2 && self->entries[pos].key == key) {
                self->ctrl[pos] = 2;
                --self->size;
                return;
            }
        }
        pos = (pos + 1) & mask;
    }
}

static inline
void xxx_i32i32hashtable_clear(xxx_i32i32hashtable_t *self) {
    if (self->bucket_count == 0) {
        return;
    }
    memset(self->ctrl, 0, self->bucket_count);
    self->size = 0;
}

static inline
xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_begin(const xxx_i32i32hashtable_t *self) {
    if (self->size == 0) {
        return self->bucket_count;
    }
    for (size_t i = 0; i < self->bucket_count; ++i) {
        if (self->ctrl[i] & 1) {
            return i;
        }
    }
    return self->bucket_count;
}

static inline
xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_end(const xxx_i32i32hashtable_t *self) {
    return self->bucket_count;
}

static inline
xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_next(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter) {
    for (size_t i = iter + 1; i < self->bucket_count; ++i) {
        if (self->ctrl[i] & 1) {
            return i;
        }
    }
    return self->bucket_count;
}

static inline
const int *xxx_i32i32hashtable_key(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter) {
    return &self->entries[iter].key;
}

static inline
int *xxx_i32i32hashtable_value(xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter) {
    return &self->entries[iter].value;
}

static inline
const int *xxx_i32i32hashtable_value_const(const xxx_i32i32hashtable_t *self, xxx_i32i32hashtable_iter_t iter) {
    return &self->entries[iter].value;
}

static inline
xxx_i32i32hashtable_iter_t xxx_i32i32hashtable_find(const xxx_i32i32hashtable_t *self, int key) {
    if (self->size == 0) {
        return self->bucket_count;
    }
    uint32_t hash = XXX_I32I32HASHTABLE_HASH(key);
    uint8_t h2 = (uint8_t)(hash >> 25);
    size_t mask = self->bucket_count - 1;
    size_t pos = hash & mask;
    while (1) {
        uint8_t ctrl = self->ctrl[pos];
        if (ctrl == 0) {
            return self->bucket_count;
        }
        if (ctrl & 1) {
            if ((ctrl >> 1) == h2 && self->entries[pos].key == key) {
                return pos;
            }
        }
        pos = (pos + 1) & mask;
    }
}

#endif
