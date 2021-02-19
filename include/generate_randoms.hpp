#ifndef SA_GENERATE_RANDOMS_HPP
#define SA_GENERATE_RANDOMS_HPP

#include <vector>
#include <algorithm>
#include <random>

#include <tbb/iterators.h>

//----------------------------------------------------------------------------//

template
<
    class ExecutionPolicy
,   class OutputIt
,   class Size1
,   class Size2
>
inline void generate_randoms(
    ExecutionPolicy&& policy
,   OutputIt first
,   Size1 n
,   Size2 d)
{
    typedef typename std::iterator_traits<OutputIt>::value_type T;

    std::vector<std::uniform_real_distribution<T>> dists;
    dists.reserve(d);
    for (auto i = 0; i < d; ++i)
        dists.emplace_back(T(0), T(1));
    std::for_each(
        std::forward<ExecutionPolicy>(policy)
    ,   tbb::counting_iterator<Size1>(0)
    ,   tbb::counting_iterator<Size1>(n * d)
    ,   [&] (Size1 i)
    {
        thread_local std::mt19937 rng;
        *(first+i) = dists[i%d](rng);
    } );
}

//----------------------------------------------------------------------------//

#endif  // SA_GENERATE_RANDOM_HPP