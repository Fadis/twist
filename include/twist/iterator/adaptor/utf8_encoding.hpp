#ifndef TWIST_ITERATOR_ADAPTOR_UTF8_ENCODING_HPP
#define TWIST_ITERATOR_ADAPTOR_UTF8_ENCODING_HPP

#include <array>
#include <iterator>
#include <functional>
#include <boost/optional.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class utf8_encoding_iterator {};

      template< typename BaseIterator >
      struct utf8_encoding_iterator_value_type {
        typedef char type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct utf8_encoding_iterator_base_type {
        typedef boost::iterator_adaptor<
          utf8_encoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf8_encoding_iterator_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename utf8_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf8_encoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf8_encoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf8_encoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf8_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };
      
      struct invalid_unicode_character {};

      template< typename Iterator >
      std::array< char, 4u > ucs4_to_utf8( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t in_ucs4 = *iter_;
        if( in_ucs4 <= 0x7Ful ) {
          return std::array< char, 4u >{{
            static_cast< char >( in_ucs4 ),
            '\0', '\0', '\0'
          }};
        }
        else if( in_ucs4 <= 0x7FFul ) {
          return std::array< char, 4u >{{
            static_cast< char >( 0xC0 | ( in_ucs4 >> 6 ) ),
            static_cast< char >( 0x80 | ( in_ucs4 & 0x3F ) ),
            '\0', '\0'
          }};
        }
        else if( in_ucs4 <= 0xFFFFul ) {
          return std::array< char, 4u >{{
            static_cast< char >( 0xE0 | ( in_ucs4 >> 12 ) ),
            static_cast< char >( 0x80 | ( ( in_ucs4 >> 6 ) & 0x3F ) ),
            static_cast< char >( 0x80 | ( in_ucs4 & 0x3F ) ),
            '\0'
          }};
        }
        else if( in_ucs4 <= 0x1FFFFFul ) {
          return std::array< char, 4u >{{
            static_cast< char >( 0xF0 | ( in_ucs4 >> 18 ) ),
            static_cast< char >( 0x80 | ( ( in_ucs4 >> 12 ) & 0x3F ) ),
            static_cast< char >( 0x80 | ( ( in_ucs4 >> 6 ) & 0x3F ) ),
            static_cast< char >( 0x80 | ( in_ucs4 & 0x3F ) )
          }};
        }
        else
          throw invalid_unicode_character {};
      }

      template< typename Iterator >
      size_t get_size_in_utf8( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t in_ucs4 = *iter_;
        if( in_ucs4 <= 0x7Ful )
          return 1u;
        else if( in_ucs4 <= 0x7FFul )
          return 2u;
        else if( in_ucs4 <= 0xFFFFul )
          return 3u;
        else if( in_ucs4 <= 0x1FFFFFul )
          return 4u;
        else
          throw invalid_unicode_character {};
      }

      template< typename BaseIterator >
      class utf8_encoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public utf8_encoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename utf8_encoding_iterator_base_type< BaseIterator >::type base_type;
        utf8_encoding_iterator() {}
        utf8_encoding_iterator( const BaseIterator &iter ) :
          base_type( iter ), index( 0u ) {}
        typename utf8_encoding_iterator_value_type< BaseIterator >::type dereference() const {
          if( !current_character_length ) {
            const auto in_utf8 = ucs4_to_utf8( base_type::base_reference() );
            return in_utf8[ index ];
          }
          else
            return current_character[ index ];
        }
        void increment() {
          if( !current_character_length ) {
            current_character_length = get_size_in_utf8( base_type::base_reference() );
            current_character = ucs4_to_utf8( base_type::base_reference() );
          }
          ++index;
          if( *current_character_length == index ) {
            ++base_type::base_reference();
            current_character_length = boost::none;
            index = 0u;
          }
        }
        void decrement() {
          if( !current_character_length || 0u == index ) {
            --base_type::base_reference();
            current_character_length = get_size_in_utf8( base_type::base_reference() );
            current_character = ucs4_to_utf8( base_type::base_reference() );
            index = *current_character_length;
          }
          --index;
        }
        bool equal( const utf8_encoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference() && y.index == index;
        }
      private:
        boost::optional< size_t > current_character_length;
        std::array< char, 4u > current_character;
        size_t index;
      };

      template< typename BaseIterator >
      utf8_encoding_iterator< BaseIterator > encode_utf8( const BaseIterator &iter ) {
        return utf8_encoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      utf8_encoding_iterator< BaseIterator > encode_utf8( const BaseIterator &iter, const BaseIterator &end ) {
        return utf8_encoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

