#ifndef XXX_I32SORT_H
#define XXX_I32SORT_H

#include "xxx_i32heap.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void xxx_i32quicksort(int *arr, size_t len);
static inline void xxx_i32quicksort_asc(int *arr, size_t len);
static inline void xxx_i32quicksort_desc(int *arr, size_t len);

static inline void xxx_i32heapsort(int *arr, size_t len);
static inline void xxx_i32heapsort_asc(int *arr, size_t len);
static inline void xxx_i32heapsort_desc(int *arr, size_t len);

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
size_t xxx_i32partition_asc(int *arr, size_t len) {
    int pivot = arr[len / 2];
    size_t i = 0;
    size_t j = len - 1;
    while (1) {
        while (arr[i] < pivot) {
            ++i;
        }
        while (arr[j] > pivot) {
            --j;
        }
        if (i >= j) {
            return j;
        }
        xxx_i32swap(&arr[i], &arr[j]);
        ++i;
        --j;
    }
}

static inline
size_t xxx_i32partition_desc(int *arr, size_t len) {
    int pivot = arr[len / 2];
    size_t i = 0;
    size_t j = len - 1;
    while (1) {
        while (arr[i] > pivot) {
            ++i;
        }
        while (arr[j] < pivot) {
            --j;
        }
        if (i >= j) {
            return j;
        }
        xxx_i32swap(&arr[i], &arr[j]);
        ++i;
        --j;
    }
}

static inline
void xxx_i32quicksort_asc_impl(int *arr, size_t len) {
    size_t pivot = xxx_i32partition_asc(arr, len);
    if (pivot > 1) {
        xxx_i32quicksort_asc_impl(arr, pivot);
    }
    size_t right_len = len - pivot - 1;
    if (right_len > 1) {
        xxx_i32quicksort_asc_impl(arr + pivot + 1, right_len);
    }
}

static inline
void xxx_i32quicksort_desc_impl(int *arr, size_t len) {
    size_t pivot = xxx_i32partition_desc(arr, len);
    if (pivot > 1) {
        xxx_i32quicksort_desc_impl(arr, pivot);
    }
    size_t right_len = len - pivot - 1;
    if (right_len > 1) {
        xxx_i32quicksort_desc_impl(arr + pivot + 1, right_len);
    }
}

static inline
void xxx_i32quicksort(int *arr, size_t len) {
    xxx_i32quicksort_asc(arr, len);
}

static inline
void xxx_i32quicksort_asc(int *arr, size_t len) {
    if (len <= 1) {
        return;
    }
    xxx_i32quicksort_asc_impl(arr, len);
}

static inline
void xxx_i32quicksort_desc(int *arr, size_t len) {
    if (len <= 1) {
        return;
    }
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

#endif
