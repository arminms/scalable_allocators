#ifndef SA_RMSD_HPP
#define SA_RMSD_HPP

#include <algorithm>
#include <numeric>
#include <cmath>

#include <tbb/iterators.h>

//----------------------------------------------------------------------------//

template <class T>
inline T sq(T x)
{
    return x*x;
}

//----------------------------------------------------------------------------//

template
<
    class T = double
,   class ExecutionPolicy
,   class InputIt1
,   class InputIt2
,   class Size1
,   class Size2
>
inline T rmsd(
    ExecutionPolicy&& policy
,   InputIt1 first1
,   InputIt2 first2
,   Size1 n
,   Size2 d)
{
    // auto s1 = make_strided_iterator(first1, first1 + n, d);
    // auto s2 = make_strided_iterator(first2, first2 + n, d);
    // auto e  = make_strided_iterator(first1 + n, first1 + n, d);

    T r = std::transform_reduce(
        std::forward<ExecutionPolicy>(policy)
    ,   first1
    ,   first1 + n
    ,   first2
    ,   T(0)
    ,   [] (T lhs, T rhs) -> T { return lhs + rhs; }
    ,   [] (T lhs, T rhs) -> T { return sq(lhs - rhs); }
    );

    // T r = std::reduce(
    //     std::forward<ExecutionPolicy>(policy)
    // ,   tbb::counting_iterator<Size1>(0)
    // ,   tbb::counting_iterator<Size1>(n)
    // ,   T(0)
    // ,   [&first1, &first2, d] (Size1 lhs, Size1 rhs) -> T
    //     {
    //         T r{};
    //         for (size_t i = 0; i < d; ++i)
    //             r += sq(*(first1+n) - *first2++);
    //         return r;
    //     }
    // );

    // T r{};
    // std::for_each(
    //     std::forward<ExecutionPolicy>(policy)
    // ,   tbb::counting_iterator<Size1>(0)
    // ,   tbb::counting_iterator<Size1>(n)
    // ,   [&] (Size1 i)
    //     {
    //         for (size_t i = 0; i < d; ++i)
    //             r += sq( *(first1+i*d) - *(first2+i*d) );
    //     }
    // );

    return std::sqrt(r / n);
}

//----------------------------------------------------------------------------//

#endif  // SA_RMSD_HPP