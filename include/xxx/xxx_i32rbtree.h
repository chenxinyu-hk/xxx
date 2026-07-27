#ifndef XXX_I32RBTREE_H
#define XXX_I32RBTREE_H

#include <stdbool.h>
#include <stddef.h>
// #include <string.h>

// #include "xxx_config.h"
#include "xxx_allocator.h"
// #include "xxx_assert.h"

#ifndef XXX_I32RBTREE_ALLOCATOR
#  define XXX_I32RBTREE_FREE    XXX_FREE
#  define XXX_I32RBTREE_MALLOC  XXX_MALLOC
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xxx_i32rbtree xxx_i32rbtree_t;

static inline int xxx_i32rbtree_init(xxx_i32rbtree_t *self);
static inline void xxx_i32rbtree_deinit(xxx_i32rbtree_t *self);
static inline int xxx_i32rbtree_copy(xxx_i32rbtree_t *dst, const xxx_i32rbtree_t *src);
static inline void xxx_i32rbtree_move(xxx_i32rbtree_t *dst, xxx_i32rbtree_t *src);
static inline size_t xxx_i32rbtree_size(const xxx_i32rbtree_t *self);
static inline bool xxx_i32rbtree_empty(const xxx_i32rbtree_t *self);
static inline bool xxx_i32rbtree_contains(const xxx_i32rbtree_t *self, int key);
static inline int xxx_i32rbtree_insert(xxx_i32rbtree_t *self, int key);
static inline void xxx_i32rbtree_erase(xxx_i32rbtree_t *self, int key);
static inline void xxx_i32rbtree_clear(xxx_i32rbtree_t *self);

typedef struct xxx_i32rbtree_node xxx_i32rbtree_node_t;
typedef xxx_i32rbtree_node_t *xxx_i32rbtree_iter_t;

#ifdef __cplusplus
}
#endif

enum { 
    XXX_I32RBTREE_BLACK, XXX_I32RBTREE_RED
};

struct xxx_i32rbtree_node {
    xxx_i32rbtree_node_t *parent;
    xxx_i32rbtree_node_t *left;
    xxx_i32rbtree_node_t *right;
    int color;
    int key;
};

struct xxx_i32rbtree {
    xxx_i32rbtree_node_t *root;
    xxx_i32rbtree_node_t *nil;
    size_t size;
};

static inline
xxx_i32rbtree_node_t *xxx_i32rbtree_create_node() {
    return (xxx_i32rbtree_node_t *)XXX_I32RBTREE_MALLOC(sizeof(xxx_i32rbtree_node_t));
}

static inline
void xxx_i32rbtree_destroy_nodes(xxx_i32rbtree_node_t *root, xxx_i32rbtree_node_t *nil) {
    xxx_i32rbtree_node_t *x = root;
    while (x != nil) {
        if (x->left != nil) {
            xxx_i32rbtree_node_t *left = x->left;
            x->left = left->right;
            left->right = x;
            x = left;
        } else {
            xxx_i32rbtree_node_t *next = x->right;
            XXX_I32RBTREE_FREE(x);
            x = next;
        }
    }
}

static inline
void xxx_i32rbtree_rotate_left(xxx_i32rbtree_t *self, xxx_i32rbtree_node_t *y) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = y->right;
    xxx_i32rbtree_node_t *g = y->parent;
    y->right = x->left;
    if (y->right != nil) {
        y->right->parent = y;
    }
    x->left = y;
    y->parent = x;
    if (g == nil) {
        self->root = x;
    } else {
        if (g->left == y) {
            g->left = x;
        } else {
            g->right = x;
        }
    }
    x->parent = g;
}

static inline
void xxx_i32rbtree_rotate_right(xxx_i32rbtree_t *self, xxx_i32rbtree_node_t *y) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = y->left;
    xxx_i32rbtree_node_t *g = y->parent;
    y->left = x->right;
    if (y->left != nil) {
        y->left->parent = y;
    }
    x->right = y;
    y->parent = x;
    if (g == nil) {
        self->root = x;
    } else {
        if (g->left == y) {
            g->left = x;
        } else {
            g->right = x;
        }
    }
    x->parent = g;
}

