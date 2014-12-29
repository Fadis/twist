#ifndef TWIST_RANGE_ADAPTOR_JSON_DECODING_HPP
#define TWIST_RANGE_ADAPTOR_JSON_DECODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/json_decoding.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct json_decoding {
          json_decoding() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::json_decoding_iterator<
              typename boost::range_const_iterator< Range >::type
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::decode_json( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::decode_json( boost::end( range ), boost::end( range ) )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::json_decoding_iterator<
              typename boost::range_iterator< Range >::type
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::decode_json( boost::begin( range ), boost::end( range ) ),
              twist::iterator::adaptor::decode_json( boost::end( range ), boost::end( range ) )
            );
          }
        };
      }
      typedef detail::json_decoding json_decoding;
      const json_decoding decode_json;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::json_decoding_iterator<
    typename boost::range_const_iterator< Range >::type
  >
> operator|( const Range &range, const twist::range::adaptor::detail::json_decoding &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::json_decoding_iterator<
    typename boost::range_iterator< Range >::type
  >
> operator|( Range &range, const twist::range::adaptor::detail::json_decoding &tag ) {
  return tag( range );
}
#endif

