#include "test.h"
#include "xxx/xxx_bitset.h"

TEST(xxx_bitset_reserve) {
    xxx_bitset_t bitset;
    xxx_bitset_init(&bitset);
    DEFER(xxx_bitset_deinit(&bitset));

    ASSERT(xxx_bitset_reserve(&bitset, SIZE_MAX) != 0, "out of memory");

    ASSERT(xxx_bitset_empty(&bitset));
    ASSERT(xxx_bitset_count(&bitset) == 0);

    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(xxx_bitset_reserve(&bitset, 10) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(xxx_bitset_reserve(&bitset, 999) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 1024,
        "Actual capacity=%d", xxx_bitset_capacity(&bitset));

    ASSERT(xxx_bitset_reserve(&bitset, 99) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_reserve(&bitset, 9) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_empty(&bitset));
    ASSERT(xxx_bitset_count(&bitset) == 0);
}

TEST(xxx_bitset_set) {
    xxx_bitset_t bitset;
    xxx_bitset_init(&bitset);
    DEFER(xxx_bitset_deinit(&bitset));

    ASSERT(!xxx_bitset_test(&bitset, SIZE_MAX));

    ASSERT(!xxx_bitset_test(&bitset, 9));
    ASSERT(xxx_bitset_set(&bitset, 9) == 0);
    ASSERT(xxx_bitset_test(&bitset, 9));
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(!xxx_bitset_test(&bitset, 99));
    ASSERT(xxx_bitset_set(&bitset, 99) == 0);
    ASSERT(xxx_bitset_test(&bitset, 99));
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(!xxx_bitset_empty(&bitset));
    ASSERT(xxx_bitset_count(&bitset) == 2);

    ASSERT(!xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);
    ASSERT(xxx_bitset_set(&bitset, 999) == 0);
    ASSERT(xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_test(&bitset, 99));
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(!xxx_bitset_test(&bitset, 1023));
    ASSERT(xxx_bitset_set(&bitset, 1023) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_set(&bitset, 1024) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 2048);

    ASSERT(!xxx_bitset_test(&bitset, 1));
    ASSERT(!xxx_bitset_test(&bitset, 111));
    ASSERT(!xxx_bitset_test(&bitset, 11111));

    ASSERT(!xxx_bitset_empty(&bitset));
    ASSERT(xxx_bitset_count(&bitset) == 5);
}

TEST(xxx_bitset_reset) {
    xxx_bitset_t bitset;
    xxx_bitset_init(&bitset);
    DEFER(xxx_bitset_deinit(&bitset));

    ASSERT(xxx_bitset_reset(&bitset, SIZE_MAX) != 0, "out of range");

    ASSERT(xxx_bitset_set(&bitset, 9) == 0);
    ASSERT(xxx_bitset_set(&bitset, 10) == 0);
    ASSERT(xxx_bitset_test(&bitset, 9));
    ASSERT(xxx_bitset_test(&bitset, 10));

    ASSERT(xxx_bitset_reset(&bitset, 9) == 0);
    ASSERT(!xxx_bitset_test(&bitset, 9));
    ASSERT(xxx_bitset_test(&bitset, 10));

    ASSERT(xxx_bitset_reset(&bitset, 1) == 0);

    ASSERT(xxx_bitset_reset(&bitset, 999) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(xxx_bitset_set(&bitset, 999) == 0);
    ASSERT(xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_test(&bitset, 10));
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_reset(&bitset, 999) == 0);
    ASSERT(!xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_reset(&bitset, 9999) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_reset(&bitset, 10) == 0);
    ASSERT(xxx_bitset_empty(&bitset));
    ASSERT(xxx_bitset_count(&bitset) == 0);
}

TEST(xxx_bitset_flip) {
    xxx_bitset_t bitset;
    xxx_bitset_init(&bitset);
    DEFER(xxx_bitset_deinit(&bitset));

    ASSERT(xxx_bitset_flip(&bitset, SIZE_MAX) != 0, "out of range");
    ASSERT(xxx_bitset_capacity(&bitset) == XXX_SHORT_BITSET_CAPACITY);

    ASSERT(!xxx_bitset_test(&bitset, 9));
    ASSERT(!xxx_bitset_test(&bitset, 10));
    ASSERT(xxx_bitset_flip(&bitset, 9) == 0);
    ASSERT(xxx_bitset_flip(&bitset, 10) == 0);
    ASSERT(xxx_bitset_test(&bitset, 9));
    ASSERT(xxx_bitset_test(&bitset, 10));

    ASSERT(xxx_bitset_flip(&bitset, 9) == 0);
    ASSERT(!xxx_bitset_test(&bitset, 9));

    ASSERT(xxx_bitset_flip(&bitset, 999) == 0);
    ASSERT(xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_test(&bitset, 10));
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_flip(&bitset, 999) == 0);
    ASSERT(!xxx_bitset_test(&bitset, 999));
    ASSERT(xxx_bitset_capacity(&bitset) == 1024);

    ASSERT(xxx_bitset_flip(&bitset, 1024) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) == 2048);

    ASSERT(xxx_bitset_count(&bitset) == 2);

    xxx_bitset_clear(&bitset);
    ASSERT(xxx_bitset_count(&bitset) == 0);
    ASSERT(xxx_bitset_capacity(&bitset) > XXX_SHORT_BITSET_CAPACITY);
    ASSERT(xxx_bitset_capacity(&bitset) == 2048);
}
