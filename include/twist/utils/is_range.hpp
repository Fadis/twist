#ifndef TWIST_UTILS_IS_RANGE_HPP
#define TWIST_UTILS_IS_RANGE_HPP

#include <utility>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility/enable_if.hpp>

namespace twist {
  namespace utils {
    template< typename T >
    class is_range {
      template< typename T_ >
      static std::true_type f( T_, typename boost::range_const_iterator< T_ >::type* = 0 );
      static std::false_type f(...);
    public:
      typedef decltype( is_range<T>::f( std::declval<T>() ) ) type;
      static constexpr bool value = type::value;
    };
  }
}

#endif

