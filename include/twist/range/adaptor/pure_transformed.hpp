#ifndef TWIST_RANGE_ADAPTOR_PURE_TRANSFORMED_HPP
#define TWIST_RANGE_ADAPTOR_PURE_TRANSFORMED_HPP

#include <boost/range/iterator_range.hpp>
#include <twist/iterator/adaptor/pure_transformed.hpp>
#include <twist/utils/is_range.hpp>

namespace twist {
  namespace range {
    namespace adaptor {
      namespace detail {
        template< typename Function > 
        struct pure_transform {
          pure_transform( const Function &func_ ) : func( func_ ) {}
          template< typename Range >
          boost::iterator_range<
            typename twist::iterator::adaptor::pure_transformed_iterator_type<
              typename boost::range_const_iterator< Range >::type,
              Function
            >::type
          > operator()( const Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::pure_transform( boost::begin( range ), func ),
              twist::iterator::adaptor::pure_transform( boost::end( range ), func )
            );
          }
          template< typename Range >
          boost::iterator_range<
            typename twist::iterator::adaptor::pure_transformed_iterator_type<
              typename boost::range_iterator< Range >::type,
              Function
            >::type
          > operator()( Range &range ) const {
            return boost::make_iterator_range(
              twist::iterator::adaptor::pure_transform( boost::begin( range ), func ),
              twist::iterator::adaptor::pure_transform( boost::end( range ), func )
            );
          }
        private:
          Function func;
        };
      }
      template< typename Function >
      using pure_transform = detail::pure_transform< Function >;
      template< typename Function >
      pure_transform<
        Function
      > pure_transformed( const Function &func ) {
        return pure_transform< Function >( func );
      }
    }
  }
}

template<
  typename Range, typename Function,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  typename twist::iterator::adaptor::pure_transformed_iterator_type<
    typename boost::range_const_iterator< Range >::type,
    Function
  >::type
> operator|( const Range &range, const twist::range::adaptor::pure_transform< Function > &tag ) {
  return tag( range );
}
template<
  typename Range, typename Function,
  typename Enable = typename boost::enable_if< typename twist::utils::is_range< Range >::type >::type
>
boost::iterator_range<
  typename twist::iterator::adaptor::pure_transformed_iterator_type<
    typename boost::range_iterator< Range >::type,
    Function
  >::type
> operator|( Range &range, const twist::range::adaptor::pure_transform< Function > &tag ) {
  return tag( range );
}
#endif

