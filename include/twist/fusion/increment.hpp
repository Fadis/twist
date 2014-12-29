#ifndef TWIST_FUSION_INCREMENT_HPP
#define TWIST_FUSION_INCREMENT_HPP

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
      void increment_at( Vector &vec ) {
        ++boost::fusion::at< Index >( vec );
      }
      template< typename Index, typename Vector >
      void increment_all(
        Vector &,
        typename boost::enable_if<
          boost::mpl::equal_to<
            boost::mpl::size< Vector >,
            Index
          >
        >::type* = 0
      ) {}
      template< typename Index, typename Vector >
      void increment_all(
        Vector &vec,
        typename boost::disable_if<
          boost::mpl::equal_to<
            boost::mpl::size< Vector >,
            Index
          >
        >::type* = 0
      ) {
        increment_at< Index >( vec );
        increment_all<
          boost::mpl::plus<
            Index,
            boost::mpl::size_t< 1 >
          >
        >( vec );
      }
    }

    template< typename OriginalVector >
    void increment( OriginalVector &vec ) {
      detail::increment_all< boost::mpl::size_t< 0 > >( vec );
    }
  }
}

#endif

