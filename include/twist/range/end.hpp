#ifndef TWIST_RANGE_END_HPP
#define TWIST_RANGE_END_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/concepts.hpp>

namespace twist {
  namespace range {
    namespace result_of {
      template< typename Range, typename Enable = void >
      struct end {};
      template< typename Range >
      struct end< Range, typename boost::enable_if< boost::is_const< Range > >::type > {
        typedef typename boost::range_const_iterator<
          typename boost::remove_reference<
            typename boost::remove_cv< Range >::type
          >::type
        >::type type;
      };
      template< typename Range >
      struct end< Range, typename boost::disable_if< boost::is_const< Range > >::type > {
        typedef typename boost::range_iterator<
          typename boost::remove_reference<
            typename boost::remove_cv< Range >::type
          >::type
        >::type type;
      };
    }
    template< typename Range >
    typename result_of::end< Range& >::type
    end( Range &range ) {
      return boost::end( range );
    }
    template< typename Range >
    typename result_of::end< const Range& >::type
    end( const Range &range ) {
      return boost::const_end( range );
    }
  }
}

#endif

