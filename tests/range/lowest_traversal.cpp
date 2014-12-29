#include <vector>
#include <list>
#include <twist/iterator/traversal.hpp>
#include <twist/iterator/lowest_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_concat
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( lowest_traversal ) {
  typedef boost::is_same<
    twist::iterator::lowest_traversal<
      boost::mpl::vector< 
        std::vector< int >::iterator,
        std::list< int >::iterator
      >
    >::type,
    boost::bidirectional_traversal_tag
  > type;
  BOOST_CHECK( type::value );
}

BOOST_AUTO_TEST_CASE( lowest_traversal_tag ) {
  typedef boost::is_same<
    twist::iterator::lowest_traversal_tag<
      boost::mpl::vector<
        boost::random_access_traversal_tag,
        boost::forward_traversal_tag,
        boost::bidirectional_traversal_tag
      >
    >::type,
    boost::forward_traversal_tag
  > type;
  BOOST_CHECK( type::value );
}

