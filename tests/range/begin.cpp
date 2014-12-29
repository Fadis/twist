#include <vector>
#include <twist/range/begin.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_begin
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( begin ) {
  typedef boost::is_same<
    decltype( twist::range::begin( std::declval< std::vector< int >& >() ) ),
    std::vector< int >::iterator
  > type;
  BOOST_CHECK( type::value );
}

BOOST_AUTO_TEST_CASE( const_begin ) {
  typedef boost::is_same<
    decltype( twist::range::begin( std::declval< const std::vector< int >& >() ) ),
    std::vector< int >::const_iterator
  > type;
  BOOST_CHECK( type::value );
}

