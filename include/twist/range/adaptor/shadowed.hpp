#ifndef TWIST_RANGE_ADAPTOR_SHADOWED_HPP
#define TWIST_RANGE_ADAPTOR_SHADOWED_HPP

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/shadowed.hpp>
#include <cxxabi.h>
#include <typeinfo>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct shadow {
          shadow() {}
          template< typename Range >
          boost::iterator_range<
            typename iterator::adaptor::get_shadowed_iterator<
              typename boost::range_iterator< Range >::type
            >::type
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              typename iterator::adaptor::get_shadowed_iterator<
                typename boost::range_iterator< Range >::type
              >::type( boost::begin( range ) ),
              typename iterator::adaptor::get_shadowed_iterator<
                typename boost::range_iterator< Range >::type
              >::type( boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            typename iterator::adaptor::get_shadowed_iterator<
              typename boost::range_const_iterator< Range >::type
            >::type
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              typename iterator::adaptor::get_shadowed_iterator<
                typename boost::range_const_iterator< Range >::type
              >::type( boost::begin( range ) ),
              typename iterator::adaptor::get_shadowed_iterator<
                typename boost::range_const_iterator< Range >::type
              >::type( boost::end( range ) )
            );
          }
        };
      }
      typedef detail::shadow shadow;
      const shadow shadowed;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  typename twist::iterator::adaptor::get_shadowed_iterator<
    typename boost::range_iterator< Range >::type
  >::type
> operator|( Range &range, const twist::range::adaptor::shadow &tag ) {
  return tag( range );
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  typename twist::iterator::adaptor::get_shadowed_iterator<
    typename boost::range_const_iterator< Range >::type
  >::type
> operator|( const Range &range, const twist::range::adaptor::shadow &tag ) {
  return tag( range );
}


#endif

