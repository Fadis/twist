#ifndef TWIST_ITERATOR_ADAPTOR_BASE64_ENCODING_HPP
#define TWIST_ITERATOR_ADAPTOR_BASE64_ENCODING_HPP

#include <array>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/optional.hpp>
#include <boost/crc.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/utils/bit_queue.hpp>
#include <twist/base64/encoder.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator, typename Base64Traits,
        typename Enable = void
      > class base64_encoding_iterator {};

      template< typename BaseIterator >
      struct base64_encoding_iterator_value_type {
        typedef char type;
      };

      template< typename BaseIterator, typename Base64Traits, typename Enable = void >
      struct base64_encoding_iterator_base_type {};

      template< typename BaseIterator, typename Base64Traits >
      struct base64_encoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_forward_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_facade<
          base64_encoding_iterator< BaseIterator, Base64Traits >,
          typename base64_encoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Base64Traits >
      struct base64_encoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_bidirectional_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_facade<
          base64_encoding_iterator< BaseIterator, Base64Traits >,
          typename base64_encoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Base64Traits >
      struct base64_encoding_iterator_base_type<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_facade<
          base64_encoding_iterator< BaseIterator, Base64Traits >,
          typename base64_encoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename base64_encoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Base64Traits >
      class base64_encoding_iterator<
        BaseIterator, Base64Traits,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public base64_encoding_iterator_base_type< BaseIterator, Base64Traits >::type {
      public:
        typedef typename base64_encoding_iterator_base_type< BaseIterator, Base64Traits >::type base_type;
        typedef twist::base64::state_machine< Base64Traits, BaseIterator > Fsm;
        base64_encoding_iterator() {}
        base64_encoding_iterator( const BaseIterator &iter, const BaseIterator &end_ ) :
          state_machine( 0u, iter, end_ ) {}
        typename base64_encoding_iterator_value_type< BaseIterator >::type dereference() const {
          return *state_machine;
        }
        void increment() {
          state_machine.process_event( twist::base64::event::next() );
        }
        bool equal( const base64_encoding_iterator< BaseIterator, Base64Traits > &y ) const {
          return state_machine == y.state_machine;
        }
      private:
        boost::msm::back::state_machine< Fsm > state_machine;
      };

      template< typename Base64Traits, typename BaseIterator >
      base64_encoding_iterator< BaseIterator, Base64Traits > encode_base64( const BaseIterator &iter, const BaseIterator &end ) {
        return base64_encoding_iterator< BaseIterator, Base64Traits >( iter, end );
      }
    }
  }
}

#endif

