#include <benchmark/benchmark.h>
#include <unordered_map>

#include "xxx/xxx_i32i32hashtable.h"

namespace {

static void BM_std_unordered_map_insert(benchmark::State& state) {
    size_t n = state.range(0);
    std::unordered_map<int, int> hm;
    for (auto _ : state) {
        hm.clear();
        for (size_t i = 0; i < n; ++i) {
            hm.emplace(i, i);
        }
        benchmark::DoNotOptimize(hm.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_map_insert)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_i32i32hashtable_insert(benchmark::State& state) {
    size_t n = state.range(0);
    xxx_i32i32hashtable_t hm;
    xxx_i32i32hashtable_init(&hm);
    for (auto _ : state) {
        xxx_i32i32hashtable_clear(&hm);
        for (size_t i = 0; i < n; ++i) {
            xxx_i32i32hashtable_insert(&hm, i, i);
        }
        benchmark::DoNotOptimize(xxx_i32i32hashtable_bucket_count(&hm));
    }
    xxx_i32i32hashtable_deinit(&hm);
}
BENCHMARK(BM_xxx_i32i32hashtable_insert)->RangeMultiplier(8)->Range(1, 1 << 16);

}
