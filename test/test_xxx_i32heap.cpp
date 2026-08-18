#include "test.h"
#include "xxx/i32heap.h"

TEST(xxx_i32maxheap_build) {
    int arr[] = {8, 3, 2, 7, 0, 1, 4, 9, 4, 5};
    size_t len = sizeof(arr) / sizeof(int);

    xxx_i32maxheap_build(arr, len);
    
    for (size_t i = 1; i < len; ++i) {
        ASSERT(arr[i] <= arr[(i-1)/2]);
    }
}

TEST(xxx_i32maxheap_push) {
    xxx_i32maxheap_t heap;
    ASSERT(xxx_i32maxheap_init(&heap) == 0);
    DEFER(xxx_i32maxheap_deinit(&heap));

    int arr[] = {8, 3, 2, 7, 0, 1, 4, 9, 4, 5};
    size_t len = sizeof(arr) / sizeof(int);

    ASSERT(xxx_i32maxheap_reserve(&heap, len) == 0);

    int curr_max = INT_MIN;
    for (size_t i = 0; i < len; ++i) {
        if (curr_max < arr[i]) {
            curr_max = arr[i];
        }
        ASSERT(xxx_i32maxheap_push(&heap, arr[i]) == 0);
        ASSERT(*xxx_i32maxheap_top(&heap) == curr_max);
    }
}

TEST(xxx_i32maxheap_pop) {
    xxx_i32maxheap_t heap;
    ASSERT(xxx_i32maxheap_init(&heap) == 0);
    DEFER(xxx_i32maxheap_deinit(&heap));

    int arr[] = {8, 3, 2, 7, 0, 1, 4, 9, 4, 5};
    size_t len = sizeof(arr) / sizeof(int);

    ASSERT(xxx_i32maxheap_reserve(&heap, len) == 0);

    for (size_t i = 0; i < len; ++i) {
        ASSERT(xxx_i32maxheap_push(&heap, arr[i]) == 0);
    }

    int prev_min = INT_MAX;
    for (size_t i = 0; i < len; ++i) {
        ASSERT(*xxx_i32maxheap_top(&heap) <= prev_min);
        prev_min = *xxx_i32maxheap_top(&heap);
        xxx_i32maxheap_pop(&heap);
    }
}
