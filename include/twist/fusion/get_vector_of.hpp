#ifndef TWIST_FUSION_GET_VECTOR_OF_HPP
#define TWIST_FUSION_GET_VECTOR_OF_HPP

#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/vector.hpp>

namespace twist {
  namespace fusion {
    template< typename Cur, typename End, typename ...Elements >
    struct get_vector_of_ {
      typedef typename get_vector_of_<
        typename boost::mpl::next< Cur >::type,
        End,
        Elements...,
        typename boost::mpl::deref< Cur >::type
      >::type type;
    };
    template< typename End, typename ...Elements >
    struct get_vector_of_< End, End, Elements... > {
      typedef boost::fusion::vector< Elements... > type;
    };
    template< typename Sequence >
    struct get_vector_of {
      typedef typename get_vector_of_<
        typename boost::mpl::begin< Sequence >::type,
        typename boost::mpl::end< Sequence >::type
      >::type type;
    };
  }
}

#endif

