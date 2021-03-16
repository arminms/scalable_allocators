#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

#include <tbb/scalable_allocator.h>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>
#include <norm2.hpp>

template <typename Policy>
static void norm2(
    benchmark::State& st
,   Policy ep)
{
    std::vector<float> A(st.range());

    generate_randoms(ep, A.begin(), st.range());

    for (auto _ : st)
    {   // This code gets timed
        float r = norm_2(ep, A.begin(), A.begin() + st.range());
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_CAPTURE(norm2, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// #ifndef _MSC_VER
// BENCHMARK_CAPTURE(norm2, unseq, std::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);
// #endif  //_MSC_VER

BENCHMARK_CAPTURE(norm2, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(norm2, par_unseq, std::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void norm2_TBBSA(
    benchmark::State& st
,   Policy ep)
{

    std::vector<float, tbb::scalable_allocator<float>> A(st.range());
    // std::vector<float, tbb::cache_aligned_allocator<float>> A(st.range() * d);

    generate_randoms(ep, A.begin(), st.range());

    for (auto _ : st)
    {   // This code gets timed
        float r = norm_2(ep, A.begin(), A.begin() + st.range());
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_CAPTURE(norm2_TBBSA, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// #ifndef _MSC_VER
// BENCHMARK_CAPTURE(norm2_TBBSA, unseq, std::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);
// #endif  //_MSC_VER

BENCHMARK_CAPTURE(norm2_TBBSA, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(norm2_TBBSA, par_unseq, std::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
