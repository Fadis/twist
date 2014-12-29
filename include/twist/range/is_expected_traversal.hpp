#ifndef TWIST_RANGE_IS_EXPECTED_TRAVERSAL_HPP
#define TWIST_RANGE_IS_EXPECTED_TRAVERSAL_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/find_if.hpp>
#include <twist/range/adaptor/downgraded.hpp>

namespace twist {
  namespace range {
    template< typename T, typename Base, typename Expected >
      struct is_expected_traversal_ :
        public twist::iterator::is_traversal_of<
        typename Expected::second,
        typename boost::range_iterator<
        decltype(
            std::declval< Base >()|
            twist::range::adaptor::downgraded<
            typename Expected::first
            >()|
            std::declval< const T& >()
            )
        >::type
        >::type {};

    template< typename T, typename Base, typename Expected >
      struct is_expected_traversal :
        public boost::is_same<
        typename boost::mpl::find_if<
        Expected,
        boost::mpl::not_< is_expected_traversal_< T, Base, boost::mpl::_1 > >
        >::type,
        typename boost::mpl::end< Expected >::type
        >::type {};
  }
}

#endif

