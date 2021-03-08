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
    T r = std::transform_reduce(
        std::forward<ExecutionPolicy>(policy)
    ,   first1
    ,   first1 + n
    ,   first2
    ,   T(0)
    ,   [] (T lhs, T rhs) -> T { return lhs + rhs; }
    ,   [] (T lhs, T rhs) -> T { return sq(lhs - rhs); }
    );

    return std::sqrt(r / n);
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
inline T rmsd_array(
    ExecutionPolicy&& policy
,   InputIt1 first1
,   InputIt2 first2
,   Size1 n
,   Size2 d)
{
    typedef typename std::iterator_traits<InputIt1>::value_type Array;

    Array a = std::transform_reduce(
        std::forward<ExecutionPolicy>(policy)
    ,   first1
    ,   first1 + n
    ,   first2
    ,   Array{}
    ,   [d] (Array lhs, Array rhs) -> Array
        {
            Array r{};
            for (Size2 i = 0; i < d; ++i)
                r[i] = lhs[i] + rhs[i];
            return r;
        }
    ,   [d] (Array lhs, Array rhs) -> Array
        {
            Array r{};
            for (Size2 i = 0; i < d; ++i)
                r[i] = sq(lhs[i] - rhs[i]);
            return r;
        }
    );

    T r{};
    for (Size2 i = 0; i < d; ++i)
        r += a[i]; 

    return std::sqrt(r / n);
}

//----------------------------------------------------------------------------//

#endif  // SA_RMSD_HPP