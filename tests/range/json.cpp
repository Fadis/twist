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
#include <twist/range/adaptor/json_decoding.hpp>
#include <twist/range/adaptor/utf8_decoding.hpp>
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

BOOST_AUTO_TEST_CASE( json_decoding_traversal ) {
  typedef twist::range::is_expected_traversal<
    twist::range::adaptor::json_decoding,
    std::string,
    expected_traversals
  > temp;
  BOOST_CHECK( temp::value );
}

const std::u32string json1( U"{\"hoge\":20,\"fuga\":true,\n/* hoge */piyo:\"a\\nb\" , 'foo':true, 'bar': [ null, 0x3, .5, ] } " );
const std::vector< twist::json::json< std::u32string > > expected{{
  twist::json::json< std::u32string >( twist::json::json_event::begin_struct, std::u32string( U"{" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::escaped_string, std::u32string( U"hoge" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::number, std::u32string( U"20" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::escaped_string, std::u32string( U"fuga" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::boolean, std::u32string( U"true" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::comment, std::u32string( U" hoge " ) ),
  twist::json::json< std::u32string >( twist::json::json_event::string, std::u32string( U"piyo" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::escaped_string, std::u32string( U"a\\nb" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::escaped_string, std::u32string( U"foo" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::boolean, std::u32string( U"true" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::escaped_string, std::u32string( U"bar" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::begin_array, std::u32string( U"[" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::null, std::u32string( U"null" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::number, std::u32string( U"0x3" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::number, std::u32string( U".5" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::end_array, std::u32string( U"]" ) ),
  twist::json::json< std::u32string >( twist::json::json_event::end_struct, std::u32string( U"}" ) )
}};

BOOST_AUTO_TEST_CASE( decode_json_string ) {
  auto iter = twist::iterator::adaptor::decode_json( json1.begin() );
  BOOST_CHECK( boost::equal( expected, json1|twist::range::adaptor::decode_json ) );
}

