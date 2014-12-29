#ifndef TWIST_VARIADIC_TEMPLATES_SIZE_HPP
#define TWIST_VARIADIC_TEMPLATES_SIZE_HPP

#include <boost/mpl/size_t.hpp>

namespace twist {
  namespace variadic_templates {
    namespace detail {
      template< typename ...Sequence >
      struct size {};
      template< typename Head, typename ...Tail >
      struct size< Head, Tail... > {
        typedef typename size< Tail... >::type::next type;
      };
      template<>
      struct size<> {
        typedef boost::mpl::size_t< 0 > type;
      };
    }
    template< typename ...Sequence >
    struct size : public detail::size< Sequence... >::type {};
  }
}

#endif

