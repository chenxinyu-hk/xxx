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

static inline int xxx_base64_decode(
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

static const uint8_t xxx_base64_decode_table[256] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0x40, 0xff, 0xff,
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static inline
int xxx_base64_encode(
    const void *in_buf, size_t in_len,
    char *out_buf, size_t out_cap, size_t *out_len)
{
    if (in_len > ((size_t)-1 - 1) / 4 * 3) {
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
                | ((uint32_t)data[i + 1] << 8) | (uint32_t)data[i + 2];
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

static inline
int xxx_base64_decode(
    const void *in_buf, size_t in_len,
    char *out_buf, size_t out_cap, size_t *out_len)
{
    if (in_len == 0) {
        if (out_cap < 1) {
            return -1;
        }
        out_buf[0] = '\0';
        if (out_len != NULL) {
            *out_len = 0;
        }
        return 0;
    }
    if (in_len & 3) {
        return -1;
    }
    size_t required = in_len / 4 * 3 + 1;
    const uint8_t *data = (const uint8_t *)in_buf;
    if (data[in_len - 1] == '=') {
        --required;
        if (data[in_len - 2] == '=') {
            --required;
        }
    }
    if (out_cap < required) {
        return -1;
    }
    size_t i = 0;
    size_t j = 0;
    while (i < in_len) {
        uint8_t v0 = xxx_base64_decode_table[data[i++]];
        uint8_t v1 = xxx_base64_decode_table[data[i++]];
        uint8_t v2 = xxx_base64_decode_table[data[i++]];
        uint8_t v3 = xxx_base64_decode_table[data[i++]];
        if (v0 > 0x3f || v1 > 0x3f) {
            return -1;
        }
        if (v2 == 0xff || v3 == 0xff) {
            return -1;
        }
        if (v2 <= 0x3f && v3 <= 0x3f) {
            uint32_t bits = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12)
                | ((uint32_t)v2 << 6) | (uint32_t)v3;
            out_buf[j++] = (char)(bits >> 16);
            out_buf[j++] = (char)(bits >> 8);
            out_buf[j++] = (char)bits;
            continue;
        }
        if (i != in_len) {
            return -1;
        }
        if (v2 == 0x40 && v3 == 0x40) {
            if (v1 & 0x0f) {
                return -1;
            }
            uint32_t bits = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12);
            out_buf[j++] = (char)(bits >> 16);
            continue;
        }
        if (v2 <= 0x3f) {
            if (v2 & 0x03) {
                return -1;
            }
            uint32_t bits = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12) | ((uint32_t)v2 << 6);
            out_buf[j++] = (char)(bits >> 16);
            out_buf[j++] = (char)(bits >> 8);
            continue;
        }
        return -1;
    }
    out_buf[j] = '\0';
    if (out_len != NULL) {
        *out_len = j;
    }
    return 0;
}

// static inline
// int xxx_base64_decode(
//     const void *in_buf, size_t in_len,
//     char *out_buf, size_t out_cap, size_t *out_len)
// {
//     if (in_len == 0) {
//         if (out_cap < 1) {
//             return -1;
//         }
//         out_buf[0] = '\0';
//         if (out_len != NULL) {
//             *out_len = 0;
//         }
//         return 0;
//     }
//     if (in_len & 3) {
//         return -1;
//     }
//     size_t required = in_len / 4 * 3 + 1;
//     const uint8_t *data = (const uint8_t *)in_buf;
//     if ((char)data[in_len - 1] == '=') {
//         --required;
//         if ((char)data[in_len - 2] == '=') {
//             --required;
//         }
//     }
//     if (out_cap < required) {
//         return -1;
//     }
//     size_t i = 0;
//     size_t j = 0;
//     while (i < in_len) {
//         uint32_t bits = 0;
//         size_t remain = 3;
//         for (size_t k = 0; k < 4; ++k) {
//             char c = (char)data[i++];
//             if (c >= 'A' && c <= 'Z') {
//                 bits = (bits << 6) | (uint32_t)(c - 'A');
//             } else if (c >= 'a' && c <= 'z') {
//                 bits = (bits << 6) | (uint32_t)(c - 'a' + 26);
//             } else if (c >= '0' && c <= '9') {
//                 bits = (bits << 6) | (uint32_t)(c - '0' + 52);
//             } else if (c == '+') {
//                 bits = (bits << 6) | 62;
//             } else if (c == '/') {
//                 bits = (bits << 6) | 63;
//             } else if (c == '=') {
//                 if (in_len - i > 1) {
//                     return -1;
//                 }
//                 if (i == in_len) {
//                     bits <<= 6;
//                     remain = 2;
//                 } else {
//                     if ((char)data[i++] != '=') {
//                         return -1;
//                     }
//                     bits <<= 12;
//                     remain = 1;
//                 }
//                 break;
//             } else {
//                 return -1;
//             }
//         }
//         if (remain == 3) {
//             out_buf[j++] = (char)((bits >> 16) & 0xff);
//             out_buf[j++] = (char)((bits >> 8) & 0xff);
//             out_buf[j++] = (char)(bits & 0xff);
//         } else if (remain == 2) {
//             if (bits & 0xff) {
//                 return -1;
//             }
//             out_buf[j++] = (char)((bits >> 16) & 0xff);
//             out_buf[j++] = (char)((bits >> 8) & 0xff);
//         } else {
//             if (bits & 0xffff) {
//                 return -1;
//             }
//             out_buf[j++] = (char)((bits >> 16) & 0xff);
//         }
//     }
//     out_buf[j] = '\0';
//     if (out_len != NULL) {
//         *out_len = j;
//     }
//     return 0;
// }

#endif
