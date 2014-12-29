#ifndef TWIST_RANGE_ADAPTOR_CONCAT_HPP
#define TWIST_RANGE_ADAPTOR_CONCAT_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/range/adaptor/detached.hpp>
#include <twist/iterator/adaptor/concatenated.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct concat_ {
          concat_(){}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::concatenated<
              typename boost::range_const_iterator< Range >::type, true
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::concat( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::concat( boost::end( range ), boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::concatenated<
              typename boost::range_iterator< Range >::type, true
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::concat( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::concat( boost::end( range ), boost::end( range ) )
            );
          }
        };
      }
      const detail::concat_ concatenated;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::concatenated<
    typename boost::range_const_iterator< Range >::type, true
  >
> operator|( const Range &range, const twist::range::adaptor::detail::concat_ &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::concatenated<
    typename boost::range_iterator< Range >::type, true
  >
> operator|( Range &range, const twist::range::adaptor::detail::concat_ &tag ) {
  return tag( range );
}

#endif

