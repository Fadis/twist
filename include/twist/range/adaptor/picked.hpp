#ifndef TWIST_RANGE_ADAPTOR_PICKED_HPP
#define TWIST_RANGE_ADAPTOR_PICKED_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/picked.hpp>
#include <cxxabi.h>
#include <typeinfo>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      template< typename Index >
      struct picked {
        picked() {}
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::picked<
            typename boost::range_iterator< Range >::type,
            Index
          >
        > operator()( Range &range ) const {
          return boost::make_iterator_range(
            twist::iterator::adaptor::picked<
              typename boost::range_iterator< Range >::type,
              Index
            >( boost::begin( range ) ),
            twist::iterator::adaptor::picked<
              typename boost::range_iterator< Range >::type,
              Index
            >( boost::end( range ) )
          );
        }
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::picked<
            typename boost::range_iterator< Range >::type,
            Index
          >
        > operator()( const Range &range ) const {
          return boost::make_iterator_range(
            twist::iterator::adaptor::picked<
              typename boost::range_iterator< Range >::type,
              Index
            >( boost::begin( range ) ),
            twist::iterator::adaptor::picked<
              typename boost::range_iterator< Range >::type,
              Index
            >( boost::end( range ) )
          );
        }
      };
    }
  }
}

template<
  typename Range, typename Index,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::picked<
    typename boost::range_iterator< Range >::type,
    Index
  >
> operator|( Range &range, const twist::range::adaptor::picked< Index > &tag ) {
  return tag( range );
}

template<
  typename Range, typename Index,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::picked<
    typename boost::range_iterator< Range >::type,
    Index
  >
> operator|( const Range &range, const twist::range::adaptor::picked< Index > &tag ) {
  return tag( range );
}


#endif

