#ifndef XXX_BASE64_H
#define XXX_BASE64_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int xxx_base64_encode(
    const void *in_buf, size_t in_len,
    char *out_buf, size_t out_cap, size_t *out_len);

#ifdef __cplusplus
}
#endif

static const char xxx_base64_encode_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "+/";

static inline
int xxx_base64_encode(
    const void *in_buf, size_t in_len,
    char *out_buf, size_t out_cap, size_t *out_len)
{
    if (in_len > (SIZE_MAX - 1) / 4 * 3) {
        return -1;
    }
    size_t required = (in_len + 2) / 3 * 4 + 1;
    if (out_cap < required) {
        return -1;
    }
    const uint8_t *data = (const uint8_t *)in_buf;
    size_t i = 0;
    size_t j = 0;
    while (i < in_len) {
        size_t remain = in_len - i;
        uint32_t bits = 0;
        if (remain >= 3) {
            bits = ((uint32_t)data[i] << 16)
              | ((uint32_t)data[i + 1] << 8)
              | (uint32_t)data[i + 2];
            i += 3;
            out_buf[j++] = xxx_base64_encode_table[(bits >> 18) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[(bits >> 12) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[(bits >> 6) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[bits & 0x3f];
        } else if (remain == 2) {
            bits = ((uint32_t)data[i] << 16)
              | ((uint32_t)data[i + 1] << 8);
            i += 2;
            out_buf[j++] = xxx_base64_encode_table[(bits >> 18) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[(bits >> 12) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[(bits >> 6) & 0x3f];
            out_buf[j++] = '=';
        } else {
            bits = (uint32_t)data[i] << 16;
            i += 1;
            out_buf[j++] = xxx_base64_encode_table[(bits >> 18) & 0x3f];
            out_buf[j++] = xxx_base64_encode_table[(bits >> 12) & 0x3f];
            out_buf[j++] = '=';
            out_buf[j++] = '=';
        }
    }
    out_buf[j] = '\0';
    if (out_len != NULL) {
        *out_len = j;
    }
    return 0;
}



#endif
