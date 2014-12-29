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
#include <twist/range/adaptor/sliced.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_sliced
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::bidirectional_traversal_tag >
> expected_traversals;

BOOST_AUTO_TEST_CASE( sliced_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::sliced,
    std::vector< int >,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( sliced_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< std::vector< int >& >()|twist::range::adaptor::sliced( 3 ) )
    >::type,
    boost::iterator_range< std::vector< int >::iterator >
  >::type value;
  BOOST_CHECK( value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< std::vector< int >& >()|twist::range::adaptor::sliced( 3 ) )
    >::type,
    boost::iterator_range< std::vector< int >::iterator >
  >::type reference;
  BOOST_CHECK( reference::value );
}

BOOST_AUTO_TEST_CASE( sliced_const_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< const std::vector< int >& >()|twist::range::adaptor::sliced( 3 ) )
    >::type,
    boost::iterator_range< std::vector< int >::const_iterator >
  >::type const_value;
  BOOST_CHECK( const_value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< const std::vector< int >& >()|twist::range::adaptor::sliced( 3 ) )
    >::type,
    boost::iterator_range< std::vector< int >::const_iterator >
  >::type const_reference;
  BOOST_CHECK( const_reference::value );
}

BOOST_AUTO_TEST_CASE( sliced ) {
  const std::vector< int > data{{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  }};
  const std::vector< std::vector< int > > expected{{
    std::vector< int >{{ 1, 2, 3 }},
    std::vector< int >{{ 4, 5, 6 }},
    std::vector< int >{{ 7, 8, 9 }},
    std::vector< int >{{ 10, 11 }}
  }};
  const auto sliced_range = data|twist::range::adaptor::sliced( 3 );
  auto iter = sliced_range.begin();
  BOOST_CHECK( boost::equal( expected[ 0 ], *iter ) );
  ++iter;
  BOOST_CHECK( boost::equal( expected[ 1 ], *iter ) );
  ++iter;
  BOOST_CHECK( boost::equal( expected[ 2 ], *iter ) );
  ++iter;
  BOOST_CHECK( boost::equal( expected[ 3 ], *iter ) );
}

