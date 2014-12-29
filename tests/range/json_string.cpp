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
#include <twist/range/adaptor/json_string_decoding.hpp>
#include <twist/range/adaptor/json_string_encoding.hpp>
#include <twist/range/adaptor/utf8_encoding.hpp>
#include <twist/range/is_expected_traversal.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_base64
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

typedef boost::mpl::vector<
  boost::mpl::pair< boost::forward_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::bidirectional_traversal_tag, boost::forward_traversal_tag >,
  boost::mpl::pair< boost::random_access_traversal_tag, boost::forward_traversal_tag >
> expected_traversals;

BOOST_AUTO_TEST_CASE( json_string_decoding_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::json_string_decoding,
    std::string,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

const std::u32string message_in_json_string( U"b\\u3042a\n\\ud842\\udfb7う" );
const std::u32string message_in_json_string2( U"b\\u3042a\\n\\ud842\\udfb7\\u3046" );
const std::u32string message_in_utf32( U"bあa\n𠮷う" );

BOOST_AUTO_TEST_CASE( decode_json_string ) {
  BOOST_CHECK( boost::equal( message_in_utf32, message_in_json_string|twist::range::adaptor::decode_json_string ) );
}

BOOST_AUTO_TEST_CASE( encode_json_string ) {
  BOOST_CHECK( boost::equal( message_in_json_string2, message_in_utf32|twist::range::adaptor::encode_json_string ) );
}

