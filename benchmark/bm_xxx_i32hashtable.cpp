#include <benchmark/benchmark.h>
#include <unordered_set>

#include "xxx/i32hashtable.h"

namespace {

static void BM_std_unordered_set_insert(benchmark::State& state) {
    size_t n = state.range(0);
    std::unordered_set<int> hs;
    for (auto _ : state) {
        hs.clear();
        for (size_t i = 0; i < n; ++i) {
            hs.emplace(i);
        }
        benchmark::DoNotOptimize(hs.bucket_count());
    }
}
BENCHMARK(BM_std_unordered_set_insert)->RangeMultiplier(8)->Range(1, 1 << 16);

static void BM_xxx_i32hashtable_insert(benchmark::State& state) {
    size_t n = state.range(0);
    xxx_i32hashtable_t hs;
    xxx_i32hashtable_init(&hs);
    for (auto _ : state) {
        xxx_i32hashtable_clear(&hs);
        for (size_t i = 0; i < n; ++i) {
            xxx_i32hashtable_insert(&hs, i);
        }
        benchmark::DoNotOptimize(xxx_i32hashtable_bucket_count(&hs));
    }
    xxx_i32hashtable_deinit(&hs);
}
BENCHMARK(BM_xxx_i32hashtable_insert)->RangeMultiplier(8)->Range(1, 1 << 16);

}
