#include "test.h"
#include "xxx/xxx_i32queue.h"

TEST(xxx_i32queue_reserve) {
    xxx_i32queue_t que;
    ASSERT(xxx_i32queue_init(&que) == 0);
    DEFER(xxx_i32queue_deinit(&que));

    ASSERT(xxx_i32queue_empty(&que));
    ASSERT(xxx_i32queue_size(&que) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 0);

    ASSERT(xxx_i32queue_reserve(&que, XXX_I32DEQUE_CAPACITY_MAX + 1) != 0);

    ASSERT(xxx_i32queue_reserve(&que, 0) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 0);

    ASSERT(xxx_i32queue_reserve(&que, 1) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 16);

    ASSERT(xxx_i32queue_reserve(&que, 15) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 16);

    ASSERT(xxx_i32queue_reserve(&que, 16) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 16);
    ASSERT(xxx_i32queue_size(&que) == 0);

    ASSERT(xxx_i32queue_reserve(&que, 35) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 64);

    ASSERT(xxx_i32queue_reserve(&que, 16) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 64);

    ASSERT(xxx_i32queue_reserve(&que, XXX_I32DEQUE_CAPACITY_MAX) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == XXX_I32DEQUE_CAPACITY_MAX);
}

TEST(xxx_i32queue_push) {
    xxx_i32queue_t que;
    ASSERT(xxx_i32queue_init(&que) == 0);
    DEFER(xxx_i32queue_deinit(&que));

    for (int i = 0; i < 16; ++i) {
        ASSERT(xxx_i32queue_push(&que, i) == 0);
    }
    
    ASSERT(xxx_i32queue_capacity(&que) == 16);
    ASSERT(xxx_i32queue_size(&que) == 16);
    ASSERT(*xxx_i32queue_front(&que) == 0);
    ASSERT(*xxx_i32queue_back(&que) == 15);

    ASSERT(xxx_i32queue_push(&que, 16) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 32);
    ASSERT(xxx_i32queue_size(&que) == 17);

    ASSERT(*xxx_i32queue_front(&que) == 0);
    ASSERT(*xxx_i32queue_back(&que) == 16);

    xxx_i32queue_clear(&que);
    ASSERT(xxx_i32queue_capacity(&que) == 32);
    ASSERT(xxx_i32queue_size(&que) == 0);
    ASSERT(xxx_i32queue_empty(&que));
}

TEST(xxx_i32queue_pop) {
    xxx_i32queue_t que;
    ASSERT(xxx_i32queue_init(&que) == 0);
    DEFER(xxx_i32queue_deinit(&que));

    for (int i = 0; i < 16; ++i) {
        ASSERT(xxx_i32queue_push(&que, i) == 0);
    }

    for (int i = 16; i < 100; ++i) {
        xxx_i32queue_pop(&que);
        ASSERT(xxx_i32queue_push(&que, i) == 0);
    }

    ASSERT(xxx_i32queue_capacity(&que) == 16);
    ASSERT(*xxx_i32queue_front(&que) == 84);
    ASSERT(*xxx_i32queue_back(&que) == 99);

    ASSERT(xxx_i32queue_push(&que, 100) == 0);
    ASSERT(xxx_i32queue_capacity(&que) == 32);
    ASSERT(*xxx_i32queue_front(&que) == 84);
    ASSERT(*xxx_i32queue_back(&que) == 100);

    for (int i = 84; i <= 100; ++i) {
        ASSERT(*xxx_i32queue_front(&que) == i, "i=%d, front=%d", i, *xxx_i32queue_front(&que));
        xxx_i32queue_pop(&que);
    }

    ASSERT(xxx_i32queue_empty(&que));
}

TEST(xxx_i32queue_copy) {

}
