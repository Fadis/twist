#include <vector>
#include <list>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm/auxiliary/copy.hpp>
#include <twist/fusion/get_begin_sequence.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE get_begin_sequence
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( get_begin_sequence ) {
  const std::vector< int > a{{ 1, 2 }};
  std::vector< int > b{{ 5, 6 }};
  boost::fusion::vector< std::vector< int >::const_iterator, std::vector< int >::const_iterator > temp;
  boost::fusion::copy(
    twist::fusion::get_begin_sequence<
      const std::vector< int >&,
      std::vector< int >&
    >()( a, b ),
    temp
  );
  BOOST_CHECK( boost::fusion::at_c< 0 >( temp ) == a.begin() );
  BOOST_CHECK( boost::fusion::at_c< 1 >( temp ) == b.begin() );
}

