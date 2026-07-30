#include "test.h"
#include "xxx/xxx_i32sort.h"
#include "xxx/xxx_i32array.h"

TEST(xxx_i32quicksort_asc) {
    xxx_i32array_t arr;
    ASSERT(xxx_i32array_init(&arr) == 0);
    DEFER(xxx_i32array_deinit(&arr));

    for (int i = 0; i <= 10; ++i) {
        int n = 1 << i;
        for (int i = 0; i < n; ++i) {
            ASSERT(xxx_i32array_push_back(&arr, rand()) == 0);
        }

        xxx_i32quicksort_asc(xxx_i32array_data(&arr), xxx_i32array_length(&arr));
        for (int i = 1; i < n; ++i) {
            ASSERT(*xxx_i32array_at(&arr, i) >= *xxx_i32array_at(&arr, i - 1));
        }

        xxx_i32array_clear(&arr);
    }
}

TEST(xxx_i32quicksort_desc) {
    return;
    xxx_i32array_t arr;
    ASSERT(xxx_i32array_init(&arr) == 0);
    DEFER(xxx_i32array_deinit(&arr));

    for (int i = 0; i <= 10; ++i) {
        int n = 1 << i;
        for (int i = 0; i < n; ++i) {
            ASSERT(xxx_i32array_push_back(&arr, rand()) == 0);
        }

        xxx_i32quicksort_desc(xxx_i32array_data(&arr), xxx_i32array_length(&arr));
        for (int i = 1; i < n; ++i) {
            ASSERT(*xxx_i32array_at(&arr, i) <= *xxx_i32array_at(&arr, i - 1));
        }

        xxx_i32array_clear(&arr);
    }
}

TEST(xxx_i32heapsort_asc) {
    xxx_i32array_t arr;
    ASSERT(xxx_i32array_init(&arr) == 0);
    DEFER(xxx_i32array_deinit(&arr));

    for (int i = 0; i <= 10; ++i) {
        int n = 1 << i;
        for (int i = 0; i < n; ++i) {
            ASSERT(xxx_i32array_push_back(&arr, rand()) == 0);
        }

        xxx_i32heapsort_asc(xxx_i32array_data(&arr), xxx_i32array_length(&arr));
        for (int i = 1; i < n; ++i) {
            ASSERT(*xxx_i32array_at(&arr, i) >= *xxx_i32array_at(&arr, i - 1));
        }

        xxx_i32array_clear(&arr);
    }
}

TEST(xxx_i32heapsort_desc) {
    xxx_i32array_t arr;
    ASSERT(xxx_i32array_init(&arr) == 0);
    DEFER(xxx_i32array_deinit(&arr));

    for (int i = 0; i <= 10; ++i) {
        int n = 1 << i;
        for (int i = 0; i < n; ++i) {
            ASSERT(xxx_i32array_push_back(&arr, rand()) == 0);
        }

        xxx_i32heapsort_desc(xxx_i32array_data(&arr), xxx_i32array_length(&arr));
        for (int i = 1; i < n; ++i) {
            ASSERT(*xxx_i32array_at(&arr, i) <= *xxx_i32array_at(&arr, i - 1));
        }

        xxx_i32array_clear(&arr);
    }
}
