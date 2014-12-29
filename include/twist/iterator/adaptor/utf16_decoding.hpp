#ifndef TWIST_ITERATOR_ADAPTOR_UTF16_DECODING_HPP
#define TWIST_ITERATOR_ADAPTOR_UTF16_DECODING_HPP

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
      > class utf16_decoding_iterator {};

      template< typename BaseIterator >
      struct utf16_decoding_iterator_value_type {
        typedef char32_t type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct utf16_decoding_iterator_base_type {};

      template< typename BaseIterator >
      struct utf16_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_forward_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf16_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf16_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_bidirectional_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf16_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      struct utf16_decoding_iterator_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          utf16_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename utf16_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      struct invalid_utf16_sequence {};

      template< typename Iterator >
      char32_t utf16_to_ucs4_without_end( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t first_word = *iter_;
        char32_t result;
        if( ( first_word & 0xFC00 ) != 0xD800 )
          result = first_word;
        else {
          ++iter_;
          const char32_t second_word = *iter_;
          if( ( second_word & 0xFC00 ) != 0xDC00 )
            throw invalid_utf16_sequence();
          else
            result = ( ( ( first_word & 0x03FF ) << 10 ) |
                     ( second_word & 0x03FF ) ) + 0x10000;
        }
        if(
          ( result < 0xD800ul || result > 0xDFFFul ) &&
          ( result < 0xFDD0ul || result > 0xFDEFul ) &&
          ( ( result & 0xFFFFul ) < 0xFFFEul )
        )
          return result;
        else
          throw invalid_utf16_sequence();
      }

      template< typename Iterator >
      char32_t utf16_to_ucs4_with_end( const Iterator &iter, const Iterator &end ) {
        const auto range_size = std::distance( iter, end );
        if( range_size < 1 )
          throw invalid_utf16_sequence();
        Iterator iter_ = iter;
        const char32_t first_word = *iter_;
        char32_t result;
        if( ( first_word & 0xFC00 ) != 0xD800 )
          result = first_word;
        else {
          if( range_size < 2 )
            throw invalid_utf16_sequence();
          ++iter_;
          const char32_t second_word = *iter_;
          if( ( second_word & 0xFC00 ) != 0xDC00 )
            throw invalid_utf16_sequence();
          else
            result = ( ( ( first_word & 0x03FF ) << 10 ) |
                     ( second_word & 0x03FF ) ) + 0x10000;
        }
        if(
          ( result < 0xD800ul || result > 0xDFFFul ) &&
          ( result < 0xFDD0ul || result > 0xFDEFul ) &&
          ( ( result & 0xFFFFul ) < 0xFFFEul )
        )
          return result;
        else
          throw invalid_utf16_sequence();
      }
      
      template< typename Iterator >
      Iterator next_utf16_code_point_without_end( const Iterator &iter ) {
        const char32_t first_word = *iter;
        char32_t result;
        if( ( first_word & 0xFC00 ) != 0xD800 )
          return std::next( iter, 1 );
        else
          return std::next( iter, 2 );
      }

      template< typename Iterator >
      Iterator next_utf16_code_point_with_end( const Iterator &iter, const Iterator &end ) {
        const auto range_size = std::distance( iter, end );
        if( range_size < 1 )
          throw invalid_utf16_sequence();
        const char32_t first_word = *iter;
        ptrdiff_t distance_to_next;
        if( ( first_word & 0xFC00 ) != 0xD800 )
          distance_to_next = 1;
        else
          distance_to_next = 2;
        if( distance_to_next > range_size )
          throw invalid_utf16_sequence();
        return std::next( iter, distance_to_next );
      }


      template< typename Iterator >
      Iterator prev_utf16_code_point( const Iterator &iter ) {
        Iterator iter_ = iter;
        do {
          --iter_;
        } while( ( *iter_ & 0xFC00 ) == 0xD800 );
        return iter_;
      }

      template< typename BaseIterator >
      class utf16_decoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public utf16_decoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename utf16_decoding_iterator_base_type< BaseIterator >::type base_type;
        utf16_decoding_iterator() {}
        utf16_decoding_iterator( const BaseIterator &iter ) :
          base_type( iter ) {
          utf16_to_ucs4 = std::bind( &utf16_to_ucs4_without_end< BaseIterator >, std::placeholders::_1 );
          next_utf16_code_point = std::bind( &next_utf16_code_point_without_end< BaseIterator >, std::placeholders::_1 );
        }
        utf16_decoding_iterator( const BaseIterator &iter, const BaseIterator &end ) :
          base_type( iter ) {
          utf16_to_ucs4 = std::bind( &utf16_to_ucs4_with_end< BaseIterator >, std::placeholders::_1, end );
          next_utf16_code_point = std::bind( &next_utf16_code_point_with_end< BaseIterator >, std::placeholders::_1, end );
        }
        typename utf16_decoding_iterator_value_type< BaseIterator >::type dereference() const {
          return utf16_to_ucs4( base_type::base_reference() );
        }
        void increment() {
          base_type::base_reference() = next_utf16_code_point( base_type::base_reference() );
        }
        template< typename T = void >
        void decrement() {
          base_type::base_reference() = prev_utf16_code_point( base_type::base_reference() );
        }
        bool equal( const utf16_decoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
      private:
        std::function< typename utf16_decoding_iterator_value_type< BaseIterator >::type( const BaseIterator& ) >
          utf16_to_ucs4;
        std::function< BaseIterator( const BaseIterator& ) >
          next_utf16_code_point;
      };

      template< typename BaseIterator >
      utf16_decoding_iterator< BaseIterator > decode_utf16( const BaseIterator &iter ) {
        return utf16_decoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      utf16_decoding_iterator< BaseIterator > decode_utf16( const BaseIterator &iter, const BaseIterator &end ) {
        return utf16_decoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

