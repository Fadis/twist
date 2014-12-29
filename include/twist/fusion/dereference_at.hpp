#ifndef TWIST_FUSION_DEREFERENCE_AT_HPP
#define TWIST_FUSION_DEREFERENCE_AT_HPP

#include <stdexcept>
#include <boost/fusion/container/vector.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/utility/enable_if.hpp>

namespace twist {
  namespace fusion {

#define TWIST_FUSION_DEREFERENCE_AT_CANDIDATE( z, index_, unused ) \
  case index_ : return *boost::fusion::at_c< index_ >( seq );

#define TWIST_FUSION_DEREFERENCE_AT_EACH_LENGTH( z, length, unsed ) \
  template< typename Sequence > \
  typename boost::iterator_reference< \
    typename boost::mpl::at< Sequence, boost::mpl::size_t< 0u > >::type \
  >::type dereference_at( \
    const Sequence &seq, int index, \
    typename boost::enable_if< \
      boost::mpl::equal_to< \
        boost::mpl::size< Sequence >, boost::mpl::size_t< length > \
      > \
    >::type* = 0 \
  ) { \
    switch( index ) { \
      BOOST_PP_REPEAT( length, TWIST_FUSION_DEREFERENCE_AT_CANDIDATE, ) \
      default: throw std::out_of_range( "dereference_at: Out of range." ); \
    } \
  }


BOOST_PP_REPEAT_FROM_TO( 1, FUSION_MAX_VECTOR_SIZE, TWIST_FUSION_DEREFERENCE_AT_EACH_LENGTH, )

  }
}

#endif

