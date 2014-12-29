#ifndef TWIST_ITERATOR_ADAPTOR_PURE_TRANSFORMED_HPP
#define TWIST_ITERATOR_ADAPTOR_PURE_TRANSFORMED_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Function,
        typename Enable = void
      > class pure_transformed {};

      template< typename BaseIterator, typename Function >
      struct pure_transformed_value_type {
        typedef decltype( std::declval< Function >()( std::declval< typename boost::iterator_reference< BaseIterator >::type >() ) ) type;
      };

      template< typename BaseIterator, typename Function >
      struct pure_transformed_base_type {
        typedef boost::iterator_adaptor<
          pure_transformed< BaseIterator, Function >,
          BaseIterator,
          typename pure_transformed_value_type< BaseIterator, Function >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename pure_transformed_value_type< BaseIterator, Function >::type
        > type;
      };

      template< typename BaseIterator, typename Function >
      class pure_transformed<
        BaseIterator,
        Function
      > :
        public pure_transformed_base_type< BaseIterator, Function >::type {
      public:
        typedef pure_transformed< BaseIterator, Function > self_type;
        typedef typename pure_transformed_base_type< BaseIterator, Function >::type base_type;
        pure_transformed() {}
        pure_transformed( const BaseIterator &iter, const Function &func_ ) :
          base_type( iter ), func( func_ ) {}
        typename pure_transformed_value_type< BaseIterator, Function >::type dereference() const {
          return func( *base_type::base_reference() );
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
        void advance( int n ) {
          base_type::base_reference() += n;
        }
        typename boost::iterator_difference< BaseIterator >::type
        distance_to( const self_type &z ) const {
          return z.base_reference() - base_type::base_reference();
        }
      private:
        Function func;
      };
      template< typename BaseIterator, typename Function >
      struct pure_transformed_function_type {
        typedef std::function<
          typename pure_transformed_value_type< BaseIterator, Function >::type(
            typename boost::iterator_reference< BaseIterator >::type
          )
        > type;
      };
      template< typename BaseIterator, typename Function >
      struct pure_transformed_iterator_type {
        typedef pure_transformed<
          BaseIterator,
          typename pure_transformed_function_type< BaseIterator, Function >::type
        > type;
      };
      template< typename BaseIterator, typename Function >
      typename pure_transformed_iterator_type< BaseIterator, Function >::type
      pure_transform( const BaseIterator &iter, const Function &func ) {
        return typename pure_transformed_iterator_type< BaseIterator, Function >::type( iter, func );
      }
    }
  }
}

#endif

