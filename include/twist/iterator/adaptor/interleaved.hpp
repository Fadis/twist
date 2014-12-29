#ifndef TWIST_ITERATOR_ADAPTOR_INTERLEAVED_HPP
#define TWIST_ITERATOR_ADAPTOR_INTERLEAVED_HPP

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/sequence/comparison/equal_to.hpp>
#include <twist/fusion/dereference_at.hpp>
#include <twist/iterator/lowest_traversal.hpp>
#include <twist/fusion/increment.hpp>
#include <twist/fusion/decrement.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterators,
        typename Enable = void
      > class interleaved {};

      template< typename Iterators >
      struct iterator_references :
        public boost::mpl::transform< Iterators, boost::iterator_reference< boost::mpl::_1 > > {};
      template< typename Iterators >
      struct first_reference_type :
        public boost::iterator_reference<
          typename boost::mpl::at<
            Iterators,
            boost::mpl::size_t< 0u >
          >::type
        > {};
      template< typename Iterators >
      struct iterator_values :
        public boost::mpl::transform< Iterators, boost::iterator_value< boost::mpl::_1 > > {};
      template< typename Iterators >
      struct first_value_type :
        public boost::iterator_reference<
          typename boost::mpl::at<
            Iterators,
            boost::mpl::size_t< 0u >
          >::type
        > {};

      template< typename Iterators >
      struct all_iterator_references_are_same :
        public boost::is_same<
          typename boost::mpl::find_if<
            typename iterator_references< Iterators >::type,
            boost::mpl::not_< boost::is_same<
              typename first_reference_type< Iterators >::type,
              boost::mpl::_1
            > >
          >::type,
          typename boost::mpl::end< typename iterator_references< Iterators >::type >::type
        > {};

      template< typename Iterators >
      struct interleaved_reference_type :
        public boost::mpl::if_<
          boost::mpl::and_<
            boost::is_reference< typename first_reference_type< Iterators >::type >,
            all_iterator_references_are_same< Iterators >
          >,
          typename first_reference_type< Iterators >::type,
          typename first_value_type< Iterators >::type
        > {};

      template< typename Iterators >
      struct interleaved_value_type :
        public first_value_type< Iterators > {};

      template< typename BaseIterators >
      struct interleaved_base_type {
        typedef boost::iterator_facade<
          interleaved< BaseIterators >,
          typename interleaved_value_type< BaseIterators >::type,
          typename lowest_traversal< BaseIterators >::type,
          typename interleaved_reference_type< BaseIterators >::type
        > type;
      };

      template< typename BaseIterators >
      class interleaved<
        BaseIterators
      > :
        public interleaved_base_type< BaseIterators >::type {
      public:
        typedef interleaved< BaseIterators > self_type;
        typedef typename interleaved_value_type< BaseIterators >::type value_type;
        typedef typename interleaved_base_type< BaseIterators >::type base_type;
        interleaved() : index( 0 ) {}
        interleaved( const BaseIterators &iters ) : base( iters ), index( 0 ) {}
        value_type dereference() const {
          return twist::fusion::dereference_at( base, index );
        }
        void increment() {
          ++index;
          if( size_t( boost::fusion::size( base ) ) == index ) {
            index = 0;
            twist::fusion::increment( base );
          }
        }
        template< typename T = void >
        void decrement() {
          if( index == 0ul ) {
            index = boost::fusion::size( base );
            twist::fusion::decrement( base );
          }
          --index;
        }
        template< typename T = void >
        bool equal( const self_type &y ) const {
          return y.base == base && y.index == index;
        }
      private:
        BaseIterators base;
        size_t index;
      };

      template< typename ...Args >
      struct interleave_ {
        typedef interleaved< boost::fusion::vector< Args... > > result_type;
        result_type operator()( const Args & ... args ) const {
          return result_type( boost::fusion::make_vector( args... ) );
        }
      };
      template< typename ...Args >
      struct interleave_< boost::fusion::vector< Args... > > {
        typedef interleaved< boost::fusion::vector< Args... > > result_type;
        result_type operator()( const boost::fusion::vector< Args... > &arg ) const {
          return result_type( arg );
        }
      };

      template< typename ...Args >
      typename interleave_< Args... >::result_type interleave( const Args & ... args ) {
        return interleave_< Args... >()( args... );
      }
    }
  }
}

#endif

