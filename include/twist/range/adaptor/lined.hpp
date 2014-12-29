#ifndef TWIST_RANGE_ADAPTOR_LINED_HPP
#define TWIST_RANGE_ADAPTOR_LINED_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/lined.hpp>
#include <cxxabi.h>
#include <typeinfo>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct lined {
          lined() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::lined<
              typename boost::range_iterator< Range >::type
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::lined<
                typename boost::range_iterator< Range >::type
              >( boost::begin( range ) ),
              twist::iterator::adaptor::lined<
                typename boost::range_iterator< Range >::type
              >( boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::lined<
              typename boost::range_iterator< Range >::type
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::lined<
                typename boost::range_iterator< Range >::type
              >( boost::begin( range ) ),
              twist::iterator::adaptor::lined<
                typename boost::range_iterator< Range >::type
              >( boost::end( range ) )
            );
          }
        };
      }
      const detail::lined lined;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::lined<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::detail::lined &tag ) {
  return tag( range );
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::lined<
    typename boost::range_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::detail::lined &tag ) {
  return tag( range );
}


#endif

