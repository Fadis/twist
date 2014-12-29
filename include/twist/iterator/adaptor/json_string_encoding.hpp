#ifndef TWIST_ITERATOR_ADAPTOR_JSON_STRING_ENCODING_HPP
#define TWIST_ITERATOR_ADAPTOR_JSON_STRING_ENCODING_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#ifndef BOOST_SPIRIT_USE_PHOENIX_V3
# define BOOST_SPIRIT_USE_PHOENIX_V3
#endif
#include <boost/spirit/include/karma.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/exceptions.hpp>
#include <twist/iterator/adaptor/concatenated.hpp>
#include <twist/resizable_array/resizable_array.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class json_string_encoding_iterator {};

      template< typename BaseIterator >
      struct json_string_encoding_iterator_value_type {
        typedef resizable_array< char32_t, 12 > type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct json_string_encoding_iterator_base_type {};

      template< typename BaseIterator >
      struct json_string_encoding_iterator_base_type<
        BaseIterator
      > {
        typedef boost::iterator_adaptor<
          json_string_encoding_iterator< BaseIterator >,
          BaseIterator,
          typename json_string_encoding_iterator_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename json_string_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };


      template< typename Iterator >
      typename json_string_encoding_iterator_value_type< Iterator >::type
      get_escaped_json_character( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t value = *iter_;
        namespace karma = boost::spirit::karma;
        typename json_string_encoding_iterator_value_type< Iterator >::type result;
        if( value == U'"' ) {
          result.assign( { U'\\', U'"' } );
          ++iter_;
        }
        else if( value == U'\'' ) {
          result.assign( { U'\\', U'\'' } );
          ++iter_;
        }
        else if( value == U'/' ) {
          result.assign( { U'\\', U'/' } );
          ++iter_;
        }
        else if( value == U'\b' ) {
          result.assign( { U'\\', U'b' } );
          ++iter_;
        }
        else if( value == U'\n' ) {
          result.assign( { U'\\', U'n' } );
          ++iter_;
        }
        else if( value == U'\f' ) {
          result.assign( { U'\\', U'f' } );
          ++iter_;
        }
        else if( value == U'\r' ) {
          result.assign( { U'\\', U'r' } );
          ++iter_;
        }
        else if( value == U'\t' ) {
          result.assign( { U'\\', U't' } );
          ++iter_;
        }
        else if( value <= 0x7F ) {
          result.assign( { char32_t( value ) } );
          ++iter_;
        }
        else if( value <= 0xFFFFul ) {
          karma::generate( std::back_inserter( result ), "\\u" << karma::right_align( 4, '0' )[ karma::hex ], value );
        }
        else if( value <= 0x10FFFFul ) {
          karma::generate(
            std::back_inserter( result ),
            "\\u" << karma::right_align( 4, '0' )[ karma::hex ],
            static_cast< char16_t >( 0xD800|( ( ( value - 0x10000ul ) >> 10 ) & 0x03FFul ) )
          );
          karma::generate(
            std::back_inserter( result ),
            "\\u" << karma::right_align( 4, '0' )[ karma::hex ],
            static_cast< char16_t >( 0xDC00|( value & 0x03FFul ) )
          );
        }
        else
          throw invalid_unicode_character();
        return result;
      }
      
      template< typename BaseIterator >
      class json_string_encoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public json_string_encoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef json_string_encoding_iterator< BaseIterator > self_type;
        typedef typename json_string_encoding_iterator_base_type< BaseIterator >::type base_type;
        json_string_encoding_iterator() {}
        json_string_encoding_iterator( const BaseIterator &iter ) : base_type( iter ) {}
        typename json_string_encoding_iterator_value_type< BaseIterator >::type dereference() const {
          return get_escaped_json_character( base_type::base_reference() );
        }
        void increment() {
          ++base_type::base_reference();
        }
        void decrement() {
          --base_type::base_reference();
        }
        bool equal( const self_type &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
        template< typename T = void >
        void advance( int n ) {
          base_type::base_reference() += n;
        }
        template< typename T = void >
        typename boost::iterator_difference< BaseIterator >::type
        distance_to( const self_type &z ) const {
          return z.base_reference() - base_type::base_reference();
        }
      private:
      };

      template< typename BaseIterator >
      concatenated< json_string_encoding_iterator< BaseIterator >, false > encode_json_string( const BaseIterator &iter ) {
        return concat( json_string_encoding_iterator< BaseIterator >( iter ) );
      }
    }
  }
}

#endif

