#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

// #include <pstl/algorithm>
// #include <pstl/numeric>
// #include <pstl/execution>


#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>

template <typename Policy>
static void GR_float(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimention
    for (auto _ : st)
    {
        std::vector<float> A(st.range() * d);
        generate_randoms(ep, A.begin(), st.range(), d);
    }
}

BENCHMARK_CAPTURE(GR_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(GR_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void SA_GR_float(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimention
    for (auto _ : st)
    {
        std::vector<float, tbb::scalable_allocator<float>> A(st.range() * d);
        generate_randoms(ep, A.begin(), st.range(), d);
    }
}

BENCHMARK_CAPTURE(SA_GR_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(SA_GR_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
