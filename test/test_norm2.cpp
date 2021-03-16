#include <vector>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

#define BOOST_TEST_MODULE TestNORM2
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <generate_randoms.hpp>
#include <norm2.hpp>

BOOST_AUTO_TEST_CASE( NORM2_FLOAT_SEQ )
{
    std::vector<float> lv{ 2.0, 3.0, 4.0, 5.0 };

    BOOST_CHECK_EQUAL(norm_2<int>(std::execution::seq, lv.begin(), lv.end()), 7);
    BOOST_CHECK_CLOSE(norm_2(std::execution::seq, lv.begin(), lv.end()), 7.348469, 0.001);
}

BOOST_AUTO_TEST_CASE( NORM2_FLOAT_PAR_10K )
{
    std::vector<float> A(10000);

    generate_randoms(std::execution::par, A.begin(), 10000);

    BOOST_CHECK_CLOSE(
        norm_2(std::execution::seq, A.begin(), A.begin() + 10000)
    ,   norm_2(std::execution::par, A.begin(), A.begin() + 10000)
    ,   0.00001);
}
