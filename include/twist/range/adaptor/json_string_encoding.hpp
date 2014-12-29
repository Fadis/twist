#ifndef TWIST_RANGE_ADAPTOR_JSON_STRING_ENCODING_HPP
#define TWIST_RANGE_ADAPTOR_JSON_STRING_ENCODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/json_string_encoding.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        struct json_string_encoding {
          json_string_encoding() {}
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::concatenated<
              twist::iterator::adaptor::json_string_encoding_iterator<
                typename boost::range_const_iterator< Range >::type
              >, false
            >
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_json_string( boost::begin( range ) ),
              twist::iterator::adaptor::encode_json_string( boost::end( range )  )
            );
          }
          template< typename Range >
          boost::iterator_range<
            twist::iterator::adaptor::concatenated<
              twist::iterator::adaptor::json_string_encoding_iterator<
                typename boost::range_iterator< Range >::type
              >, false
            >
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::encode_json_string( boost::begin( range ) ),
              twist::iterator::adaptor::encode_json_string( boost::end( range )  )
            );
          }
        };
      }
      typedef detail::json_string_encoding json_string_encoding;
      const json_string_encoding encode_json_string;
    }
  }
}

template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::concatenated<
    twist::iterator::adaptor::json_string_encoding_iterator<
      typename boost::range_const_iterator< Range >::type
    >, false
  >
> operator|( const Range &range, const twist::range::adaptor::detail::json_string_encoding &tag ) {
  return tag( range );
}
template<
  typename Range,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  twist::iterator::adaptor::concatenated<
    twist::iterator::adaptor::json_string_encoding_iterator<
      typename boost::range_iterator< Range >::type
    >, false
  >
> operator|( Range &range, const twist::range::adaptor::detail::json_string_encoding &tag ) {
  return tag( range );
}
#endif

