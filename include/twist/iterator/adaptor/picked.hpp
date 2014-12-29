#ifndef TWIST_ITERATOR_ADAPTOR_PICKED_HPP
#define TWIST_ITERATOR_ADAPTOR_PICKED_HPP

#include <iterator>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Index,
        typename Enable = void
      > class picked {};

      template< typename BaseIterator, typename Index >
      struct picked_value_type {
        typedef typename boost::mpl::at<
          typename boost::iterator_value< BaseIterator >::type,
          Index
        >::type type;
      };
      
      template< typename BaseIterator, typename Index >
      struct picked_base_type {
        typedef boost::iterator_adaptor<
          picked< BaseIterator, Index >,
          BaseIterator,
          typename picked_value_type< BaseIterator, Index >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename picked_value_type< BaseIterator, Index >::type
        > type;
      };
      
      template< typename BaseIterator, typename Index >
      class picked< BaseIterator, Index > :
        public picked_base_type< BaseIterator, Index >::type {
      public:
        typedef picked< BaseIterator, Index > self_type;
        typedef typename picked_value_type< BaseIterator, Index >::type value_type;
        typedef typename picked_base_type< BaseIterator, Index >::type base_type;
        picked() {}
        explicit picked( const BaseIterator &iters ) : base_type( iters ) {}
        value_type dereference() const {
          return boost::fusion::at< Index >( *base_type::base_reference() );
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
      template< typename Index, typename BaseIterator >
      picked< BaseIterator, Index > line( const BaseIterator &base ) {
        picked< BaseIterator, Index > temp( base );
        return temp;
      }
    }
  }
}

#endif

