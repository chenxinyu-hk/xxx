#include <benchmark/benchmark.h>
#include <vector>

#include "xxx/i32array.h"

namespace {

static void BM_std_vector_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    std::vector<int> vec;
    vec.reserve(n);
    for (auto _ : state) {
        vec.clear();
        for (size_t i = 0; i < n; ++i) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}
BENCHMARK(BM_std_vector_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_i32array_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    xxx_i32array_t arr;
    xxx_i32array_init(&arr);
    xxx_i32array_reserve(&arr, n);
    for (auto _ : state) {
        xxx_i32array_clear(&arr);
        for (size_t i = 0; i < n; ++i) {
            xxx_i32array_push_back(&arr, i);
        }
        benchmark::DoNotOptimize(xxx_i32array_data(&arr));
    }
    xxx_i32array_deinit(&arr);
}
BENCHMARK(BM_xxx_i32array_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_std_vector_push_back_with_grow(benchmark::State& state) {
    const size_t n = state.range(0);
    for (auto _ : state) {
        std::vector<int> vec;
        for (size_t i = 0; i < n; ++i) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}
BENCHMARK(BM_std_vector_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_i32array_push_back_with_grow(benchmark::State& state) {
    const size_t n = state.range(0);
    for (auto _ : state) {
        xxx_i32array_t arr;
        xxx_i32array_init(&arr);
        for (size_t i = 0; i < n; ++i) {
            xxx_i32array_push_back(&arr, i);
        }
        benchmark::DoNotOptimize(xxx_i32array_data(&arr));
        xxx_i32array_deinit(&arr);
    }
}
BENCHMARK(BM_xxx_i32array_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

}
