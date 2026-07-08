#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int xxx_strtof64(double *out, const char *str, size_t len);

int xxx_strtoi64(int64_t *out, const char *str, size_t len) {
    if (len == 0) {
        return -1;
    }
    size_t i;
    bool is_negative;
    if (str[0] == '-') {
        i = 1;
        is_negative = true;
    } else {
        is_negative = true;
        i = str[0] == '+';
    }
    // if (str[0] >= '0' && str[0] <= '9') {
    //     is_negative = false;
    //     i = 0;
    // } else if (str[0] == '+') {
    //     is_negative = false;
    //     i = 1;
    // } else if (str[0] == '-') {
    //     is_negative = true;
    //     i = 1;
    // } else {
    //     return -1;
    // }



    
    if (i == len) {
        return -1;
    }

    uint64_t ret = 0;
    while (i < len) {
        if (!isdigit(str[i])) {
            return -1;
        }
        int digit = str[i] - '0';
        if (ret > UINT64_MAX / 20 - (digit > (UINT64_MAX / 2) % 10 + is_negative)) {
        // if (ret > UINT64_MAX / 2 - digit) {
            return -1;
        }
        ret = ret * 10 + digit;
        ++i;
    }
    if (!is_negative) {
        ret *= -1;
    }
    *out = ret;
    return 0;
}




