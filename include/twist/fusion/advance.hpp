#ifndef TWIST_FUSION_ADVANCE_HPP
#define TWIST_FUSION_ADVANCE_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>

namespace twist {
  namespace fusion {
    namespace detail {
      template< typename Index, typename Vector >
      void advance_at( Vector &vec, int offset ) {
        std::advance( boost::fusion::at< Index >( vec ), offset );
      };
      template< typename Index, typename Vector >
      void advance_all(
        Vector &vec, int offset,
        typename boost::enable_if<
          boost::mpl::equal_to<
            boost::mpl::size< Vector >,
            Index
          >
        >::type* = 0
      ) {};
      template< typename Index, typename Vector >
      void advance_all(
        Vector &vec, int offset,
        typename boost::disable_if<
          boost::mpl::equal_to<
            boost::mpl::size< Vector >,
            Index
          >
        >::type* = 0
      ) {
        advance_at< Index >( vec, offset );
        advance_all<
          boost::mpl::plus<
            Index,
            boost::mpl::size_t< 1 >
          >
        >( vec, offset );
      };
    }

    template< typename OriginalVector >
    void advance( OriginalVector &vec, int offset ) {
      detail::advance_all< boost::mpl::size_t< 0 > >( vec, offset );
    }
  }
}

#endif

