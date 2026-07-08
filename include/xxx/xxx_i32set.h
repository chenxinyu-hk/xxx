#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdbool.h>

#ifndef XXX_I32SET_HASH
#  define XXX_I32SET_HASH 
#endif

enum {
    XXX_I32SET_STATE_EMPTY, XXX_I32SET_STATE_OCCUPIED, XXX_I32SET_STATE_DELETED
};

typedef struct xxx_i32set xxx_i32set_t;





typedef struct xxx_i32set_entry xxx_i32set_entry_t;

struct xxx_i32set_entry {
    int key;
    int state;
};

struct xxx_i32set {
    xxx_i32set_entry_t *entries;
    size_t size;
    size_t capacity;
};

int xxx_i32set_init(xxx_i32set_t *self) {
    self->entries = NULL;
    self->size = 0;
    self->capacity = 0;
}

void xxx_i32set_deinit(xxx_i32set_t *self) {
    XXX_I32SET_FREE(self->entries);
    self->entries = NULL;
}

int xxx_i32set_copy(xxx_i32set_t *dst, const xxx_i32set_t *src);

void xxx_i32set_move(xxx_i32set_t *dst, xxx_i32set_t *src);

size_t xxx_i32set_size(const xxx_i32set_t *self) {
    return self->size;
}

bool xxx_i32set_empty(const xxx_i32set_t *self) {
    return self->size == 0;
}

int xxx_i32set_reserve(xxx_i32set_t *self, size_t n);

bool xxx_i32set_find(const xxx_i32set_t *self, int key) {
    uint64_t hash = XXX_I32SET_HASH(key);
    size_t mask = self->capacity - 1;
    size_t pos = hash & mask;
    while (1) {
        const xxx_i32set_entry_t *entry = &self->entries[pos];
        if (entry->state == 0) {
            return false;
        }
        if (entry->state == 1 && entry->key == key) {
            return true;
        }
        pos = (pos + 1) & mask;
    }
}

int xxx_i32set_rehash(xxx_i32set_t *self) {
    
}

int xxx_i32set_insert(xxx_i32set_t *self, int key) {
    if (self->size + 1 > (self->capacity - (self->capacity / 4))) {
        if (xxx_i32set_rehash(self) != 0) {
            return -1;
        }
    }
    uint64_t hash = XXX_I32SET_HASH(key);
    size_t mask = self->capacity - 1;
    size_t pos = hash & mask;
    size_t first_deleted_pos = self->capacity;
    while (1) {
        xxx_i32set_entry_t *entry = &self->entries[pos];
        if (entry->state == XXX_I32SET_STATE_EMPTY) {
            if (first_deleted_pos != self->capacity) {
                entry = &self->entries[first_deleted_pos];
            }
            entry->key = key;
            entry->state = XXX_I32SET_STATE_OCCUPIED;
            ++self->size;
            return 0;
        }
        if (entry->state == XXX_I32SET_STATE_OCCUPIED) {
            if (entry->key == key) {
                return 0;
            }
        } else if (first_deleted_pos == self->capacity) {
            first_deleted_pos = pos;
        }
        pos = (pos + 1) & mask;
    }
}

void xxx_i32set_remove(xxx_i32set_t *self, int key) {
    uint64_t hash = XXX_I32SET_HASH(key);
    size_t mask = self->capacity - 1;
    size_t pos = hash & mask;
    while (1) {
        xxx_i32set_entry_t *entry = &self->entries[pos];
        if (entry->state == 0) {
            return;
        }
        if (entry->state == 1 && entry->key == key) {
            entry->state = 2;
            --self->size;
            return;
        }
        pos = (pos + 1) & mask;
    }
}

void xxx_i32set_clear(xxx_i32set_t *self) {
    // if (self->size == 0) {
    //     return;
    // }
    // memset(self->entries, 0, self->capacity * sizeof(xxx_i32set_entry_t));
    // self->size = 0;
}
