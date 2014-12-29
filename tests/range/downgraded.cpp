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
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_downgraded
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::single_pass_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::forward_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::single_pass_traversal_tag >
> single_pass_expected_traversals;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::single_pass_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::forward_traversal_tag >
> forward_expected_traversals;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::single_pass_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::bidirectional_traversal_tag >
> bidirectional_expected_traversals;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::single_pass_traversal_tag, boost::single_pass_traversal_tag >,
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::random_access_traversal_tag >
> random_access_expected_traversals;

BOOST_AUTO_TEST_CASE( downgrade_to_single_pass_traversal ) {
  typedef twist::range::is_expected_traversal<
    decltype( twist::range::adaptor::downgraded< boost::single_pass_traversal_tag >() ),
    std::vector< int >,
    single_pass_expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( downgrade_to_forward_traversal ) {
  typedef twist::range::is_expected_traversal<
    decltype( twist::range::adaptor::downgraded< boost::forward_traversal_tag >() ),
    std::vector< int >,
    forward_expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( downgrade_to_bidirectional_traversal ) {
  typedef twist::range::is_expected_traversal<
    decltype( twist::range::adaptor::downgraded< boost::bidirectional_traversal_tag >() ),
    std::vector< int >,
    bidirectional_expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( downgrade_to_random_access_traversal ) {
  typedef twist::range::is_expected_traversal<
    decltype( twist::range::adaptor::downgraded< boost::random_access_traversal_tag >() ),
    std::vector< int >,
    random_access_expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

BOOST_AUTO_TEST_CASE( downgraded_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< std::vector< int >& >()|twist::range::adaptor::downgraded< boost::forward_traversal_tag >() )
    >::type,
    int
  >::type const_value;
  BOOST_CHECK( const_value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< std::vector< int >& >()|twist::range::adaptor::downgraded< boost::forward_traversal_tag >() )
    >::type,
    int&
  >::type const_reference;
  BOOST_CHECK( const_reference::value );
}

BOOST_AUTO_TEST_CASE( downgraded_const_value_type ) {
  typedef boost::is_same<
    boost::range_value<
      decltype( std::declval< const std::vector< int >& >()|twist::range::adaptor::downgraded< boost::forward_traversal_tag >() )
    >::type,
    int
  >::type const_value;
  BOOST_CHECK( const_value::value );
  typedef boost::is_same<
    boost::range_reference<
      decltype( std::declval< const std::vector< int >& >()|twist::range::adaptor::downgraded< boost::forward_traversal_tag >() )
    >::type,
    const int&
  >::type const_reference;
  BOOST_CHECK( const_reference::value );
}

BOOST_AUTO_TEST_CASE( downgraded ) {
  const std::vector< int > data{{ 1, 2, 3 }};
  const std::vector< int > expected{ 1, 2, 3 };
  BOOST_CHECK( boost::equal( expected, data|twist::range::adaptor::downgraded< boost::bidirectional_traversal_tag >() ) );
  const std::vector< int > rexpected{ 3, 2, 1 };
  BOOST_CHECK( boost::equal( rexpected, data|boost::adaptors::reversed|twist::range::adaptor::downgraded< boost::bidirectional_traversal_tag >() ) );
}

