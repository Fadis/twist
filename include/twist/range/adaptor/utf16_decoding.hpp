#ifndef TWIST_RANGE_ADAPTOR_UTF8_DECODING_HPP
#define TWIST_RANGE_ADAPTOR_UTF8_DECODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/utf16_decoding.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct utf16_decoding {
          utf16_decoding() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf16_decoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::decode_utf16( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::decode_utf16( boost::end( range ), boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf16_decoding_iterator<
              typename boost::range_iterator< Range >::type
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::decode_utf16( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::decode_utf16( boost::end( range ), boost::end( range ) )
            );
          }
        };
      }
      typedef detail::utf16_decoding utf16_decoding;
      const utf16_decoding decode_utf16;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf16_decoding_iterator<
    typename boost::range_const_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::detail::utf16_decoding &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf16_decoding_iterator<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::detail::utf16_decoding &tag ) {
  return tag( range );
}
#endif

