#include <iostream>
#include <random>

// #include <pstl/algorithm>
// #include <pstl/numeric>
// #include <pstl/execution>

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
static void RMSD_float(
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

BENCHMARK_CAPTURE(RMSD_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(RMSD_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(RMSD_float, unseq, pstl::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(RMSD_float, par_unseq, pstl::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void SA_RMSD_float(
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

BENCHMARK_CAPTURE(SA_RMSD_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(SA_RMSD_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(SA_RMSD_float, unseq, pstl::execution::unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

// BENCHMARK_CAPTURE(SA_RMSD_float, par_unseq, pstl::execution::par_unseq)
// ->  RangeMultiplier(2)
// ->  Range(1<<20, 1<<24)
// ->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
