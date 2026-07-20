#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <bitset>

#include "xxx/xxx_string.h"
#include "xxx/xxx_array.h"
#include "xxx/xxx_i32array.h"
#include "xxx/xxx_stack.h"
#include "xxx/xxx_i32stack.h"
#include "xxx/xxx_bitset.h"
#include "xxx/xxx_i32heap.h"
#include "xxx/xxx_i32sort.h"

using namespace std;


int main() {
    int nums[] = {10,9,1,2,3,5,7,10, 11, 12, -1,-5};
    int n = sizeof(nums) / sizeof(int);
    int len = n;
    xxx_i32heapsort_desc(nums, len);
    for (int i = 0; i < n; ++i) {
        printf("%d,", nums[i]);
    }
}