static inline
void xxx_i32rbtree_insert_fixup(xxx_i32rbtree_t *self, xxx_i32rbtree_node_t *z) {
    while (1) {
        xxx_i32rbtree_node_t *p = z->parent;
        if (p->color == XXX_I32RBTREE_BLACK) {
            break;
        }
        xxx_i32rbtree_node_t *g = p->parent;
        if (p == g->left) {
            xxx_i32rbtree_node_t *y = g->right;
            if (y->color == XXX_I32RBTREE_RED) {
                p->color = XXX_I32RBTREE_BLACK;
                y->color = XXX_I32RBTREE_BLACK;
                g->color = XXX_I32RBTREE_RED;
                z = g;
            } else {
                if (z == p->right) {
                    xxx_i32rbtree_rotate_left(self, p);
                    p = z;
                }
                xxx_i32rbtree_rotate_right(self, g);
                p->color = XXX_I32RBTREE_BLACK;
                g->color = XXX_I32RBTREE_RED;
                break;
            }
        } else {
            xxx_i32rbtree_node_t *y = g->left;
            if (y->color == XXX_I32RBTREE_RED) {
                p->color = XXX_I32RBTREE_BLACK;
                y->color = XXX_I32RBTREE_BLACK;
                g->color = XXX_I32RBTREE_RED;
                z = g;
            } else {
                if (z == p->left) {
                    xxx_i32rbtree_rotate_right(self, p);
                    p = z;
                }
                xxx_i32rbtree_rotate_left(self, g);
                p->color = XXX_I32RBTREE_BLACK;
                g->color = XXX_I32RBTREE_RED;
                break;
            }
        }
    }
    self->root->color = XXX_I32RBTREE_BLACK;
}

static inline
int xxx_i32rbtree_init(xxx_i32rbtree_t *self) {
    xxx_i32rbtree_node_t *nil = xxx_i32rbtree_create_node();
    if (nil == NULL) {
        return -1;
    }
    nil->color = XXX_I32RBTREE_BLACK;
    nil->parent = nil;
    nil->left = nil;
    nil->right = nil;
    self->root = nil;
    self->nil = nil;
    self->size = 0;
    return 0;
}

static inline
void xxx_i32rbtree_deinit(xxx_i32rbtree_t *self) {
    if (self->nil == NULL) {
        return;
    }
    xxx_i32rbtree_destroy_nodes(self->root, self->nil);
    XXX_I32RBTREE_FREE(self->nil);
    self->root = NULL;
    self->nil = NULL;
    self->size = 0;
}

static inline
int xxx_i32rbtree_copy(xxx_i32rbtree_t *dst, const xxx_i32rbtree_t *src) {
    if (dst == src) {
        return 0;
    }
    xxx_i32rbtree_node_t *src_curr = src->root;
    xxx_i32rbtree_node_t *dst_curr = dst->nil;
    while (src_curr != src->nil) {
        xxx_i32rbtree_node_t *new_node = xxx_i32rbtree_create_node();
        if (new_node == NULL) {
            xxx_i32rbtree_destroy_nodes(dst->nil->right, dst->nil);
            dst->nil->right = dst->nil;
            return -1;
        }
        new_node->key = src_curr->key;
        new_node->color = src_curr->color;
        new_node->parent = dst_curr;
        new_node->right = dst->nil;
        dst_curr->right = new_node;
        dst_curr = new_node;
        while (src_curr->left != src->nil) {
            src_curr = src_curr->left;
            xxx_i32rbtree_node_t *new_node = xxx_i32rbtree_create_node();
            if (new_node == NULL) {
                xxx_i32rbtree_destroy_nodes(dst->nil->right, dst->nil);
                dst->nil->right = dst->nil;
                return -1;
            }
            new_node->key = src_curr->key;
            new_node->color = src_curr->color;
            new_node->parent = dst_curr;
            new_node->right = dst->nil;
            dst_curr->left = new_node;
            dst_curr = new_node;
        }
        dst_curr->left = dst->nil;
        while ((src_curr != src->nil && src_curr->right == src->nil) || dst_curr->right != dst->nil) {
            dst_curr = dst_curr->parent;
            src_curr = src_curr->parent;
        }
        src_curr = src_curr->right;
    }
    xxx_i32rbtree_destroy_nodes(dst->root, dst->nil);
    dst->root = dst->nil->right;
    dst->nil->right = dst->nil;
    dst->size = src->size;
    return 0;
}

