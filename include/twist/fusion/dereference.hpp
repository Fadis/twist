#ifndef TWIST_FUSION_DEREFERENCED_HPP
#define TWIST_FUSION_DEREFERENCED_HPP

#include <vector>
#include <iostream>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/view/transform_view.hpp>
#include <boost/mpl/at.hpp>

namespace twist {
  namespace fusion {
    namespace detail {
      struct dereference {
        template< typename T >
        struct result;
        template< typename T >
        struct result< dereference( T ) > : boost::iterator_value<
          typename boost::remove_reference<
            typename boost::remove_cv< T >::type
          >::type
        > {};
        template< typename T >
        typename boost::iterator_value<
          typename boost::remove_reference<
            typename boost::remove_cv< T >::type
          >::type
        >::type operator()( const T &value ) const {
          return *value;
        }
      };
    }

    namespace result_of {
      template< typename OriginalVector >
      struct dereference {
        typedef typename boost::fusion::result_of::transform< OriginalVector, twist::fusion::detail::dereference >::type type; 
      };
    }

    template< typename OriginalVector >
    typename result_of::dereference< OriginalVector >::type
    dereference( OriginalVector &vec ) {
      return boost::fusion::transform( vec, detail::dereference() );
    }
  }
}

#endif

