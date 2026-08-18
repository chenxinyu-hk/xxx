#include "test.h"
#include "xxx/string.h"

TEST(xxx_string_reserve) {
    xxx_string_t str;
    ASSERT(xxx_string_init(&str) == 0);
    DEFER(xxx_string_deinit(&str));

    ASSERT(xxx_string_empty(&str));
    ASSERT(xxx_string_length(&str) == 0);
    ASSERT(xxx_string_capacity(&str) == XXX_SHORT_STRING_CAPACITY);

    ASSERT(xxx_string_reserve(&str, XXX_STRING_CAPACITY_MAX + 1) != 0, "out of range");

    ASSERT(xxx_string_reserve(&str, 10) == 0);
    ASSERT(xxx_string_capacity(&str) == XXX_SHORT_STRING_CAPACITY);
    ASSERT(xxx_string_length(&str) == 0);

    ASSERT(xxx_string_reserve(&str, 100) == 0);
    ASSERT(xxx_string_capacity(&str) == 100);
    ASSERT(xxx_string_length(&str) == 0);

    ASSERT(xxx_string_reserve(&str, 10) == 0);
    ASSERT(xxx_string_capacity(&str) == 100);
    ASSERT(xxx_string_length(&str) == 0);

    ASSERT(xxx_string_empty(&str));
}

TEST(xxx_string_assign) {
    xxx_string_t str;
    ASSERT(xxx_string_init(&str) == 0);
    DEFER(xxx_string_deinit(&str));

    char s[100];
    memset(s, 'a', 100);

    ASSERT(xxx_string_assign(&str, s, 8) == 0);
    ASSERT(strncmp(xxx_string_c_str(&str), s, 8) == 0);
    ASSERT(xxx_string_length(&str) == 8);
    ASSERT(xxx_string_capacity(&str) == XXX_SHORT_STRING_CAPACITY);

    ASSERT(xxx_string_assign(&str, s, 88) == 0);
    ASSERT(strncmp(xxx_string_c_str(&str), s, 88) == 0);
    ASSERT(xxx_string_length(&str) == 88);
    ASSERT(xxx_string_capacity(&str) != XXX_SHORT_STRING_CAPACITY);

    size_t cap_before_clear = xxx_string_capacity(&str);
    xxx_string_clear(&str);
    ASSERT(xxx_string_length(&str) == 0);
    ASSERT(xxx_string_capacity(&str) == cap_before_clear);
    ASSERT(xxx_string_empty(&str));
}

TEST(xxx_string_push_back) {
    xxx_string_t str;
    ASSERT(xxx_string_init(&str) == 0);
    DEFER(xxx_string_deinit(&str));

    for (size_t i = 0; i < XXX_SHORT_STRING_CAPACITY; ++i) {
        ASSERT(xxx_string_push_back(&str, 'a') == 0);
        ASSERT(xxx_string_length(&str) == i + 1);
        ASSERT(xxx_string_capacity(&str) == XXX_SHORT_STRING_CAPACITY);
    }

    ASSERT(xxx_string_push_back(&str, 'a') == 0);
    ASSERT(xxx_string_capacity(&str) != XXX_SHORT_STRING_CAPACITY);

    size_t cap_before_pop_back = xxx_string_capacity(&str);
    xxx_string_pop_back(&str);
    ASSERT(xxx_string_length(&str) == XXX_SHORT_STRING_CAPACITY);
    ASSERT(xxx_string_capacity(&str) == cap_before_pop_back);
}

TEST(xxx_string_append) {
    xxx_string_t str;
    ASSERT(xxx_string_init(&str) == 0);
    DEFER(xxx_string_deinit(&str));

    char s[100];
    memset(s, 'a', 100);

    ASSERT(xxx_string_append(&str, s, 8) == 0);
    ASSERT(strncmp(xxx_string_c_str(&str), s, 8) == 0);
    ASSERT(xxx_string_length(&str) == 8);
    ASSERT(xxx_string_capacity(&str) == XXX_SHORT_STRING_CAPACITY);

    ASSERT(xxx_string_append(&str, s, 80) == 0);
    ASSERT(strncmp(xxx_string_c_str(&str), s, 88) == 0);
    ASSERT(xxx_string_length(&str) == 88);
    ASSERT(xxx_string_capacity(&str) != XXX_SHORT_STRING_CAPACITY);
}

TEST(xxx_string_copy) {
    xxx_string_t src;
    xxx_string_init(&src);
    DEFER(xxx_string_deinit(&src));

    xxx_string_t dst;
    xxx_string_init(&dst);
    DEFER(xxx_string_deinit(&dst));

    char s[100];
    memset(s, 'a', 100);

    ASSERT(xxx_string_assign(&src, s, 8) == 0);
    ASSERT(xxx_string_copy(&dst, &src) == 0);
    ASSERT(strncmp(xxx_string_c_str(&dst), xxx_string_c_str(&src), 8) == 0);
    ASSERT(xxx_string_length(&dst) == 8);
    ASSERT(xxx_string_capacity(&dst) == XXX_SHORT_STRING_CAPACITY);
    ASSERT(xxx_string_length(&src) == 8);
    ASSERT(xxx_string_capacity(&src) == XXX_SHORT_STRING_CAPACITY);

    ASSERT(xxx_string_assign(&src, s, 88) == 0);
    ASSERT(xxx_string_copy(&dst, &src) == 0);
    ASSERT(strncmp(xxx_string_c_str(&dst), xxx_string_c_str(&src), 88) == 0);
    ASSERT(xxx_string_length(&dst) == 88);
    ASSERT(xxx_string_capacity(&dst) != XXX_SHORT_STRING_CAPACITY);
    ASSERT(xxx_string_length(&src) == 88);
    ASSERT(xxx_string_capacity(&src) != XXX_SHORT_STRING_CAPACITY);
}

