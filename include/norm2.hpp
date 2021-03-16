#ifndef SA_NORM2_HPP
#define SA_NORM2_HPP

#include <algorithm>
#include <numeric>
#include <cmath>

//----------------------------------------------------------------------------//

template
<
    class T = double
,   class ExecutionPolicy
,   class InputIt
>
inline T norm_2(
    ExecutionPolicy&& policy
,   InputIt first
,   InputIt last)
{
    return std::sqrt(std::reduce(
        std::forward<ExecutionPolicy>(policy)
    ,   first+1
    ,   last
    ,   *first * *first
    ,   [] (T lhs, T rhs) { return lhs + (rhs * rhs); }
    )   );
}

//----------------------------------------------------------------------------//

#endif  // SA_NORM2_HPP