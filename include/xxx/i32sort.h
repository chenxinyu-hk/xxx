#ifndef XXX_I32SORT_H
#define XXX_I32SORT_H

#include "xxx/i32heap.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void xxx_i32quicksort(int *arr, size_t len);
static inline void xxx_i32quicksort_asc(int *arr, size_t len);
static inline void xxx_i32quicksort_desc(int *arr, size_t len);

static inline void xxx_i32heapsort(int *arr, size_t len);
static inline void xxx_i32heapsort_asc(int *arr, size_t len);
static inline void xxx_i32heapsort_desc(int *arr, size_t len);

static inline void xxx_i32insertionsort(int *arr, size_t len);
static inline void xxx_i32insertionsort_asc(int *arr, size_t len);
static inline void xxx_i32insertionsort_desc(int *arr, size_t len);

#ifdef __cplusplus
}
#endif

static inline
void xxx_i32swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

static inline
void xxx_i32quicksort(int *arr, size_t len) {
    xxx_i32quicksort_asc(arr, len);
}

// static inline
// size_t xxx_i32partition_asc(int *arr, size_t len) {
//     int pivot = arr[len / 2];
//     int i = 0;
//     int j = len - 1;
//     while (1) {
//         while (arr[i] < pivot) {
//             ++i;
//         }
//         while (arr[j] > pivot) {
//             --j;
//         }
//         if (i >= j) {
//             return j;
//         }
//         xxx_i32swap(&arr[i], &arr[j]);
//         ++i;
//         --j;
//     }
// }

static inline
void xxx_i32quicksort_asc_impl(int *arr, size_t len) {
    if (len <= 1) {
        return;
    }
    int pivot = arr[len / 2];
    int lt = 0;
    int gt = len - 1;
    int i = 0;
    while (i <= gt) {
        if (arr[i] < pivot) {
            xxx_i32swap(&arr[lt], &arr[i]);
            ++lt;
            ++i;
        } else if (arr[i] > pivot) {
            xxx_i32swap(&arr[gt], &arr[i]);
            --gt;
        } else {
            ++i;
        }
    }
    xxx_i32quicksort_asc_impl(arr, lt);
    xxx_i32quicksort_asc_impl(arr + gt + 1, len - gt - 1);
}

static inline
void xxx_i32quicksort_asc(int *arr, size_t len) {
    xxx_i32quicksort_asc_impl(arr, len);
}

// static inline
// size_t xxx_i32partition_desc(int *arr, size_t len) {
//     int pivot = arr[len / 2];
//     size_t i = 0;
//     size_t j = len - 1;
//     while (1) {
//         while (arr[i] > pivot) {
//             ++i;
//         }
//         while (arr[j] < pivot) {
//             --j;
//         }
//         if (i >= j) {
//             return j;
//         }
//         xxx_i32swap(&arr[i], &arr[j]);
//         ++i;
//         --j;
//     }
// }

static inline
void xxx_i32quicksort_desc_impl(int *arr, size_t len) {
    if (len <= 1) {
        return;
    }
    int pivot = arr[len / 2];
    int lt = 0;
    int gt = len - 1;
    int i = 0;
    while (i <= gt) {
        if (arr[i] > pivot) {
            xxx_i32swap(&arr[lt], &arr[i]);
            ++lt;
            ++i;
        } else if (arr[i] < pivot) {
            xxx_i32swap(&arr[gt], &arr[i]);
            --gt;
        } else {
            ++i;
        }
    }
    xxx_i32quicksort_asc_impl(arr, lt);
    xxx_i32quicksort_asc_impl(arr + gt + 1, len - gt - 1);
}

static inline
void xxx_i32quicksort_desc(int *arr, size_t len) {
    xxx_i32quicksort_desc_impl(arr, len);
}

static inline
void xxx_i32heapsort(int *arr, size_t len) {
    xxx_i32heapsort_asc(arr, len);
}

static inline
void xxx_i32heapsort_asc(int *arr, size_t len) {
    xxx_i32maxheap_build(arr, len);
    while (len > 1) {
        xxx_i32swap(&arr[0], &arr[--len]);
        xxx_i32maxheap_sift_down(arr, len, 0);
    }
}

static inline
void xxx_i32heapsort_desc(int *arr, size_t len) {
    xxx_i32minheap_build(arr, len);
    while (len > 1) {
        xxx_i32swap(&arr[0], &arr[--len]);
        xxx_i32minheap_sift_down(arr, len, 0);
    }
}

static inline
void xxx_i32insertionsort(int *arr, size_t len) {
    xxx_i32insertionsort_asc(arr, len);
}

static inline
void xxx_i32insertionsort_asc(int *arr, size_t len) {
    for (size_t i = 1; i < len; ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

static inline
void xxx_i32insertionsort_desc(int *arr, size_t len) {
    for (size_t i = 1; i < len; ++i) {
        int key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] < key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

#endif
