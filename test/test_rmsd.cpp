#include <vector>
#include <random>

#include <algorithm>
#include <numeric>
#include <execution>

#define BOOST_TEST_MODULE TestRMSD
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <generate_randoms.hpp>
#include <rmsd.hpp>

BOOST_AUTO_TEST_CASE( RMSD_FLOAT_SEQ )
{
    std::vector<float> A
    {
         2.3f,  3.1f,  4.4f,
        10.7f, 15.6f, 14.9f,
         5.8f, 13.9f, 25.4f
    };

    std::vector<float> B
    {
         2.1f,  3.4f,  4.5f,
        10.3f, 15.8f, 14.1f,
         5.3f, 13.2f, 25.9f
    };

    BOOST_CHECK_CLOSE(
        rmsd(std::execution::seq, A.begin(), B.begin(), 3, 3)
    ,   0.81035
    ,   0.0001);
}

BOOST_AUTO_TEST_CASE( RMSD_FLOAT_PAR_10K )
{
    std::vector<float> A(10000 * 3);
    std::vector<float> B(10000 * 3);

    generate_randoms(std::execution::par, A.begin(), B.begin(), 10000, 3);

    BOOST_CHECK_CLOSE(
        rmsd(std::execution::seq, A.begin(), B.begin(), 10000, 3)
    ,   rmsd(std::execution::par, A.begin(), B.begin(), 10000, 3)
    ,   0.00001);
}

BOOST_AUTO_TEST_CASE( RMSD_FLOAT_PARUNSEQ_10K )
{
    std::vector<float> A(10000 * 3);
    std::vector<float> B(10000 * 3);

    generate_randoms(std::execution::par_unseq, A.begin(), B.begin(), 10000, 3);

    BOOST_CHECK_CLOSE(
        rmsd(std::execution::seq, A.begin(), B.begin(), 10000, 3)
    ,   rmsd(std::execution::par_unseq, A.begin(), B.begin(), 10000, 3)
    ,   0.00001);
}
