#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

// #define TBB_PREVIEW_MEMORY_POOL 1
// #include <tbb/memory_pool.h>
#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>
#include <rmsd.hpp>

template <typename Policy>
static void RMSD(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimension
    double r{};
    for (auto _ : st)
    {
        std::vector<double> A(st.range() * d);
        std::vector<double> B(st.range() * d);

        generate_randoms(ep, A.begin(), B.begin(), st.range(), d);

        r = rmsd(ep, A.begin(), B.begin(), st.range(), d);
        benchmark::DoNotOptimize(r);
    }
    // std::cout << r << std::endl;
}

BENCHMARK_CAPTURE(RMSD, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

#ifndef _MSC_VER
BENCHMARK_CAPTURE(RMSD, unseq, std::execution::unseq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);
#endif  //_MSC_VER

BENCHMARK_CAPTURE(RMSD, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(RMSD, par_unseq, std::execution::par_unseq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void RMSD_TBBSA(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimension
    for (auto _ : st)
    {
        std::vector<float, tbb::scalable_allocator<float>> A(st.range() * d);
        std::vector<float, tbb::scalable_allocator<float>> B(st.range() * d);
        // std::vector<float, tbb::cache_aligned_allocator<float>> A(st.range() * d);
        // std::vector<float, tbb::cache_aligned_allocator<float>> B(st.range() * d);

        generate_randoms(ep, A.begin(), B.begin(), st.range(), d);

        float r = rmsd(ep, A.begin(), B.begin(), st.range(), d);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_CAPTURE(RMSD_TBBSA, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

#ifndef _MSC_VER
BENCHMARK_CAPTURE(RMSD_TBBSA, unseq, std::execution::unseq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);
#endif  //_MSC_VER

BENCHMARK_CAPTURE(RMSD_TBBSA, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(RMSD_TBBSA, par_unseq, std::execution::par_unseq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
