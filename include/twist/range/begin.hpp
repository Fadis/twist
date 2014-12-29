#ifndef TWIST_RANGE_BEGIN_HPP
#define TWIST_RANGE_BEGIN_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/concepts.hpp>

namespace twist {
  namespace range {
    namespace result_of {
      template< typename Range, typename Enable = void >
      struct begin {};
      template< typename Range >
      struct begin< Range, typename boost::enable_if< boost::is_const< Range > >::type > {
        typedef typename boost::range_const_iterator<
          typename boost::remove_reference<
            typename boost::remove_cv< Range >::type
          >::type
        >::type type;
      };
      template< typename Range >
      struct begin< Range, typename boost::disable_if< boost::is_const< Range > >::type > {
        typedef typename boost::range_iterator<
          typename boost::remove_reference<
            typename boost::remove_cv< Range >::type
          >::type
        >::type type;
      };
    }
    template< typename Range >
    typename result_of::begin< Range& >::type
    begin( Range &range ) {
      return boost::begin( range );
    }
    template< typename Range >
    typename result_of::begin< const Range& >::type
    begin( const Range &range ) {
      return boost::const_begin( range );
    }
  }
}

#endif

