#ifndef TWIST_RANGE_ADAPTOR_DETACH_HPP
#define TWIST_RANGE_ADAPTOR_DETACH_HPP

#include <boost/type_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/shared_container_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct detach {
          detach() {}
          template< typename Range >
          boost::iterator_range<
            boost::shared_container_iterator< typename boost::remove_reference< Range >::type >
          > operator()( Range &&range_ ) const {
            typedef typename boost::remove_reference< Range >::type type;
            boost::shared_ptr< type > range( new type( range_ ) );
            return boost::make_iterator_range(
              boost::shared_container_iterator< type >( boost::begin( *range ), range ),
              boost::shared_container_iterator< type >( boost::end( *range ), range )
            );
          }
        };
      }
      const detail::detach detached;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  boost::shared_container_iterator< Range >
> operator|( Range &&range, const twist::range::adaptor::detail::detach &tag ) {
  return tag( range );
}
#endif

