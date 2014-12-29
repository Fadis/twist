#ifndef TWIST_ITERATOR_ADAPTOR_DOWNGRADED_HPP
#define TWIST_ITERATOR_ADAPTOR_DOWNGRADED_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/iterator/lowest_traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Traversal,
        typename Enable = void
      > class downgraded {};

      template< typename BaseIterator >
      struct downgraded_value_type {
        typedef typename boost::iterator_value< BaseIterator >::type type;
      };
      
      template< typename BaseIterator >
      struct downgraded_reference_type {
        typedef typename boost::iterator_reference< BaseIterator >::type type;
      };

      template< typename BaseIterator, typename Traversal, typename Enable = void >
      struct downgraded_base_type {
        typedef boost::iterator_adaptor<
          downgraded< BaseIterator, Traversal >,
          BaseIterator,
          typename downgraded_value_type< BaseIterator >::type,
          typename lowest_traversal_tag<
            boost::mpl::vector<
              Traversal,
              typename boost::iterator_traversal< BaseIterator >::type
            >
          >::type,
          typename boost::iterator_reference< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator, typename Traversal >
      class downgraded<
        BaseIterator,
        Traversal
      > :
        public downgraded_base_type< BaseIterator, Traversal >::type {
      public:
        typedef typename downgraded_base_type< BaseIterator, Traversal >::type base_type;
        downgraded() {}
        downgraded( const BaseIterator &iter ) :
          base_type( iter ) {}
        typename downgraded_reference_type< BaseIterator >::type dereference() const {
          return *base_type::base_reference();
        }
        void increment() {
          ++base_type::base_reference();
        }
        void decrement() {
          --base_type::base_reference();
        }
        bool equal( const downgraded< BaseIterator, Traversal > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
        void advance( int n ) {
          base_type::base_reference() += n;
        }
        typename boost::iterator_difference< BaseIterator >::type
        distance_to( const downgraded< BaseIterator, Traversal > &z ) const {
          return z.base_reference() - base_type::base_reference();
        }
      };

      template< typename Traversal, typename BaseIterator >
      downgraded< BaseIterator, Traversal > downgrade( const BaseIterator &iter ) {
        return downgraded< BaseIterator, Traversal >( iter );
      }
    }
  }
}

#endif

