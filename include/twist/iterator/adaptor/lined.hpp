#ifndef TWIST_ITERATOR_ADAPTOR_LINED
#define TWIST_ITERATOR_ADAPTOR_LINED

#include <iterator>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/fusion/algorithm/auxiliary/copy.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class lined {};

      template< typename BaseIterator >
      struct lined_value_type {
        typedef boost::array<
          typename boost::mpl::at_c<
            typename boost::iterator_value< BaseIterator >::type,
            0u
          >::type,
          boost::mpl::size<
            typename boost::iterator_value< BaseIterator >::type
          >::value
        > type;
      };
      
      template< typename BaseIterator >
      struct lined_base_type {
        typedef boost::iterator_adaptor<
          lined< BaseIterator >,
          BaseIterator,
          typename lined_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename lined_value_type< BaseIterator >::type
        > type;
      };
      
      template< typename BaseIterator >
      class lined< BaseIterator > :
        public lined_base_type< BaseIterator >::type {
      public:
        typedef lined< BaseIterator > self_type;
        typedef typename lined_value_type< BaseIterator >::type value_type;
        typedef typename lined_base_type< BaseIterator >::type base_type;
        lined() {}
        explicit lined( const BaseIterator &iters ) : base_type( iters ) {}
        value_type dereference() const {
          value_type temp;
          boost::fusion::copy( *base_type::base_reference(), temp );
          return temp;
        }
        void increment() {
          ++base_type::base_reference();
        }
        template< typename T = void >
        void decrement() {
          --base_type::base_reference();
        }
        template< typename T = void >
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
      };
      template< typename BaseIterator >
      lined< BaseIterator > line( const BaseIterator &base ) {
        lined< BaseIterator > temp( base );
        return temp;
      }
    }
  }
}

#endif

