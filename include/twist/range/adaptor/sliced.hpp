#ifndef TWIST_RANGE_ADAPTOR_SLICED_HPP
#define TWIST_RANGE_ADAPTOR_SLICED_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/sliced.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::sliced<
            typename boost::range_const_iterator< Range >::type
          >
        > slice( const Range &range, size_t stride ) {
          return boost::make_iterator_range(
            twist::iterator::adaptor::sliced<
              typename boost::range_const_iterator< Range >::type
            >( boost::const_begin( range ), boost::const_end( range ), stride ),
            twist::iterator::adaptor::sliced<
              typename boost::range_const_iterator< Range >::type
            >( boost::const_end( range ), boost::const_end( range ), stride )
          );
        }
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::sliced<
            typename boost::range_iterator< Range >::type
          >
        > slice( Range &range, size_t stride ) {
          return boost::make_iterator_range(
            twist::iterator::adaptor::sliced<
              typename boost::range_iterator< Range >::type
            >( boost::begin( range ), boost::end( range ), stride ),
            twist::iterator::adaptor::sliced<
              typename boost::range_iterator< Range >::type
            >( boost::end( range ), boost::end( range ), stride )
          );
        }
      }
      struct sliced {
        sliced( size_t stride_ ) : stride( stride_ ) {}
        const size_t stride;
      };
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::sliced<
    typename boost::range_const_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::sliced &params ) {
  return twist::range::adaptor::detail::slice( range, params.stride );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::sliced<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::sliced &params ) {
  return twist::range::adaptor::detail::slice( range, params.stride );
}
#endif

