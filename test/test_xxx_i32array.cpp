#include "test.h"
#include "xxx/xxx_i32array.h"

TEST(xxx_i32array_reserve) {
    xxx_i32array_t arr;
    xxx_i32array_init(&arr);
    DEFER(xxx_i32array_deinit(&arr));

    ASSERT(xxx_i32array_empty(&arr));
    ASSERT(xxx_i32array_length(&arr) == 0);
    ASSERT(xxx_i32array_capacity(&arr) == 0);

    ASSERT(xxx_i32array_reserve(&arr, SIZE_MAX) != 0, "out of range");

    ASSERT(xxx_i32array_reserve(&arr, 10) == 0);
    ASSERT(xxx_i32array_length(&arr) == 0);
    ASSERT(xxx_i32array_capacity(&arr) == 10);

    ASSERT(xxx_i32array_reserve(&arr, 100) == 0);
    ASSERT(xxx_i32array_length(&arr) == 0);
    ASSERT(xxx_i32array_capacity(&arr) == 100);

    ASSERT(xxx_i32array_reserve(&arr, 10) == 0);
    ASSERT(xxx_i32array_length(&arr) == 0);
    ASSERT(xxx_i32array_capacity(&arr) == 100);
}

TEST(xxx_i32array_assign) {
    xxx_i32array_t arr;
    xxx_i32array_init(&arr);
    DEFER(xxx_i32array_deinit(&arr));

    int a[100];
    memset(a, 0, sizeof(a));

    ASSERT(xxx_i32array_assign(&arr, a, 10) == 0);
    ASSERT(xxx_i32array_length(&arr) == 10);

    ASSERT(xxx_i32array_assign(&arr, a, 100) == 0);
    ASSERT(xxx_i32array_length(&arr) == 100);

    for (size_t i = 0; i < 100; ++i) {
        ASSERT(*xxx_i32array_at(&arr, i) == 0);
    }

    size_t cap_before_clear = xxx_i32array_capacity(&arr);
    xxx_i32array_clear(&arr);
    ASSERT(xxx_i32array_length(&arr) == 0);
    ASSERT(xxx_i32array_capacity(&arr) == cap_before_clear);
}

TEST(xxx_i32array_push_back) {
    xxx_i32array_t arr;
    xxx_i32array_init(&arr);
    DEFER(xxx_i32array_deinit(&arr));

    ASSERT(xxx_i32array_push_back(&arr, 0) == 0);
    ASSERT(xxx_i32array_length(&arr) == 1);
}

TEST(xxx_i32array_copy) {
    xxx_i32array_t src;
    xxx_i32array_init(&src);
    DEFER(xxx_i32array_deinit(&src));

    xxx_i32array_t dst;
    xxx_i32array_init(&dst);
    DEFER(xxx_i32array_deinit(&dst));

    int a[100];
    memset(a, 0, sizeof(a));

    xxx_i32array_assign(&src, a, 88);

    ASSERT(xxx_i32array_copy(&dst, &src) == 0);
    ASSERT(xxx_i32array_length(&dst) == 88);
    for (size_t i = 0; i < 88; ++i) {
        ASSERT(*xxx_i32array_at(&dst, i) == 0);
    }
}
