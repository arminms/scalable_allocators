#include <iostream>
#include <random>

#include <pstl/algorithm>
#include <pstl/numeric>
#include <pstl/execution>

#include <benchmark/benchmark.h>

#include <generate_randoms.hpp>
#include <rmsd.hpp>

template <typename Policy>
static void GR_float(
    benchmark::State& st
,   Policy ep)
{
    size_t d = 3; // dimention
    std::mt19937 eng;
    std::vector<float> A(st.range() * d);

    for (auto _ : st)
        generate_randoms(ep, A.begin(), st.range(), d, eng);
}

BENCHMARK_CAPTURE(GR_float, seq, std::execution::seq)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(GR_float, par, pstl::execution::par)
->  RangeMultiplier(2)
->  Range(1<<20, 1<<24)
->  Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
