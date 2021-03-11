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
        // thread_local std::mt19937 rng;
        thread_local std::default_random_engine rng1(hasher(std::this_thread::get_id()));
        thread_local std::default_random_engine rng2(hasher(std::this_thread::get_id()));
        *(first+i) = dist1(rng1);
        *(second+i) = dist2(rng2) + *(first+i);
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
inline void generate_randoms_array(
    ExecutionPolicy&& policy
,   OutputIt first
,   OutputIt second
,   Size1 n
,   Size2 d)
{
    typedef typename std::iterator_traits<OutputIt>::value_type Array;
    typedef typename Array::value_type T;

    std::hash<std::thread::id> hasher;
    std::uniform_real_distribution<T> dist1(T(-50), T(50));
    std::uniform_real_distribution<T> dist2(T(-0.1), T(0.1));
    std::for_each(
        std::forward<ExecutionPolicy>(policy)
    ,   tbb::counting_iterator<Size1>(0)
    ,   tbb::counting_iterator<Size1>(n * d)
    ,   [&] (Size1 i)
    {
        thread_local std::default_random_engine rng1(hasher(std::this_thread::get_id()));
        thread_local std::default_random_engine rng2(hasher(std::this_thread::get_id()));
        for (Size2 j = 0; j < d; ++j)
        {
            (*(first+i))[j] = dist1(rng1);
            (*(second+i))[j] = (*(first+i))[j] + dist2(rng2);
        }
    } );
}

//----------------------------------------------------------------------------//

#endif  // SA_GENERATE_RANDOM_HPP