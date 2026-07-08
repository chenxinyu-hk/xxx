#include <benchmark/benchmark.h>
#include <vector>

#include "xxx/xxx_array.h"

namespace {

static void BM_std_vector_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    std::vector<void*> vec;
    vec.reserve(n);
    for (auto _ : state) {
        vec.clear();
        for (size_t i = 0; i < n; ++i) {
            vec.push_back(nullptr);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}
BENCHMARK(BM_std_vector_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_array_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    xxx_array_t arr;
    xxx_array_init(&arr);
    xxx_array_reserve(&arr, n);
    for (auto _ : state) {
        xxx_array_clear(&arr);
        for (size_t i = 0; i < n; ++i) {
            xxx_array_push_back(&arr, nullptr);
        }
        benchmark::DoNotOptimize(xxx_array_data(&arr));
    }
    xxx_array_deinit(&arr);
}
BENCHMARK(BM_xxx_array_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_std_vector_push_back_with_grow(benchmark::State& state) {
    const size_t n = state.range(0);
    for (auto _ : state) {
        std::vector<void*> vec;
        for (size_t i = 0; i < n; ++i) {
            vec.push_back(nullptr);
        }
        benchmark::DoNotOptimize(vec.data());
    }
}
BENCHMARK(BM_std_vector_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_array_push_back_with_grow(benchmark::State& state) {
    const size_t n = state.range(0);
    for (auto _ : state) {
        xxx_array_t arr;
        xxx_array_init(&arr);
        for (size_t i = 0; i < n; ++i) {
            xxx_array_push_back(&arr, nullptr);
        }
        benchmark::DoNotOptimize(xxx_array_data(&arr));
        xxx_array_deinit(&arr);
    }
}
BENCHMARK(BM_xxx_array_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

}
