#ifndef TWIST_RANGE_ADAPTOR_UTF8_ENCODING_HPP
#define TWIST_RANGE_ADAPTOR_UTF8_ENCODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/utf8_encoding.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct utf8_encoding {
          utf8_encoding() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf8_encoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_utf8( boost::begin( range ) ),
              twist::iterator::adaptor::encode_utf8( boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf8_encoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_utf8( boost::begin( range ) ),
              twist::iterator::adaptor::encode_utf8( boost::end( range ) )
            );
          }
        };
      }
      typedef detail::utf8_encoding utf8_encoding;
      const utf8_encoding encode_utf8;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf8_encoding_iterator<
    typename boost::range_const_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::detail::utf8_encoding &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf8_encoding_iterator<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::detail::utf8_encoding &tag ) {
  return tag( range );
}
#endif

