#include "test.h"
#include "xxx/i32hashtable.h"

TEST(xxx_i32hashtable_insert) {
    xxx_i32hashtable_t set;
    ASSERT(xxx_i32hashtable_init(&set) == 0);
    DEFER(xxx_i32hashtable_deinit(&set));

    for (int i = 0; i < 100; ++i) {
        ASSERT(xxx_i32hashtable_insert(&set, i) == 0);
        xxx_i32hashtable_erase(&set, i);
    }
    ASSERT(xxx_i32hashtable_bucket_count(&set) == 16);
    xxx_i32hashtable_contains(&set, 1);
    // xxx_i32hashtable_insert(&set, 1);
    // xxx_i32hashtable_insert(&set, 10);
    // xxx_i32hashtable_insert(&set, 100);
}
