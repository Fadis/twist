#ifndef TWIST_ITERATOR_ADAPTOR_SLICED_HPP
#define TWIST_ITERATOR_ADAPTOR_SLICED_HPP

#include <iterator>
#include <stdexcept>
#include <boost/optional.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/empty.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/utility.hpp>
#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
      typename BaseIterator,
        typename Enable = void
      > class sliced {};


      template< typename BaseIterator >
      struct sliced_value_type {
        typedef boost::iterator_range< BaseIterator > type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct sliced_base_type {
        typedef boost::iterator_adaptor<
          sliced< BaseIterator >,
          BaseIterator,
          typename sliced_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename sliced_value_type< BaseIterator >::type
        > type;
      };
      template< typename BaseIterator >
      struct sliced_base_type<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          sliced< BaseIterator >,
          BaseIterator,
          typename sliced_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename sliced_value_type< BaseIterator >::type
        > type;
      };

      template< typename BaseIterator >
      class sliced<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public sliced_base_type< BaseIterator >::type {
      public:
        typedef typename sliced_base_type< BaseIterator >::type base_type;
        sliced() {}
        sliced( const BaseIterator &iter, size_t interval_ ) :
          base_type( iter ), interval( interval_ ), tail_size( 0 ) {}
        sliced( const BaseIterator &iter, const BaseIterator &end_, size_t interval_ ) :
          base_type( iter ), end( end_ ), interval( interval_ ), tail_size( 0 ) {
          const ptrdiff_t distance_to_end = std::distance( base_type::base_reference(), *end );
          final_checkpoint = boost::next( iter, distance_to_end - ( distance_to_end % interval ) );
        }
        typename sliced_value_type< BaseIterator >::type dereference() const {
          if( end && base_type::base_reference() == final_checkpoint )
            return boost::make_iterator_range( base_type::base_reference(), *end );
          else
            return boost::make_iterator_range( base_type::base_reference(), boost::next( base_type::base_reference(), interval ) );
        }
        void increment() {
          if( end ) {
            if( base_type::base_reference() == *end )
              throw std::out_of_range( "out of range" );
            else if( base_type::base_reference() == final_checkpoint ) {
              base_type::base_reference() = *end;
            }
            else
              std::advance( base_type::base_reference(), interval );
          }
          else
            std::advance( base_type::base_reference(), interval );
        }
        void decrement() {
          if( end ) {
            if( base_type::base_reference() == *end && final_checkpoint != *end )
              base_type::base_reference() = final_checkpoint;
            else
              std::advance( base_type::base_reference(), -interval );
          }
          else
            std::advance( base_type::base_reference(), -interval );
        }
        bool equal( const sliced< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference() && y.interval == interval;
        }
      private:
        boost::optional< BaseIterator > end;
        BaseIterator final_checkpoint;
        ptrdiff_t interval;
        ptrdiff_t tail_size;
      };
    }
  }
}

#endif

