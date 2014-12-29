#ifndef TWIST_RANGE_ADAPTOR_UTF16_ENCODING_HPP
#define TWIST_RANGE_ADAPTOR_UTF16_ENCODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/utf16_encoding.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct utf16_encoding {
          utf16_encoding() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf16_encoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_utf16( boost::begin( range ) ),
              twist::iterator::adaptor::encode_utf16( boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::utf16_encoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_utf16( boost::begin( range ) ),
              twist::iterator::adaptor::encode_utf16( boost::end( range ) )
            );
          }
        };
      }
      typedef detail::utf16_encoding utf16_encoding;
      const utf16_encoding encode_utf16;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf16_encoding_iterator<
    typename boost::range_const_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::detail::utf16_encoding &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::utf16_encoding_iterator<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::detail::utf16_encoding &tag ) {
  return tag( range );
}
#endif

