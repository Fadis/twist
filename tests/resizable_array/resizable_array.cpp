#include <twist/resizable_array/resizable_array.hpp>
#ifndef BOOST_SPIRIT_USE_PHOENIX_V3
# define BOOST_SPIRIT_USE_PHOENIX_V3
#endif
#include <boost/spirit/include/karma.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE range_adaptor_base64
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

class not_pod {
public:
  not_pod() : value( 0 ) {}
  not_pod( int value_ ) : value( value_ ) {}
  not_pod( const not_pod& ) = default;
  not_pod( not_pod&& ) = default;
  not_pod &operator=( const not_pod& ) = default;
  not_pod &operator=( not_pod&& ) = default;
  bool operator==( const not_pod &y ) const {
    return value == y.value;
  }
  bool operator!=( const not_pod &y ) const {
    return value != y.value;
  }
  int &operator*() {
    return value;
  }
  const int &operator*() const {
    return value;
  }
private:
  int value;
};

std::ostream &operator<<( std::ostream &stream, const not_pod &value ) {
  stream << *value;
  return stream;
}

BOOST_AUTO_TEST_CASE( resizable_array_max_size ) {
  twist::resizable_array< int, 10 > array;
  BOOST_CHECK_EQUAL( array.max_size(), 10u );
}

BOOST_AUTO_TEST_CASE( resizable_array_capacity ) {
  twist::resizable_array< int, 10 > array;
  BOOST_CHECK_EQUAL( array.capacity(), 10u );
}

BOOST_AUTO_TEST_CASE( resizable_array_empty ) {
  twist::resizable_array< int, 10 > array1;
  BOOST_CHECK( array1.empty() );
  twist::resizable_array< int, 10 > array2{{ 1, 2, 3 }};
  BOOST_CHECK( !array2.empty() );
}

BOOST_AUTO_TEST_CASE( resizable_array_size ) {
  twist::resizable_array< int, 10 > array1;
  BOOST_CHECK_EQUAL( array1.size(), 0u );
  twist::resizable_array< int, 10 > array2{{ 1, 2, 3 }};
  BOOST_CHECK_EQUAL( array2.size(), 3u );
}

BOOST_AUTO_TEST_CASE( resizable_array_front ) {
  twist::resizable_array< int, 10 > array{{ 1, 2, 3 }};
  BOOST_CHECK_EQUAL( array.front(), 1 );
}

BOOST_AUTO_TEST_CASE( resizable_array_back ) {
  twist::resizable_array< int, 10 > array{{ 1, 2, 3 }};
  BOOST_CHECK_EQUAL( array.back(), 3 );
}


BOOST_AUTO_TEST_CASE( resizable_array_at ) {
  twist::resizable_array< int, 10 > array{{ 1, 2, 3 }};
  BOOST_CHECK_EQUAL( array.at( 0 ), 1 );
  BOOST_CHECK_EQUAL( array.at( 1 ), 2 );
  BOOST_CHECK_EQUAL( array.at( 2 ), 3 );
  BOOST_CHECK_THROW( array.at( 4 ), std::out_of_range )
}

BOOST_AUTO_TEST_CASE( resizable_array_subscript ) {
  twist::resizable_array< int, 10 > array{{ 1, 2, 3 }};
  BOOST_CHECK_EQUAL( array[ 0 ], 1 );
  BOOST_CHECK_EQUAL( array[ 1 ], 2 );
  BOOST_CHECK_EQUAL( array[ 2 ], 3 );
}


BOOST_AUTO_TEST_CASE( resizable_array_push_back ) {
  twist::resizable_array< not_pod, 3 > array;
  array.push_back( not_pod( 5 ) );
  BOOST_CHECK_EQUAL( array.size(), 1 );
  not_pod a( 6 );
  array.push_back( a );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  BOOST_CHECK_EQUAL( array.back(), not_pod( 6 ) );
  BOOST_CHECK_EQUAL( array.front(), not_pod( 5 ) );
}
BOOST_AUTO_TEST_CASE( resizable_array_pop_back ) {
  twist::resizable_array< not_pod, 3 > array{{ not_pod( 7 ), not_pod( 8 ) }};
  BOOST_CHECK_EQUAL( array.back(), not_pod( 8 ) );
  array.pop_back();
  BOOST_CHECK_EQUAL( array.back(), not_pod( 7 ) );
  BOOST_CHECK_EQUAL( array.size(), 1 );
  array.pop_back();
  BOOST_CHECK_EQUAL( array.size(), 0 );
}

BOOST_AUTO_TEST_CASE( resizable_array_iterator ) {
  twist::resizable_array< int, 3 > array{{ 1, 2, 3 }};
  static const std::array< int, 3 > expected{{ 1, 2, 3 }};
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
}

