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

// _Atomic int c;

// void *print_a(void *arg) {
//     while (1) {
//         pthread_mutex_lock(&mtx);
//         if (atomic_load(&c) == 'a') {
//             printf("a");
//             atomic_store(&c, 'b');
//         }
//         pthread_mutex_unlock(&mtx);
//     }
//     return NULL;
// }


// int ready;

// void *print_a(void *arg) {
//     pthread_mutex_lock(&mtx);
//     while (ready != 'a') {
//         pthread_cond_wait(&cv1, &mtx);
//     }
//     printf("a");
//     ready = 'b';
//     pthread_cond_signal(&cv2);
//     pthread_mutex_unlock(&mtx);
// }


// int main() {
    
// }


// typedef struct {
//     void *(*func)(void *);
//     void *arg;
//     task_t *next;
// } task_t;

// typedef struct {
//     pthread_t *tids;
//     int thread_num;

//     task_t *head;
//     task_t *tail;
//     int task_num;
    
//     pthread_mutex_t mtx;
//     pthread_cond_t cv;

//     int stop;
// } thread_pool_t;

// void *consumer(void *arg) {
//     thread_pool_t *tp = (thread_pool_t *)arg;
//     while (1) {
//         pthread_mutex_lock(&tp->mtx);
//         while (tp->task_num == 0 && !tp->stop) {
//             pthread_cond_wait(&tp->cv, &tp->mtx);
//         }
//         if (tp->stop) {
//             pthread_mutex_unlock(&tp->mtx);
//             return NULL;
//         }
//         task_t *task = tp->head;
//         tp->head = tp->head->next;
//         --tp->thread_num;

//         pthread_mutex_unlock(&tp->mtx);
//         task->func(task->arg);
//         free(task);
//     }
// }

// int thread_pool_init(thread_pool_t *tp, int thread_num) {
//     tp->tids = (pthread_t *)malloc(thread_num * sizeof(pthread_t));
//     if (tp->tids == NULL) {
//         return -1;
//     }
//     tp->thread_num = thread_num;

//     tp->head = NULL;
//     tp->tail = NULL;
//     tp->task_num = 0;

//     pthread_mutex_init(&tp->mtx, NULL);
//     pthread_cond_init(&tp->cv, NULL);

//     tp->stop = 0;

//     for (int i = 0; i < thread_num; ++i) {
//         pthread_create(&tp->tids[i], NULL, consumer, tp);
//     }
//     return 0;
// }

// void thread_pool_deinit(thread_pool_t *tp) {

// }

// #include <stdatomic.h>

// typedef struct xxx_spsc_i32queue xxx_spsc_i32queue_t;

// struct xxx_spsc_i32queue {
//     int *buf;
//     size_t cap;
//     alignas(64) _Atomic size_t head;
//     alignas(64) _Atomic size_t tail;
// };

// int xxx_spsc_i32queue_init(xxx_spsc_i32queue_t *self, size_t n) {
//     self->buf = (int *)malloc(n * sizeof(int));
//     if (self->buf == NULL) {
//         return -1;
//     }
//     self->cap = n;
//     atomic_init(&self->head, 0);
//     atomic_init(&self->tail, 0);
//     return 0;
// }



// int xxx_spsc_i32queue_push(xxx_spsc_i32queue_t *self, int x) {
//     for (int i = 0; i < 5; ++i) {
//         size_t head = atomic_load_explicit(&self->head, memory_order_acquire);
//         size_t tail = atomic_load_explicit(&self->tail, memory_order_acquire);
//         if (tail - head < self->cap) {
//             atomic_fetch_add_explicit(&self->tail, memory_order_release);
//             return 0;
//         }
//     }
//     return -1;
// }

// int xxx_spsc_i32queue_pop(xxx_spsc_i32queue_t *self, int *ret) {
//     for (int i = 0; i < 5; ++i) {
//         size_t head = atomic_load_explicit(&self->head, memory_order_acquire);
//         size_t tail = atomic_load_explicit(&self->tail, memory_order_acquire);
//         if (tail - head > 0) {
//             atomic_fetch_add_explicit(&self->head, memory_order_release);
//             *ret = self->buf[head % self->cap];
//             return 0;
//         }
//     }
//     return -1;
// }



// #include <stdatomic.h>
// #include <stddef.h>
// #include <stdlib.h>

// typedef struct xxx_spsc_i32queue {
//     int *buf;
//     size_t cap;

