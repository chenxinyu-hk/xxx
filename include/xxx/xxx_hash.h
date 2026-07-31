#ifndef XXX_HASH_H
#define XXX_HASH_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint32_t xxx_fnv1a32(const void *data, size_t len);
static inline uint64_t xxx_fnv1a64(const void *data, size_t len);

#ifdef __cplusplus
}
#endif

static inline
uint32_t xxx_fnv1a32(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    uint32_t h = 2166136261U;
    for (size_t i = 0; i < len; ++i) {
        h ^= p[i];
        h *= 16777619U;
    }
    return h;
}

static inline
uint64_t xxx_fnv1a64(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    uint64_t h = 14695981039346656037ULL;
    for (size_t i = 0; i < len; ++i) {
        h ^= p[i];
        h *= 1099511628211ULL;
    }
    return h;
}

// static inline
// uint64_t xxx_djb2(const char *data, size_t len) {
//     const uint8_t *p = (const uint8_t *)data;
//     uint64_t hash = 5381;
//     for (size_t i = 0; i < len; ++i) {
//         hash = ((hash << 5) + hash) + p[i];
//     }
//     return hash;
// }

// static inline
// uint64_t xxx_djb2_64(const char *data, size_t len) {
//     const uint8_t *p = (const uint8_t *)data;
//     uint64_t hash = 14695981039346656037ULL; 
//     for (size_t i = 0; i < len; ++i) {
//         hash = ((hash << 5) + hash) + p[i];
//     }
//     return hash;
// }

// static inline
// uint64_t sdbm(const char *data, size_t len) {
//     const uint8_t *p = (const uint8_t *)data;
//     uint64_t hash = 0;
//     for (size_t i = 0; i < len; ++i) {
//         hash = hash * 65599 + p[i];
//     }
//     return hash;
// }


#endif
