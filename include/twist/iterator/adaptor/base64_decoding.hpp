#ifndef TWIST_ITERATOR_ADAPTOR_BASE64_DECODING_HPP
#define TWIST_ITERATOR_ADAPTOR_BASE64_DECODING_HPP

#include <array>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <boost/utility.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/optional.hpp>
#include <boost/crc.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/utils/bit_queue.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator, typename Base64Traits,
        typename Enable = void
      > class base64_decoding_iterator {};

      template< typename BaseIterator >
      struct base64_decoding_iterator_value_type {
        typedef uint8_t type;
      };

      template< typename BaseIterator, typename Base64Traits, typename Enable = void >
      struct base64_decoding_iterator_base_type {};

      template< typename BaseIterator, typename Base64Traits >
      struct base64_decoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_forward_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          base64_decoding_iterator< BaseIterator, Base64Traits >,
          BaseIterator,
          typename base64_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Base64Traits >
      struct base64_decoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_bidirectional_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          base64_decoding_iterator< BaseIterator, Base64Traits >,
          BaseIterator,
          typename base64_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Base64Traits >
      struct base64_decoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          base64_decoding_iterator< BaseIterator, Base64Traits >,
          BaseIterator,
          typename base64_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      struct unexpected_character {};
      struct unexpected_end {};

      template< typename Base64Traits >
      class base64_decoding_table {
      public:
        base64_decoding_table() {};
        boost::optional< uint8_t > operator()( char value ) const {
          if( uint8_t( value ) >= 128 )
            return boost::none;
          else {
            const auto converted = get_table()[ static_cast< size_t >( value ) ];
            if( converted >= 64u )
              return boost::none;
            else
              return converted;
          }
        }
      private:
        static const std::array< uint8_t, 128u > &get_table() {
          static const auto table = generate_table();
          return table;
        }
        static std::array< uint8_t, 128u >
          generate_table() {
          std::array< uint8_t, 128u > map{{
            64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64, 64,64,64,64,64,64,64,64,
            52,53,54,55,56,57,58,59, 60,61,64,64,64,64,64,64,
            64, 0, 1, 2, 3, 4, 5, 6,  7, 8, 9,10,11,12,13,14,
            15,16,17,18,19,20,21,22, 23,24,25,64,64,64,64,64,
            64,26,27,28,29,30,31,32, 33,34,35,36,37,38,39,40,
            41,42,43,44,45,46,47,48, 49,50,51,64,64,64,64,64,
          }};
          map[ static_cast< size_t >( twist::base64::plus< Base64Traits >::value ) ] = 62;
          map[ static_cast< size_t >( twist::base64::slash< Base64Traits >::value ) ] = 63;
          return map;
        }
      };

      template< typename BaseIterator, typename Base64Traits >
      class base64_decoding_iterator<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public base64_decoding_iterator_base_type< BaseIterator, Base64Traits >::type {
      public:
        typedef typename base64_decoding_iterator_base_type< BaseIterator, Base64Traits >::type base_type;
        base64_decoding_iterator() {}
        base64_decoding_iterator( const BaseIterator &iter, const BaseIterator &end_ ) :
          base_type( iter ), end( end_ ), phase( 0u ) {}
        typename base64_decoding_iterator_value_type< BaseIterator >::type dereference() const {
          return get( base_type::base_reference(), end, phase ); 
        }
        void increment() {
          base_type::base_reference() =  next( base_type::base_reference(), end, phase );
          ++phase;
        }
        bool equal( const base64_decoding_iterator< BaseIterator, Base64Traits > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
      private:
        template< typename Iterator >
        static Iterator find_valid_character( const Iterator &iter, const Iterator &end, bool is_first_character ) {
          static const base64_decoding_table< Base64Traits > table; 
          if( iter == end ) return iter;
          const auto raw = *iter;
          if( raw == '\x0a' || raw == '\x0d' )
            return find_valid_character( boost::next( iter ), end, is_first_character );
          else if( raw == twist::base64::padding< Base64Traits >::value ) {
            if( !is_first_character )
              return end;
            else {
              if( twist::base64::checksum< Base64Traits >::value )
                return end;
              else if( twist::base64::skip_unexpected< Base64Traits >::value )
                return find_valid_character( boost::next( iter ), end, is_first_character );
              else throw unexpected_character();
            }
          }
          else if( table( raw ) )
            return iter;
          else {
            if( twist::base64::skip_unexpected< Base64Traits >::value )
              return find_valid_character( boost::next( iter ), end, is_first_character );
            else throw unexpected_character();
          }
        }
        template< typename Iterator >
        static Iterator next( const Iterator &iter, const Iterator &end, size_t phase ) {
          if( iter == end ) return end;
          const auto f0 = find_valid_character( boost::next( iter ), end, true );
          if( f0 == end ) return end;
          else if( phase % 3 != 2 ) {
            const auto f1 = find_valid_character( boost::next( f0 ), end, false );
            if( f1 == end )
              return end;
            else
              return f0;
          }
          else
            return find_valid_character( boost::next( f0 ), end, false );
        }
        template< typename Iterator >
        static uint8_t get( const Iterator &iter, const Iterator &end, size_t upper_shift, size_t lower_shift ) {
          if( iter == end ) throw std::out_of_range( "out_of_range" );
          const auto f0_iter = find_valid_character( iter, end, true );
          if( f0_iter == end ) throw std::out_of_range( "out_of_range" );
          else {
            static const base64_decoding_table< Base64Traits > table; 
            const auto f0 = table( *f0_iter );
            const auto f1_iter = find_valid_character( boost::next( f0_iter ), end, true );
            if( f1_iter == end )
              throw unexpected_end();
            else {
              const auto f1 = table( *f1_iter );
              return ( *f0 << upper_shift ) | ( *f1 >> lower_shift );
            }
          }
        }
        template< typename Iterator >
        static uint8_t get( const Iterator &iter, const Iterator &end, size_t phase ) {
          static const std::array< size_t, 3u > upper_shift{{ 2, 4, 6 }};
          static const std::array< size_t, 3u > lower_shift{{ 4, 2, 0 }};
          return get( iter, end, upper_shift[ phase % 3 ], lower_shift[ phase % 3 ] );
        }
        BaseIterator end;
        size_t phase;
      };

      template< typename Base64Traits, typename BaseIterator >
      base64_decoding_iterator< BaseIterator, Base64Traits > decode_base64( const BaseIterator &iter ) {
        return base64_decoding_iterator< BaseIterator, Base64Traits >( iter );
      }
      template< typename Base64Traits, typename BaseIterator >
      base64_decoding_iterator< BaseIterator, Base64Traits > decode_base64( const BaseIterator &iter, const BaseIterator &end ) {
        return base64_decoding_iterator< BaseIterator, Base64Traits >( iter, end );
      }
    }
  }
}

#endif