static inline
void xxx_i32rbtree_move(xxx_i32rbtree_t *dst, xxx_i32rbtree_t *src) {
    if (dst == src) {
        return;
    }
    xxx_i32rbtree_destroy_nodes(dst->root, dst->nil);
    XXX_I32RBTREE_FREE(dst->nil);
    *dst = *src;
    src->root = NULL;
    src->nil = NULL;
    src->size = 0;
}

static inline
size_t xxx_i32rbtree_size(const xxx_i32rbtree_t *self) {
    return self->size;
}

static inline
bool xxx_i32rbtree_empty(const xxx_i32rbtree_t *self) {
    return self->size == 0;
}

static inline
bool xxx_i32rbtree_contains(const xxx_i32rbtree_t *self, int key) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = self->root;
    while (x != nil) {
        if (key == x->key) {
            return true;
        }
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return false;
}

static inline
int xxx_i32rbtree_insert(xxx_i32rbtree_t *self, int key) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *p = nil;
    xxx_i32rbtree_node_t *x = self->root;
    while (x != nil) {
        if (key == x->key) {
            return 0;
        }
        p = x;
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    xxx_i32rbtree_node_t *z = xxx_i32rbtree_create_node();
    if (z == NULL) {
        return -1;
    }
    z->key = key;
    z->parent = p;
    z->left = nil;
    z->right = nil;
    if (p == nil) {
        z->color = XXX_I32RBTREE_BLACK;
        self->root = z;
        ++self->size;
        return 0;
    }
    z->color = XXX_I32RBTREE_RED;
    if (key < p->key) {
        p->left = z;
    } else {
        p->right = z;
    }
    xxx_i32rbtree_insert_fixup(self, z);
    ++self->size;
    return 0;
}

static inline
void xxx_i32rbtree_clear(xxx_i32rbtree_t *self) {
    xxx_i32rbtree_destroy_nodes(self->root, self->nil);
    self->root = self->nil;
    self->size = 0;
}

#endif

static inline
void xxx_i32rbtree_erase(xxx_i32rbtree_t *self, int key) {

}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_begin(xxx_i32rbtree_t *self) {
    xxx_i32rbtree_node_t *x = self->root;
    while (x->left != self->nil) {
        x = x->left;
    }
    return x;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_end(xxx_i32rbtree_t *self) {
    return self->nil;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_next(xxx_i32rbtree_t *self, xxx_i32rbtree_iter_t iter) {
    xxx_i32rbtree_node_t *nil = self->nil;
    if (iter == nil) {
        return nil;
    }
    if (iter->right != nil) {
        iter = iter->right;
        while (iter->left != nil) {
            iter = iter->left;
        }
        return iter;
    }
    while (iter == iter->parent->right) {
        iter = iter->parent;
    }
    return iter->parent;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_prev(xxx_i32rbtree_t *self, xxx_i32rbtree_iter_t iter) {
    xxx_i32rbtree_node_t *nil = self->nil;
    if (iter == nil) {
        return nil;
    }
    if (iter->left != nil) {
        iter = iter->left;
        while (iter->right != nil) {
            iter = iter->right;
        }
        return iter;
    }
    while (iter == iter->parent->left) {
        iter = iter->parent;
    }
    return iter->parent;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_find(xxx_i32rbtree_t *self, int key) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = self->root;
    while (x != nil) {
        if (key == x->key) {
            return x;
        }
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return nil;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_lower_bound(xxx_i32rbtree_t *self, int key) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = self->root;
    xxx_i32rbtree_node_t *y = nil;
    while (x != nil) {
        if (key == x->key) {
            return x;
        }
        if (key < x->key) {
            y = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return y;
}

static inline
xxx_i32rbtree_iter_t xxx_i32rbtree_upper_bound(xxx_i32rbtree_t *self, int key) {
    xxx_i32rbtree_node_t *nil = self->nil;
    xxx_i32rbtree_node_t *x = self->root;
    xxx_i32rbtree_node_t *y = nil;
    while (x != nil) {
        if (key < x->key) {
            y = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return y;
}
