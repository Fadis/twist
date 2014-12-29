#ifndef TWIST_ITERATOR_ADAPTOR_UTF16_ENCODING_HPP
#define TWIST_ITERATOR_ADAPTOR_UTF16_ENCODING_HPP

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
      > class utf16_encoding_iterator {};

      template< typename BaseIterator >
      struct utf16_encoding_iterator_value_type {
        typedef char16_t type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct utf16_encoding_iterator_base_type {
        typedef boost::iterator_adaptor<
          utf16_encoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf16_encoding_iterator_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename utf16_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf16_encoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf16_encoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf16_encoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf16_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };
      
      struct invalid_unicode_character {};

      template< typename Iterator >
      std::array< char16_t, 2u > ucs4_to_utf16( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t in_ucs4 = *iter_;
        if( in_ucs4 <= 0xFFFFul ) {
          if( ( in_ucs4 & 0xFC00 ) == 0xD800 )
            throw invalid_unicode_character {};
          else if( ( in_ucs4 & 0xFC00 ) == 0xD800 )
            throw invalid_unicode_character {};
          return std::array< char16_t, 2u >{{ static_cast< char16_t >( in_ucs4 ), 0 }};
        }
        else if( in_ucs4 <= 0x10FFFFul )
          return std::array< char16_t, 2u >{{
            static_cast< char16_t >( 0xD800|( ( ( in_ucs4 - 0x10000ul ) >> 10 ) & 0x03FFul ) ), 
            static_cast< char16_t >( 0xDC00|( in_ucs4 & 0x03FFul ) )
          }};
        else
          throw invalid_unicode_character {};
      }

      template< typename Iterator >
      size_t get_size_in_utf16( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t in_ucs4 = *iter_;
        if( in_ucs4 <= 0xFFFFul ) {
          if( ( in_ucs4 & 0xFC00 ) == 0xD800 )
            throw invalid_unicode_character {};
          else if( ( in_ucs4 & 0xFC00 ) == 0xD800 )
            throw invalid_unicode_character {};
          return 1u;
        }
        else if( in_ucs4 <= 0x10FFFFul )
          return 2u;
        else
          throw invalid_unicode_character {};
      }

      template< typename BaseIterator >
      class utf16_encoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public utf16_encoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename utf16_encoding_iterator_base_type< BaseIterator >::type base_type;
        utf16_encoding_iterator() {}
        utf16_encoding_iterator( const BaseIterator &iter ) :
          base_type( iter ), index( 0u ) {}
        typename utf16_encoding_iterator_value_type< BaseIterator >::type dereference() const {
          if( !current_character_length ) {
            const auto in_utf16 = ucs4_to_utf16( base_type::base_reference() );
            return in_utf16[ index ];
          }
          else
            return current_character[ index ];
        }
        void increment() {
          if( !current_character_length ) {
            current_character_length = get_size_in_utf16( base_type::base_reference() );
            current_character = ucs4_to_utf16( base_type::base_reference() );
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
            current_character_length = get_size_in_utf16( base_type::base_reference() );
            current_character = ucs4_to_utf16( base_type::base_reference() );
            index = *current_character_length;
          }
          --index;
        }
        bool equal( const utf16_encoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference() && y.index == index;
        }
      private:
        boost::optional< size_t > current_character_length;
        std::array< char16_t, 2u > current_character;
        size_t index;
      };

      template< typename BaseIterator >
      utf16_encoding_iterator< BaseIterator > encode_utf16( const BaseIterator &iter ) {
        return utf16_encoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      utf16_encoding_iterator< BaseIterator > encode_utf16( const BaseIterator &iter, const BaseIterator &end ) {
        return utf16_encoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

