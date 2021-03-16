#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

#include <tbb/scalable_allocator.h>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>

template <typename Policy>
static void GR(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimension

    std::vector<float> A(st.range() * d);
    std::vector<float> B(st.range() * d);

    for (auto _ : st)
    {   // This code gets timed
        generate_randoms(ep, A.begin(), B.begin(), st.range(), d);
    }
}

BENCHMARK_CAPTURE(GR, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// #ifndef _MSC_VER
// BENCHMARK_CAPTURE(GR, unseq, std::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);
// #endif  //_MSC_VER

BENCHMARK_CAPTURE(GR, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(GR, par_unseq, std::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void GR_TBBSA(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimension

    std::vector<float, tbb::scalable_allocator<float>> A(st.range() * d);
    std::vector<float, tbb::scalable_allocator<float>> B(st.range() * d);

    for (auto _ : st)
    {   // This code gets timed
        generate_randoms(ep, A.begin(), B.begin(), st.range(), d);
    }
}

BENCHMARK_CAPTURE(GR_TBBSA, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// #ifndef _MSC_VER
// BENCHMARK_CAPTURE(GR_TBBSA, unseq, std::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);
// #endif  //_MSC_VER

BENCHMARK_CAPTURE(GR_TBBSA, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(GR_TBBSA, par_unseq, std::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
