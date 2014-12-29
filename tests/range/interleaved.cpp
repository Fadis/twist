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
#include <twist/range/adaptor/interleaved.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_interleaved
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( interleaved ) {
  const std::vector< int > a{{ 1, 2, 3 }};
  std::list< int > b{{ 6, 7, 8 }};
  std::vector< int > c{{ 11, 12, 13 }};
  const auto range = twist::range::adaptor::interleave( a, b, c );
  const std::vector< int > expected{ 1, 6, 11, 2, 7, 12, 3, 8, 13 };
  BOOST_CHECK( boost::equal( expected, range ) );
  auto d = a|boost::adaptors::reversed;
  auto e = b|boost::adaptors::reversed;
  auto f = c|boost::adaptors::reversed;
  const auto rrange = twist::range::adaptor::interleave( d, e, f );
  const std::vector< int > rexpected{ 3, 8, 13, 2, 7, 12, 1, 6, 11 };
  BOOST_CHECK( boost::equal( rexpected, rrange ) );
}

