#include <vector>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/value_type.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/find_if.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/range/adaptor/detached.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_detached
#include <boost/test/unit_test.hpp>
#
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( detached ) {
  const auto range =
    std::vector< int >{{ 1, 2, 3 }}|
    twist::range::adaptor::detached;
  const std::vector< int > expected{ 1, 2, 3 };
  BOOST_CHECK( boost::equal( expected, range ) );
}

BOOST_AUTO_TEST_CASE( detached2 ) {
  const auto range =
    std::vector< int >{{ 1, 2, 3 }}|
    twist::range::adaptor::detached|
    boost::adaptors::reversed;
  const std::vector< int > expected{ 3, 2, 1 };
  BOOST_CHECK( boost::equal( expected, range ) );
}

