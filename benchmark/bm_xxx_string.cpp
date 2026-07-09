#include <benchmark/benchmark.h>
#include <string>

#include "xxx/xxx_string.h"

namespace {

static void BM_std_string_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    std::string str;
    str.reserve(n);
    for (auto _ : state) {
        str.clear();
        for (size_t i = 0; i < n; ++i) {
            str.push_back('x');
        }
        benchmark::DoNotOptimize(str.c_str());
    }
}
BENCHMARK(BM_std_string_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_string_push_back_reserved(benchmark::State& state) {
    size_t n = state.range(0);
    xxx_string_t str;
    xxx_string_init(&str);
    xxx_string_reserve(&str, n);
    for (auto _ : state) {
        xxx_string_clear(&str);
        for (size_t i = 0; i < n; ++i) {
            xxx_string_push_back(&str, 'x');
        }
        benchmark::DoNotOptimize(xxx_string_c_str(&str));
    }
    xxx_string_deinit(&str);
}
BENCHMARK(BM_xxx_string_push_back_reserved)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_std_string_push_back_with_grow(benchmark::State& state) {
    size_t n = state.range(0);
    for (auto _ : state) {
        std::string str;
        for (size_t i = 0; i < n; ++i) {
            str.push_back('x');
        }
        benchmark::DoNotOptimize(str.c_str());
    }
}
BENCHMARK(BM_std_string_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_string_push_back_with_grow(benchmark::State& state) {
    size_t n = state.range(0);
    for (auto _ : state) {
        xxx_string_t str;
        xxx_string_init(&str);
        for (size_t i = 0; i < n; ++i) {
            xxx_string_push_back(&str, 'x');
        }
        benchmark::DoNotOptimize(xxx_string_c_str(&str));
        xxx_string_deinit(&str);
    }
}
BENCHMARK(BM_xxx_string_push_back_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_std_string_append_with_grow(benchmark::State& state) {
    size_t n = state.range(0);
    for (auto _ : state) {
        std::string str;
        str.assign("xxx");
        for (size_t i = 0; i < n; ++i) {
            str.append(str.c_str() + i, 3);
        }
        benchmark::DoNotOptimize(str.c_str());
    }
}
BENCHMARK(BM_std_string_append_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_string_append_with_grow(benchmark::State& state) {
    size_t n = state.range(0);
    for (auto _ : state) {
        xxx_string_t str;
        xxx_string_init(&str);
        xxx_string_assign(&str, "xxx", 3);
        for (size_t i = 0; i < n; ++i) {
            xxx_string_append(&str, xxx_string_c_str(&str) + i, 3);
        }
        benchmark::DoNotOptimize(xxx_string_c_str(&str));
        xxx_string_deinit(&str);
    }
}
BENCHMARK(BM_xxx_string_append_with_grow)->RangeMultiplier(8)->Range(1, 1 << 16);

}
