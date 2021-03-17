#ifndef SA_GENERATE_RANDOMS_HPP
#define SA_GENERATE_RANDOMS_HPP

#include <algorithm>
#include <random>
#include <thread>

#include <tbb/iterators.h>

//----------------------------------------------------------------------------//

template
<
    class ExecutionPolicy
,   class OutputIt
,   class Size
>
inline void generate_randoms(
    ExecutionPolicy&& policy
,   OutputIt out
,   Size n)
{
    typedef typename std::iterator_traits<OutputIt>::value_type T;

    std::hash<std::thread::id> hasher;
    std::uniform_real_distribution<T> dist(T(-1), T(1));
    std::for_each(
        std::forward<ExecutionPolicy>(policy)
    ,   tbb::counting_iterator<Size>(0)
    ,   tbb::counting_iterator<Size>(n)
    ,   [&] (Size i)
    {
        // thread_local std::mt19937 rng(hasher(std::this_thread::get_id()));
        thread_local std::default_random_engine rng(hasher(std::this_thread::get_id()));
        *(out+i) = dist(rng);
    } );
}

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
,   OutputIt second
,   Size1 n
,   Size2 d)
{
    typedef typename std::iterator_traits<OutputIt>::value_type T;

    std::hash<std::thread::id> hasher;
    std::uniform_real_distribution<T> dist1(T(-50), T(50));
    std::uniform_real_distribution<T> dist2(T(-0.1), T(0.1));
    std::for_each(
        std::forward<ExecutionPolicy>(policy)
    ,   tbb::counting_iterator<Size1>(0)
    ,   tbb::counting_iterator<Size1>(n * d)
    ,   [&] (Size1 i)
    {
        // thread_local std::mt19937 rng1(hasher(std::this_thread::get_id()));
        // thread_local std::mt19937 rng2(hasher(std::this_thread::get_id()));
        thread_local std::default_random_engine rng1(hasher(std::this_thread::get_id()));
        thread_local std::default_random_engine rng2(hasher(std::this_thread::get_id()));
        *(first+i) = dist1(rng1);
        *(second+i) = dist2(rng2) + *(first+i);
    } );
}

//----------------------------------------------------------------------------//

#endif  // SA_GENERATE_RANDOM_HPP