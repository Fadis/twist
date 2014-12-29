#ifndef TWIST_FUSION_GET_BEGIN_SEQUENCE_HPP
#define TWIST_FUSION_GET_BEGIN_SEQUENCE_HPP

#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/algorithm/auxiliary/copy.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator.hpp>
#include <twist/range/begin.hpp>
#include <twist/fusion/get_vector_of.hpp>

namespace twist {
  namespace fusion {
    namespace result_of {
      template< typename T >
      struct get_begin {
        typedef typename twist::range::result_of::begin<
            T
        >::type type;
      };
      template< typename T >
      struct get_begin_sequence {
        typedef typename twist::fusion::get_vector_of<
          typename boost::mpl::transform<
            T, get_begin< boost::mpl::_1 >
          >::type
        >::type type;
      };
    }
    struct get_begin {
      template< typename T >
      struct result;
      template< typename T >
      struct result< get_begin( T ) > {
        typedef typename result_of::get_begin< T >::type type;
      };
      template< typename T >
      typename result_of::get_begin< T >::type
      operator()( const T &value ) const {
        return twist::range::begin( value );
      }
    };
    template< typename ... Ranges >
    struct get_begin_sequence {
    };
    template< typename Head, typename ...Tail >
    struct get_begin_sequence< Head, Tail... > {
      typedef typename boost::fusion::result_of::push_front<
        const typename get_begin_sequence< Tail... >::result_type,
        typename twist::range::result_of::begin<
          Head
        >::type
      >::type result_type; 
      result_type operator()( Head &head, Tail & ... tail ) const {
        return boost::fusion::push_front( get_begin_sequence< Tail... >()( tail... ), twist::range::begin( head ) );
      }
    };
    template<>
    struct get_begin_sequence<> {
      typedef boost::fusion::vector<> result_type;
      result_type operator()() const {
        return boost::fusion::vector<>();
      };
    };
  }
}

#endif

