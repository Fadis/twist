#ifndef TWIST_ITERATOR_ADAPTOR_ZIP
#define TWIST_ITERATOR_ADAPTOR_ZIP

#include <iterator>
#include <boost/optional.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/empty.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/comparison/equal_to.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <twist/iterator/lowest_traversal.hpp>
#include <twist/fusion/dereference.hpp>
#include <twist/fusion/increment.hpp>
#include <twist/fusion/decrement.hpp>
#include <twist/fusion/advance.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterators,
        typename Enable = void
      > class zipped {};

      template< typename BaseIterators >
      struct zipped_value_type {
        typedef typename twist::fusion::result_of::dereference< const BaseIterators >::type type;
      };
      
      template< typename BaseIterators >
      struct zipped_base_type {
        typedef boost::iterator_facade<
          zipped< BaseIterators >,
          typename zipped_value_type< BaseIterators >::type,
          typename lowest_traversal< BaseIterators >::type,
          typename zipped_value_type< BaseIterators >::type
        > type;
      };

      template< typename BaseIterators >
      class zipped< BaseIterators > :
        public zipped_base_type< BaseIterators >::type {
      public:
        typedef zipped< BaseIterators > self_type;
        typedef typename zipped_value_type< BaseIterators >::type value_type;
        typedef typename zipped_base_type< BaseIterators >::type base_type;
        zipped() {}
        zipped( const BaseIterators &iters ) : base( iters ) {}
        value_type dereference() const {
          return twist::fusion::dereference( base );
        }
        void increment() {
          twist::fusion::increment( base );
        }
        void decrement() {
          twist::fusion::decrement( base );
        }
        bool equal( const self_type &y ) const {
          return y.base == base;
        }
        void advance( int distance ) {
          twist::fusion::advance( base, distance );
        }
        typename boost::iterator_difference<
          typename boost::mpl::at_c< BaseIterators, 0 >::type
        >::type
        distance_to( const self_type &z ) const {
          return std::distance(
            boost::fusion::at_c< 0 >( base ),
            boost::fusion::at_c< 0 >( z.base )
          );
        }
      private:
        BaseIterators base;
      };

      template< typename ...Args >
      struct zip_ {
        typedef zipped< boost::fusion::vector< Args... > > result_type;
        result_type operator()( const Args & ... args ) const {
          return result_type( boost::fusion::make_vector( args... ) );
        }
      };
      template< typename ...Args >
      struct zip_< boost::fusion::vector< Args... > > {
        typedef zipped< boost::fusion::vector< Args... > > result_type;
        result_type operator()( const boost::fusion::vector< Args... > &arg ) const {
          return result_type( arg );
        }
      };

      template< typename ...Args >
      typename zip_< Args... >::result_type zip( const Args & ... args ) {
        return zip_< Args... >()( args... );
      }
    }
  }
}

#endif

