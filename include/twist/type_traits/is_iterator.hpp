#ifndef TWIST_TYPE_TRAITS_IS_ITERATOR_HPP
#define TWIST_TYPE_TRAITS_IS_ITERATOR_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace twist {
  namespace type_traits {
    BOOST_MPL_HAS_XXX_TRAIT_DEF( iterator_category )
    template< typename T >
    struct is_iterator :
      public boost::mpl::or_<
        boost::is_pointer< T >,
        has_iterator_category< T >
      > {};
  }
}

#endif

