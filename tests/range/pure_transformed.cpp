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
#include <twist/range/adaptor/downgraded.hpp>
#include <twist/range/adaptor/pure_transformed.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_pure_transformed
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::random_access_traversal_tag >
> expected_traversals;

BOOST_AUTO_TEST_CASE( pure_transformed_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::pure_transform< int(*)( int ) >,
    std::vector< int >,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

const std::vector< int > data{{ 47,83,90,74,29,74,23,89,4,39,83,78,92,78,94,3 }};
const std::vector< int > expected{{ 49,85,92,76,31,76,25,91,6,41,85,80,94,80,96,5 }};

BOOST_AUTO_TEST_CASE( pure_transformed ) {
  BOOST_CHECK_EQUAL( expected.size(), boost::size( data|twist::range::adaptor::pure_transformed( []( int value ){ return value + 2; } ) ) );
  BOOST_CHECK( boost::equal( expected, data|twist::range::adaptor::pure_transformed( []( int value ){ return value + 2; } ) ) );
}

