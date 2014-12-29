#ifndef TWIST_RANGE_ADAPTOR_DOWNGRADED_HPP
#define TWIST_RANGE_ADAPTOR_DOWNGRADED_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/downgraded.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        template< typename Traversal, typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::downgraded<
            typename boost::range_const_iterator< Range >::type,
            Traversal
          >
        > downgrade( const Range &range ) {
          return boost::make_iterator_range(
            twist::iterator::adaptor::downgrade< Traversal >( boost::const_begin( range ) ),
            twist::iterator::adaptor::downgrade< Traversal >( boost::const_end( range ) )
          );
        }
        template< typename Traversal, typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::downgraded<
            typename boost::range_iterator< Range >::type,
            Traversal
          >
        > downgrade( Range &range ) {
          return boost::make_iterator_range(
            twist::iterator::adaptor::downgrade< Traversal >( boost::begin( range ) ),
            twist::iterator::adaptor::downgrade< Traversal >( boost::end( range ) )
          );
        }
      }
      template< typename Traversal >
      struct downgraded {
        typedef Traversal type;
      };
    }
  }
}

template<
  typename Range, typename Traversal,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::downgraded<
    typename boost::range_const_iterator< Range >::type,
    Traversal
  >
> operator|( const Range &range, const twist::range::adaptor::downgraded< Traversal > &tag ) {
  return twist::range::adaptor::detail::downgrade< Traversal >( range );
}
template<
  typename Range, typename Traversal,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::downgraded<
    typename boost::range_iterator< Range >::type,
    Traversal
  >
> operator|( Range &range, const twist::range::adaptor::downgraded< Traversal > &tag ) {
  return twist::range::adaptor::detail::downgrade< Traversal >( range );
}
#endif

