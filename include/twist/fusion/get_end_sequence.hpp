#ifndef TWIST_FUSION_GET_END_SEQUENCE_HPP
#define TWIST_FUSION_GET_END_SEQUENCE_HPP

#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/algorithm/auxiliary/copy.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator.hpp>
#include <twist/range/end.hpp>
#include <twist/fusion/get_vector_of.hpp>

namespace twist {
  namespace fusion {
    namespace result_of {
      template< typename T >
      struct get_end {
        typedef typename twist::range::result_of::end<
            T
        >::type type;
      };
      template< typename T >
      struct get_end_sequence {
        typedef typename twist::fusion::get_vector_of<
          typename boost::mpl::transform<
            T, get_end< boost::mpl::_1 >
          >::type
        >::type type;
      };
    }
    struct get_end {
      template< typename T >
      struct result;
      template< typename T >
      struct result< get_end( T ) > {
        typedef typename result_of::get_end< T >::type type;
      };
      template< typename T >
      typename result_of::get_end< T >::type
      operator()( const T &value ) const {
        return twist::range::end( value );
      }
    };
    template< typename ... Ranges >
    struct get_end_sequence {
    };
    template< typename Head, typename ...Tail >
    struct get_end_sequence< Head, Tail... > {
      typedef typename boost::fusion::result_of::push_front<
        const typename get_end_sequence< Tail... >::result_type,
        typename twist::range::result_of::end<
          Head
        >::type
      >::type result_type; 
      result_type operator()( Head &head, Tail & ... tail ) const {
        return boost::fusion::push_front( get_end_sequence< Tail... >()( tail... ), twist::range::end( head ) );
      }
    };
    template<>
    struct get_end_sequence<> {
      typedef boost::fusion::vector<> result_type;
      result_type operator()() const {
        return boost::fusion::vector<>();
      };
    };
  }
}

#endif

