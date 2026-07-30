#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <bitset>
#include <stack>
#include <queue>

// #include "xxx/xxx_string.h"
// #include "xxx/xxx_array.h"
// #include "xxx/xxx_i32array.h"
// #include "xxx/xxx_stack.h"
// #include "xxx/xxx_i32stack.h"
// #include "xxx/xxx_bitset.h"
// #include "xxx/xxx_i32heap.h"
#include "xxx/xxx_i32sort.h"

using namespace std;

int main() {
    int a[] = {2,2,2,2,2,2,2,2,2};
    xxx_i32quicksort_asc(a, sizeof(a) / sizeof(int));
}
