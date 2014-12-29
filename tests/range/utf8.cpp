#include <array>
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
#include <twist/range/adaptor/utf8_encoding.hpp>
#include <twist/range/adaptor/utf8_decoding.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_base64
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::bidirectional_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::bidirectional_traversal_tag >
> expected_traversals;

BOOST_AUTO_TEST_CASE( utf8_decoding_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::utf8_decoding,
    std::string,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}
BOOST_AUTO_TEST_CASE( utf8_encoding_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::utf8_encoding,
    std::u32string,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

const std::vector< char > message_in_utf8 {{
 '\x55', '\x54', '\x46', '\x38', '\xe3', '\x81', '\xae', '\xe6', '\x96', '\x87',
 '\xe5', '\xad', '\x97', '\xe5', '\x88', '\x97',
}};

const std::u32string message_in_utf32( U"UTF8の文字列" );

BOOST_AUTO_TEST_CASE( decode_utf8 ) {
  BOOST_CHECK( boost::equal( message_in_utf32, message_in_utf8|twist::range::adaptor::decode_utf8 ) );
}

BOOST_AUTO_TEST_CASE( encode_utf8 ) {
  BOOST_CHECK( boost::equal( message_in_utf8, message_in_utf32|twist::range::adaptor::encode_utf8 ) );
}