BOOST_AUTO_TEST_CASE( resizable_array_const_iterator ) {
  static const twist::resizable_array< int, 3 > array{{ 1, 2, 3 }};
  static const std::array< int, 3 > expected{{ 1, 2, 3 }};
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK( std::equal( array.cbegin(), array.cend(), expected.begin() ) );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert1 ) {
  twist::resizable_array< int, 6 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 6 > expected{{ 5, 1, 1, 6, 7, 8 }};
  const auto result = array.insert( std::next( array.begin() ), 2, 1 );
  BOOST_CHECK_EQUAL( array.size(), 6 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, 1 );
  BOOST_CHECK_THROW( array.insert( array.end(), 1 ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert2 ) {
  twist::resizable_array< int, 7 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 3 > inserted{{ 1, 2, 3 }};
  static const std::array< int, 7 > expected{{ 5, 1, 2, 3, 6, 7, 8 }};
  const auto result = array.insert( std::next( array.begin() ), inserted.begin(), inserted.end() );
  BOOST_CHECK_EQUAL( array.size(), 7 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, 1 );
  BOOST_CHECK_THROW( array.insert( array.end(), 1 ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert3 ) {
  twist::resizable_array< int, 7 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 7 > expected{{ 5, 1, 2, 3, 6, 7, 8 }};
  const auto result = array.insert( std::next( array.begin() ), { 1, 2, 3 } );
  BOOST_CHECK_EQUAL( array.size(), 7 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, 1 );
  BOOST_CHECK_THROW( array.insert( array.end(), 1 ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert4 ) {
  twist::resizable_array< not_pod, 6 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 6 > expected{{ not_pod( 5 ), not_pod( 1 ), not_pod( 1 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  const auto result = array.insert( std::next( array.begin() ), 2, not_pod( 1 ) );
  BOOST_CHECK_EQUAL( array.size(), 6 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, not_pod( 1 ) );
  BOOST_CHECK_THROW( array.insert( array.end(), not_pod( 1 ) ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert5 ) {
  twist::resizable_array< not_pod, 7 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 3 > inserted{{ not_pod( 1 ), not_pod( 2 ), not_pod( 3 ) }};
  static const std::array< not_pod, 7 > expected{{ not_pod( 5 ), not_pod( 1 ), not_pod( 2 ), not_pod( 3 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  const auto result = array.insert( std::next( array.begin() ), inserted.begin(), inserted.end() );
  BOOST_CHECK_EQUAL( array.size(), 7 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, not_pod( 1 ) );
  BOOST_CHECK_THROW( array.insert( array.end(), not_pod( 1 ) ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_insert6 ) {
  twist::resizable_array< not_pod, 7 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 7 > expected{{ not_pod( 5 ), not_pod( 1 ), not_pod( 2 ), not_pod( 3 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  const auto result = array.insert( std::next( array.begin() ), { not_pod( 1 ), not_pod( 2 ), not_pod( 3 ) } );
  BOOST_CHECK_EQUAL( array.size(), 7 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, not_pod( 1 ) );
  BOOST_CHECK_THROW( array.insert( array.end(), not_pod( 1 ) ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_erase1 ) {
  twist::resizable_array< int, 10 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 3 > expected{{ 5, 7, 8 }};
  const auto result = array.erase( std::next( array.begin() ) );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, 7 );
}

BOOST_AUTO_TEST_CASE( resizable_array_erase2 ) {
  twist::resizable_array< int, 10 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 2 > expected{{ 5, 8 }};
  const auto result = array.erase( std::next( array.begin() ), std::next( array.begin(), 3 ) );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, 8 );
}

BOOST_AUTO_TEST_CASE( resizable_array_erase3 ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 3 > expected{{ not_pod( 5 ), not_pod( 7 ), not_pod( 8 ) }};
  const auto result = array.erase( std::next( array.begin() ) );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, not_pod( 7 ) );
}

BOOST_AUTO_TEST_CASE( resizable_array_erase4 ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 2 > expected{{ not_pod( 5 ), not_pod( 8 ) }};
  const auto result = array.erase( std::next( array.begin() ), std::next( array.begin(), 3 ) );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_EQUAL( *result, not_pod( 8 ) );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign1 ) {
  twist::resizable_array< int, 10 > array{{ 5, 6, 7, 8 }};
  static const std::array< int,2 > expected{{ 1, 1 }};
  array.assign( 2, 1 );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_THROW( array.assign( 11, 1 ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign2 ) {
  twist::resizable_array< int, 10 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 3 > inserted{{ 1, 2, 3 }};
  array.assign( inserted.begin(), inserted.end() );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  static const std::array< int, 11 > inserted2{{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }};
  BOOST_CHECK_THROW( array.assign( inserted2.begin(), inserted2.end() ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign3 ) {
  twist::resizable_array< int, 10 > array{{ 5, 6, 7, 8 }};
  static const std::array< int, 3 > expected{{ 1, 2, 3 }};
  array.assign( { 1, 2, 3 } );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_THROW( array.assign( { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign4 ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 2 > expected{{ not_pod( 1 ), not_pod( 1 ) }};
  array.assign( 2, not_pod( 1 ) );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_THROW( array.assign( 11, not_pod( 1 ) ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign5 ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 3 > inserted{{ not_pod( 1 ), not_pod( 2 ), not_pod( 3 ) }};
  array.assign( inserted.begin(), inserted.end() );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), inserted.begin() ) );
  static const std::array< not_pod, 11 > inserted2{{ not_pod( 1 ), not_pod( 2 ), not_pod( 3 ), not_pod( 4 ), not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ), not_pod( 9 ), not_pod( 10 ), not_pod( 11 ) }};
  BOOST_CHECK_THROW( array.assign( inserted2.begin(), inserted2.end() ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_assign6 ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 3 > expected{{ not_pod( 1 ), not_pod( 2 ), not_pod( 3 ) }};
  array.assign( { not_pod( 1 ), not_pod( 2 ), not_pod( 3 ) } );
  BOOST_CHECK_EQUAL( array.size(), 3 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  BOOST_CHECK_THROW( array.assign( { not_pod( 1 ), not_pod( 2 ), not_pod( 3 ), not_pod( 4 ), not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ), not_pod( 9 ), not_pod( 10 ), not_pod( 11 ) } ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( resizable_array_emplace ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 5 > expected{{ not_pod( 5 ), not_pod( 1 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  array.emplace( std::next( array.begin() ), 1 );
  BOOST_CHECK_EQUAL( array.size(), 5 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  twist::resizable_array< not_pod, 3 > array2{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ) }};
  BOOST_CHECK_THROW( array2.emplace( std::next( array2.begin() ), 8 ), std::out_of_range )
}

BOOST_AUTO_TEST_CASE( resizable_array_emplace_back ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  static const std::array< not_pod, 5 > expected{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ), not_pod( 1 ) }};
  array.emplace_back( 1 );
  BOOST_CHECK_EQUAL( array.size(), 5 );
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected.begin() ) );
  twist::resizable_array< not_pod, 3 > array2{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ) }};
  BOOST_CHECK_THROW( array2.emplace_back( 8 ), std::out_of_range )
}

BOOST_AUTO_TEST_CASE( resizable_array_resize ) {
  twist::resizable_array< not_pod, 10 > array{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  array.resize( 2 );
  BOOST_CHECK_EQUAL( array.size(), 2 );
  static const std::array< not_pod, 5 > expected1{{ not_pod( 5 ), not_pod( 6 ) }};
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected1.begin() ) );
  array.resize( 7 );
  BOOST_CHECK_EQUAL( array.size(), 7 );
  array.resize( 1 );
  array.resize( 5, not_pod( 9 ) );
  static const std::array< not_pod, 5 > expected2{{ not_pod( 5 ), not_pod( 9 ), not_pod( 9 ), not_pod( 9 ), not_pod( 9 ) }};
  BOOST_CHECK( std::equal( array.begin(), array.end(), expected2.begin() ) );
  BOOST_CHECK_THROW( array.resize( 11 ), std::out_of_range )
}

BOOST_AUTO_TEST_CASE( resizable_array_swap ) {
  twist::resizable_array< not_pod, 10 > array1{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  twist::resizable_array< not_pod, 10 > array2{{ not_pod( 2 ), not_pod( 3 ) }};
  static const std::array< not_pod, 2 > expected1{{ not_pod( 2 ), not_pod( 3 ) }};
  static const std::array< not_pod, 4 > expected2{{ not_pod( 5 ), not_pod( 6 ), not_pod( 7 ), not_pod( 8 ) }};
  array1.swap( array2 );
  BOOST_CHECK( std::equal( array1.begin(), array1.end(), expected1.begin() ) );
  BOOST_CHECK( std::equal( array2.begin(), array2.end(), expected2.begin() ) );
}

BOOST_AUTO_TEST_CASE( resizable_array_compare ) {
  twist::resizable_array< int, 3 > array1{{ 1, 2, 3 }};
  twist::resizable_array< int, 3 > array2{{ 5, 6, 7 }};
  twist::resizable_array< int, 4 > array3{{ 1, 2, 3, 4 }};
  BOOST_CHECK( array1 == array1 );
  BOOST_CHECK( array1 != array2 );
  BOOST_CHECK( array1 != array3 );
  BOOST_CHECK( array2 > array1 );
  BOOST_CHECK( array1 < array2 );
  BOOST_CHECK( array3 > array1 );
  BOOST_CHECK( array1 < array3 );
  BOOST_CHECK( array2 >= array1 );
  BOOST_CHECK( array1 <= array2 );
  BOOST_CHECK( array3 >= array1 );
  BOOST_CHECK( array1 <= array3 );
}

