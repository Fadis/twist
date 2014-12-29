#ifndef TWIST_ITERATOR_ADAPTOR_CONCAT
#define TWIST_ITERATOR_ADAPTOR_CONCAT

#include <iterator>
#include <boost/optional.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/empty.hpp>
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <twist/iterator/traversal.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      struct concatenated_tag {};

      template<
        typename BaseIterator,
        bool check_empty
      > class concatenated {};
      
      template< typename BaseIterator >
      struct concatenated_primary_reference_type {
        typedef typename boost::iterator_reference<
          typename boost::remove_reference<
            BaseIterator
          >::type
        >::type type;
      };

      template< typename BaseIterator >
      struct concatenated_secondary_reference_type {
        typedef typename boost::mpl::if_<
          boost::is_reference<
            typename concatenated_primary_reference_type< BaseIterator >::type
          >,
          typename boost::range_reference<
            typename boost::remove_reference<
              typename concatenated_primary_reference_type< BaseIterator >::type
            >::type
          >::type,
          typename boost::range_value<
            typename boost::remove_reference<
              typename concatenated_primary_reference_type< BaseIterator >::type
            >::type
          >::type
        >::type type;
      };

      template< typename BaseIterator >
      struct concatenated_secondary_value_type {
        typedef typename boost::mpl::if_<
          boost::is_reference<
            typename concatenated_primary_reference_type< BaseIterator >::type
          >,
          typename boost::range_value<
            typename boost::remove_reference<
              typename concatenated_primary_reference_type< BaseIterator >::type
            >::type
          >::type,
          typename boost::range_value<
            typename boost::remove_reference<
              typename concatenated_primary_reference_type< BaseIterator >::type
            >::type
          >::type
        >::type type;
      };

      template< typename BaseIterator, bool check_empty, typename Enable = void >
      struct concatenated_base_type {
        typedef boost::iterator_adaptor<
          concatenated< BaseIterator, check_empty >,
          BaseIterator,
          typename concatenated_secondary_value_type< BaseIterator >::type,
          typename twist::iterator::get_pure_traversal<
            typename boost::iterator_traversal< BaseIterator >::type
          >::type,
          typename concatenated_secondary_reference_type< BaseIterator >::type
        > type;
      };
      template< typename BaseIterator, bool check_empty >
      struct concatenated_base_type<
        BaseIterator, check_empty,
        typename boost::enable_if<
          twist::iterator::is_random_access_traversal<
            BaseIterator
          >
        >::type
      > {
        typedef boost::iterator_adaptor<
          concatenated< BaseIterator, check_empty >,
          BaseIterator,
          typename concatenated_secondary_value_type< BaseIterator >::type,
          boost::bidirectional_traversal_tag,
          typename concatenated_secondary_reference_type< BaseIterator >::type
        > type;
      };

      namespace detail {
        template< typename BaseIterator >
        void drift_forward( BaseIterator &iter, const BaseIterator &end ) {
          while(
            iter != end &&
            boost::empty( *iter )
          ) ++iter;
        }
        template< typename BaseIterator >
        void drift_backward( BaseIterator &iter, const BaseIterator &begin ) {
          while(
            iter != begin &&
            boost::empty( *iter )
          ) --iter;
        }
        template< typename BaseIterator >
        typename concatenated_secondary_reference_type< BaseIterator >::type
        dereference( const BaseIterator &iter, size_t index ) {
          return iter->at( index );
        }
        template< typename BaseIterator >
        void increment( BaseIterator &iter, const BaseIterator &end, size_t &index ) {
          ++index;
          if( index == boost::distance( *iter ) ) {
            ++iter;
            drift_forward( iter, end );
            index = 0;
          }
        }
        template< typename BaseIterator >
        void decrement( BaseIterator &iter, const BaseIterator &begin, size_t &index ) {
          if( index == 0 ) {
            --iter;
            drift_backward( iter, begin );
            index = boost::distance( *iter );
          }
          --index;
        }
        template< typename BaseIterator >
        bool equal( const BaseIterator &x_iter, size_t x_index, const BaseIterator &y_iter, size_t y_index ) {
          return y_iter == x_iter && y_index == x_index;
        }
      }

      template< typename BaseIterator >
      class concatenated<
        BaseIterator, true
      > :
        public concatenated_base_type< BaseIterator, true >::type {
      public:
        typedef concatenated< BaseIterator, true > self_type;
        typedef typename concatenated_primary_reference_type< BaseIterator >::type primary_reference_type;
        typedef typename concatenated_secondary_reference_type< BaseIterator >::type secondary_reference_type;
        typedef typename concatenated_secondary_value_type< BaseIterator >::type secondary_value_type;
        typedef typename concatenated_base_type< BaseIterator, true >::type base_type;
        concatenated() : index( 0u ) {}
        concatenated( const BaseIterator &iter, const BaseIterator &end ) : base_type( iter ), begin( iter ), end( end ), index( 0u ) {
          detail::drift_forward( base_type::base_reference(), end );
        }
        secondary_reference_type dereference() const {
          return base_type::base_reference()->at( index );
        }
        void increment() {
          ++index;
          if( index == boost::distance( *base_type::base_reference() ) ) {
            ++base_type::base_reference();
            detail::drift_forward( base_type::base_reference(), end );
            index = 0;
          }
        }
        void decrement() {
          if( index == 0 ) {
            --base_type::base_reference();
            detail::drift_backward( base_type::base_reference(), begin );
            index = boost::distance( *base_type::base_reference() );
          }
          --index;
        }
        bool equal( const self_type &y ) const {
          return base_type::base_reference() == y.base_reference() && index == y.index;
        }
      private:
        const BaseIterator begin;
        const BaseIterator end;
        ptrdiff_t index;
      };
      template< typename BaseIterator >
      class concatenated<
        BaseIterator, false
      > :
        public concatenated_base_type< BaseIterator, false >::type {
      public:
        typedef concatenated< BaseIterator, false > self_type;
        typedef typename concatenated_primary_reference_type< BaseIterator >::type primary_reference_type;
        typedef typename concatenated_secondary_reference_type< BaseIterator >::type secondary_reference_type;
        typedef typename concatenated_secondary_value_type< BaseIterator >::type secondary_value_type;
        typedef typename concatenated_base_type< BaseIterator, false >::type base_type;
        concatenated() : index( 0u ) {}
        concatenated( const BaseIterator &iter ) : base_type( iter ), index( 0u ) {}
        secondary_reference_type dereference() const {
          return base_type::base_reference()->at( index );
        }
        void increment() {
          ++index;
          if( index == boost::distance( *base_type::base_reference() ) ) {
            ++base_type::base_reference();
            index = 0;
          }
        }
        void decrement() {
          if( index == 0 ) {
            --base_type::base_reference();
            index = boost::distance( *base_type::base_reference() );
          }
          --index;
        }
        bool equal( const self_type &y ) const {
          return base_type::base_reference() == y.base_reference() && index == y.index;
        }
      private:
        ptrdiff_t index;
      };
      template< typename BaseIterator >
      concatenated< BaseIterator, true > concat( const BaseIterator &base, const BaseIterator end ) {
        return concatenated< BaseIterator, true >( base, end );
      }
      template< typename BaseIterator >
      concatenated< BaseIterator, false > concat( const BaseIterator &base ) {
        return concatenated< BaseIterator, false >( base );
      }
    }
  }
}

#endif

