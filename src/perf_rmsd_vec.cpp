#include <iostream>
#include <random>

// #include <pstl/algorithm>
// #include <pstl/numeric>
// #include <pstl/execution>

#include <algorithm>
#include <numeric>
#include <execution>

#include <tbb/scalable_allocator.h>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>
#include <rmsd.hpp>

template <typename Policy>
static void RMSD_VEC_float(
    benchmark::State& st
,   Policy ep)
{
    const size_t d = 3; // dimention
    float r{};
    for (auto _ : st)
    {
        std::vector<std::array<float, d>> A(st.range());
        std::vector<std::array<float, d>> B(st.range());

        generate_randoms_array(ep, A.begin(), st.range(), d);
        generate_randoms_array(ep, B.begin(), st.range(), d);

        r = rmsd_array<float>(ep, A.begin(), B.begin(), st.range(), d);
        benchmark::DoNotOptimize(r);
    }
    std::cout << r << std::endl;
}

BENCHMARK_CAPTURE(RMSD_VEC_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(RMSD_VEC_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

template <typename Policy>
static void SA_RMSD_VEC_float(
    benchmark::State& st
,   Policy ep)
{
    const size_t d = 3; // dimention
    for (auto _ : st)
    {
        std::vector<std::array<float, d>, tbb::scalable_allocator<float>> A(st.range());
        std::vector<std::array<float, d>, tbb::scalable_allocator<float>> B(st.range());

        generate_randoms_array(ep, A.begin(), st.range(), d);
        generate_randoms_array(ep, B.begin(), st.range(), d);

        float r = rmsd_array<float>(ep, A.begin(), B.begin(), st.range(), d);
        benchmark::DoNotOptimize(r);
    }
}

BENCHMARK_CAPTURE(SA_RMSD_VEC_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(SA_RMSD_VEC_float, par, std::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
