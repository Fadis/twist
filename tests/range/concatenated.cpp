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
#include <twist/range/adaptor/concatenated.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_concat
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::single_pass_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::bidirectional_traversal_tag >
> expected_traversals;

BOOST_AUTO_TEST_CASE( concat_traversal ) {
  typedef twist::range::is_expected_traversal<
    decltype( twist::range::adaptor::concatenated ),
    std::vector< std::vector< int > >,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( concat_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< std::vector< std::vector< int > >& >()|twist::range::adaptor::concatenated )
    >::type,
    int
  >::type value;
  BOOST_CHECK( value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< std::vector< std::vector< int > >& >()|twist::range::adaptor::concatenated )
    >::type,
    int&
  >::type reference;
  BOOST_CHECK( reference::value );
}

BOOST_AUTO_TEST_CASE( concat_const_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< const std::vector< std::vector< int > >& >()|twist::range::adaptor::concatenated )
    >::type,
    int
  >::type const_value;
  BOOST_CHECK( const_value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< const std::vector< std::vector< int > >& >()|twist::range::adaptor::concatenated )
    >::type,
    const int&
  >::type const_reference;
  BOOST_CHECK( const_reference::value );
}

BOOST_AUTO_TEST_CASE( concat ) {
  const std::vector< std::vector< int > > data{{
    std::vector< int >(),
    std::vector< int >{{ 1, 2, 3 }},
    std::vector< int >{{ 4, 5, 6 }},
    std::vector< int >(),
    std::vector< int >{{ 7, 8, 9, 10 }}
  }};
  const std::vector< int > expected{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  BOOST_CHECK( boost::equal( expected, data|twist::range::adaptor::concatenated ) );
  const std::vector< int > rexpected{ 7, 8, 9, 10, 4, 5, 6, 1, 2, 3 };
  BOOST_CHECK( boost::equal( rexpected, data|boost::adaptors::reversed|twist::range::adaptor::concatenated ) );
}

BOOST_AUTO_TEST_CASE( concat_empty_l2 ) {
  const std::vector< std::vector< int > > data{{
    std::vector< int >(),
    std::vector< int >(),
    std::vector< int >()
  }};
  const std::vector< int > expected{};
  BOOST_CHECK( boost::equal( expected, data|twist::range::adaptor::concatenated ) );
  const std::vector< int > rexpected{};
  BOOST_CHECK( boost::equal( rexpected, data|boost::adaptors::reversed|twist::range::adaptor::concatenated ) );
}

BOOST_AUTO_TEST_CASE( concat_empty_l1 ) {
  const std::vector< std::vector< int > > data{{}};
  const std::vector< int > expected{};
  BOOST_CHECK( boost::equal( expected, data|twist::range::adaptor::concatenated ) );
  const std::vector< int > rexpected{};
  BOOST_CHECK( boost::equal( rexpected, data|boost::adaptors::reversed|twist::range::adaptor::concatenated ) );
}

