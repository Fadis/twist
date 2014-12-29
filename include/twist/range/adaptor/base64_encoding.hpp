#ifndef TWIST_RANGE_ADAPTOR_BASE64_ENCODING_HPP
#define TWIST_RANGE_ADAPTOR_BASE64_ENCODING_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/base64_encoding.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      template< typename Traits >
      struct encode_base64 {
        encode_base64() {}
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::base64_encoding_iterator<
            typename boost::range_const_iterator< Range >::type,
            Traits
          >
        > operator()( const Range &range ) const {
          return boost::make_iterator_range(
            twist::iterator::adaptor::encode_base64< Traits >( boost::begin( range ), boost::end( range ) ),
            twist::iterator::adaptor::encode_base64< Traits >( boost::end( range ), boost::end( range ) )
          );
        }
        template< typename Range >
        boost::iterator_range<
          twist::iterator::adaptor::base64_encoding_iterator<
            typename boost::range_iterator< Range >::type,
            Traits
          >
        > operator()( Range &range ) const {
          return boost::make_iterator_range(
            twist::iterator::adaptor::encode_base64< Traits >( boost::begin( range ), boost::end( range ) ),
            twist::iterator::adaptor::encode_base64< Traits >( boost::end( range ), boost::end( range ) )
          );
        }
      };
    }
  }
}

template< typename Range, typename Traits >
boost::iterator_range<
  twist::iterator::adaptor::base64_encoding_iterator<
    typename boost::range_const_iterator< Range >::type,
    Traits
  >
> operator|( const Range &range, const twist::range::adaptor::encode_base64< Traits > &tag ) {
  return tag( range );
}
template< typename Range, typename Traits >
boost::iterator_range<
  twist::iterator::adaptor::base64_encoding_iterator<
    typename boost::range_iterator< Range >::type,
    Traits
  >
> operator|( Range &range, const twist::range::adaptor::encode_base64< Traits > &tag ) {
  return tag( range );
}
#endif

