#ifndef TWIST_ITERATOR_ADAPTOR_UTF8_DECODING_HPP
#define TWIST_ITERATOR_ADAPTOR_UTF8_DECODING_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class utf8_decoding_iterator {};

      template< typename BaseIterator >
      struct utf8_decoding_iterator_value_type {
        typedef char32_t type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct utf8_decoding_iterator_base_type {};

      template< typename BaseIterator >
      struct utf8_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_forward_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf8_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf8_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_bidirectional_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf8_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf8_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf8_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf8_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      struct invalid_utf8_sequence {};

      template< typename Iterator >
      char32_t utf8_to_ucs4_without_end( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t first_byte = *iter_;
        char32_t result;
        if( first_byte < 0x80 )
          result = first_byte;
        else if( ( first_byte & 0xE0 ) == 0xC0 && ( first_byte & 0x1F ) ) {
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x1F ) << 6 ) | ( second_byte & 0x3F );
        }
        else if( ( first_byte & 0xF0 ) == 0xE0 && ( first_byte & 0x0F ) ) {
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t third_byte = *iter_;
          if( ( third_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x0F ) << 12 ) | ( ( second_byte & 0x3F ) << 6 ) | ( third_byte & 0x3F );
        }
        else if( ( first_byte & 0xF8 ) == 0xF0 && ( first_byte & 0x07 ) ) {
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t third_byte = *iter_;
          if( ( third_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t fourth_byte = *iter_;
          if( ( fourth_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x0F ) << 18 ) | ( ( second_byte & 0x3F ) << 12 ) |
                 ( ( third_byte & 0x3F ) << 6 ) | ( fourth_byte & 0x3F );
        }
        else
          throw invalid_utf8_sequence();
        if(
          ( result < 0xD800u || result > 0xDFFFu ) &&
          ( result < 0xFDD0u || result > 0xFDEFu ) &&
          ( ( result & 0xFFFFu ) < 0xFFFEu ) &&
          ( result <= 0x10FFFFu )
        )
          return result;
        else
          throw invalid_utf8_sequence();
      }

      template< typename Iterator >
      char32_t utf8_to_ucs4_with_end( const Iterator &iter, const Iterator &end ) {
        const auto range_size = std::distance( iter, end );
        if( range_size < 1 )
          throw invalid_utf8_sequence();
        Iterator iter_ = iter;
        char32_t result;
        const char32_t first_byte = *iter_;
        if( first_byte < 0x80 )
          result = first_byte;
        else if( ( first_byte & 0xE0 ) == 0xC0 && ( first_byte & 0x1F ) ) {
          if( range_size < 2 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x1F ) << 6 ) | ( second_byte & 0x3F );
        }
        else if( ( first_byte & 0xF0 ) == 0xE0 && ( first_byte & 0x0F ) ) {
          if( range_size < 3 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t third_byte = *iter_;
          if( ( third_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x0F ) << 12 ) | ( ( second_byte & 0x3F ) << 6 ) | ( third_byte & 0x3F );
        }
        else if( ( first_byte & 0xF8 ) == 0xF0 && ( first_byte & 0x07 ) ) {
          if( range_size < 4 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t second_byte = *iter_;
          if( ( second_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t third_byte = *iter_;
          if( ( third_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          ++iter_;
          const char32_t fourth_byte = *iter_;
          if( ( fourth_byte & 0xC0 ) != 0x80 )
            throw invalid_utf8_sequence();
          result = ( ( first_byte & 0x0F ) << 18 ) | ( ( second_byte & 0x3F ) << 12 ) |
                 ( ( third_byte & 0x3F ) << 6 ) | ( fourth_byte & 0x3F );
        }
        else
          throw invalid_utf8_sequence();
        if(
          ( result < 0xD800u || result > 0xDFFFu ) &&
          ( result < 0xFDD0u || result > 0xFDEFu ) &&
          ( ( result & 0xFFFFu ) < 0xFFFEu ) &&
          ( result <= 0x10FFFFu )
        )
          return result;
        else
          throw invalid_utf8_sequence();
      }

      template< typename Iterator >
      Iterator next_utf8_code_point_without_end( const Iterator &iter ) {
        const char32_t first_byte = *iter;
        if( first_byte < 0x80 )
          return std::next( iter, 1 );
        else if( ( first_byte & 0xE0 ) == 0xC0 )
          return std::next( iter, 2 );
        else if( ( first_byte & 0xF0 ) == 0xE0 )
          return std::next( iter, 3 );
        else if( ( first_byte & 0xF8 ) == 0xF0 )
          return std::next( iter, 4 );
        else
          throw invalid_utf8_sequence();
      }
      
      template< typename Iterator >
      Iterator next_utf8_code_point_with_end( const Iterator &iter, const Iterator &end ) {
        const char32_t first_byte = *iter;
        const auto range_size = std::distance( iter, end );
        ptrdiff_t distance_to_next;
        if( first_byte < 0x80 )
          distance_to_next = 1;
        else if( ( first_byte & 0xE0 ) == 0xC0 )
          distance_to_next = 2;
        else if( ( first_byte & 0xF0 ) == 0xE0 )
          distance_to_next = 3;
        else if( ( first_byte & 0xF8 ) == 0xF0 )
          distance_to_next = 4;
        else
          throw invalid_utf8_sequence();
        if( distance_to_next > range_size )
          throw invalid_utf8_sequence();
        return std::next( iter, distance_to_next );
      }

      template< typename Iterator >
      Iterator prev_utf8_code_point( const Iterator &iter ) {
        Iterator iter_ = iter;
        do {
          --iter_;
        } while( ( *iter_ & 0xC0 ) == 0x80 );
        return iter_;
      }

      template< typename BaseIterator >
      class utf8_decoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public utf8_decoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename utf8_decoding_iterator_base_type< BaseIterator >::type base_type;
        utf8_decoding_iterator() {}
        utf8_decoding_iterator( const BaseIterator &iter ) :
          base_type( iter ) {
          utf8_to_ucs4 = std::bind( &utf8_to_ucs4_without_end< BaseIterator >, std::placeholders::_1 );
          next_utf8_code_point = std::bind( &next_utf8_code_point_without_end< BaseIterator >, std::placeholders::_1 );
        }
        utf8_decoding_iterator( const BaseIterator &iter, const BaseIterator &end ) :
          base_type( iter ) {
          utf8_to_ucs4 = std::bind( &utf8_to_ucs4_with_end< BaseIterator >, std::placeholders::_1, end );
          next_utf8_code_point = std::bind( &next_utf8_code_point_with_end< BaseIterator >, std::placeholders::_1, end );
        }
        typename utf8_decoding_iterator_value_type< BaseIterator >::type dereference() const {
          return utf8_to_ucs4( base_type::base_reference() );
        }
        void increment() {
          base_type::base_reference() = next_utf8_code_point( base_type::base_reference() );
        }
        void decrement() {
          base_type::base_reference() = prev_utf8_code_point( base_type::base_reference() );
        }
        bool equal( const utf8_decoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
      private:
        std::function< typename utf8_decoding_iterator_value_type< BaseIterator >::type( const BaseIterator& ) >
          utf8_to_ucs4;
        std::function< BaseIterator( const BaseIterator& ) >
          next_utf8_code_point;
      };

      template< typename BaseIterator >
      utf8_decoding_iterator< BaseIterator > decode_utf8( const BaseIterator &iter ) {
        return utf8_decoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      utf8_decoding_iterator< BaseIterator > decode_utf8( const BaseIterator &iter, const BaseIterator &end ) {
        return utf8_decoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

