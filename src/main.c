#include <stdio.h>

#include "xxx/string.h"
#include "xxx/i32array.h"
#include "xxx/i32hashtable.h"
#include "xxx/i32i32hashtable.h"
#include "xxx/i32stack.h"
#include "xxx/i32queue.h"
#include "xxx/i32heap.h"
#include "xxx/i32sort.h"
// #include "xxx/i32rbtree.h"
#include "xxx/bitset.h"
#include "xxx/base64.h"

int main() {
    char in_buf[] = "g28g8dgyxgwy7171881";
    char out_buf[1024];
    xxx_base64_encode(in_buf, sizeof(in_buf), out_buf, sizeof(out_buf), NULL);
    printf("%s\n", out_buf);
}