//     alignas(64) _Atomic size_t head;
//     alignas(64) _Atomic size_t tail;
// } xxx_spsc_i32queue_t;

// int xxx_spsc_i32queue_init(xxx_spsc_i32queue_t *self, size_t cap)
// {
//     self->buf = malloc(cap * sizeof(int));
//     if (self->buf == NULL) {
//         return -1;
//     }

//     self->cap = cap;

//     atomic_init(&self->head, 0);
//     atomic_init(&self->tail, 0);

//     return 0;
// }

// void xxx_spsc_i32queue_destroy(xxx_spsc_i32queue_t *self)
// {
//     free(self->buf);
//     self->buf = NULL;
// }

// int xxx_spsc_i32queue_push(xxx_spsc_i32queue_t *self, int value)
// {
//     /* tail 只有 producer 修改 */
//     size_t tail = atomic_load_explicit(&self->tail, memory_order_relaxed);

//     /* head 由 consumer 修改，需要 acquire */
//     size_t head = atomic_load_explicit(&self->head, memory_order_acquire);

//     if (tail - head == self->cap) {
//         return -1;      /* full */
//     }

//     self->buf[tail % self->cap] = value;

//     atomic_store_explicit(
//         &self->tail,
//         tail + 1,
//         memory_order_release);

//     return 0;
// }

// int xxx_spsc_i32queue_pop(xxx_spsc_i32queue_t *self, int *value)
// {
//     /* head 只有 consumer 修改 */
//     size_t head = atomic_load_explicit(&self->head, memory_order_relaxed);

//     /* tail 由 producer 修改，需要 acquire */
//     size_t tail = atomic_load_explicit(&self->tail, memory_order_acquire);

//     if (tail == head) {
//         return -1;      /* empty */
//     }

//     *value = self->buf[head % self->cap];

//     atomic_store_explicit(
//         &self->head,
//         head + 1,
//         memory_order_release);

//     return 0;
// }


// typedef struct xxx_mpsc_i32queue_slot {
//     alignas(64) _Atomic size_t state;
//     int value;
// } xxx_mpsc_i32queue_slot_t;

// typedef struct xxx_mpsc_i32queue {
//     xxx_mpsc_i32queue_slot_t *slots;
//     size_t cap;

//     alignas(64) _Atomic size_t head;
//     alignas(64) _Atomic size_t tail;
// } xxx_mpsc_i32queue_t;

// int xxx_mpsc_i32queue_push(xxx_mpsc_i32queue_t *self, int value) {
//     while (1) {
//         size_t head = atomic_load_explicit(&self->head, memory_order_acquire);
//         size_t tail = atomic_load_explicit(&self->tail, memory_order_acquire);
//         if (tail - head == self->cap) {
//             return -1;
//         }
//         size_t expected = tail;
//         size_t desired = tail + 1;
//         if (atomic_compare_exchange_weak(&self->tail, &expected, desired)) {
//             xxx_mpsc_i32queue_slot_t *slot = &self->slots[tail % self->cap];
//             slot->value = value;
//             atomic_store_explicit(&slot->state, 1, memory_order_release);
//             return 0;
//         }
//     }
// }

// int xxx_mpsc_i32queue_pop(xxx_mpsc_i32queue_t *self, int *value) {
//     size_t head = atomic_load_explicit(&self->head, memory_order_relaxed);
//     size_t tail = atomic_load_explicit(&self->tail, memory_order_acquire);
//     if (tail == head) {
//         return -1;
//     }
//     while (1) {
//         xxx_mpsc_i32queue_slot_t *slot = &self->slots[head % self->cap];
//         if (atomic_load_explicit(&slot->state, memory_order_acquire) != 1) {
//             continue;
//         }
//         *value = slot->value;
//         atomic_store_explicit(&slot->state, 0, memory_order_relaxed);
//         atomic_store_explicit(&self->head, head + 1, memory_order_release);
//         return 0;
//     }
// }

#include <stdio.h>
// #include <string>
// #include <vector>
// #include <unordered_set>
// #include <unordered_map>
// #include <stack>
// #include <queue>

// using namespace std;

int main() {
    char in_buf[] = "a+181919j3691196199812363828283723927392 ns";
    char out_buf[1024];
    xxx_base64_encode(in_buf, strlen(in_buf), out_buf, sizeof(out_buf), NULL);
    printf("%s\n", out_buf);
    xxx_base64_decode(out_buf, strlen(out_buf), out_buf, sizeof(out_buf), NULL);
    printf("%s\n", out_buf);
}
