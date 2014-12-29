#ifndef TWIST_UTILS_BASE64_TRAITS_HPP
#define TWIST_UTILS_BASE64_TRAITS_HPP

#include <boost/integer_traits.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/char.hpp>

namespace twist {
  namespace base64 {
    struct generic {};
    struct rfc1421 {};
    struct rfc2045 {};
    struct rfc3548 {};
    struct rfc4648 {};
    struct rfc4880 {};
    struct utf7 {};
    struct url {};

    template< typename Tag >
      struct plus {};
    template<>
      struct plus< generic > : public boost::mpl::char_< '+' > {};
    template<>
      struct plus< rfc1421 > : public plus< generic > {};
    template<>
      struct plus< rfc2045 > : public plus< generic > {};
    template<>
      struct plus< rfc3548 > : public plus< generic > {};
    template<>
      struct plus< rfc4648 > : public plus< rfc3548 > {};
    template<>
      struct plus< rfc4880 > : public plus< generic > {};
    template<>
      struct plus< utf7 > : public plus< generic > {};
    template<>
      struct plus< url > : public boost::mpl::char_< '-' > {};

    template< typename Tag >
      struct slash {};
    template<>
      struct slash< generic > : public boost::mpl::char_< '/' > {};
    template<>
      struct slash< rfc1421 > : public slash< generic > {};
    template<>
      struct slash< rfc2045 > : public slash< generic > {};
    template<>
      struct slash< rfc3548 > : public slash< generic > {};
    template<>
      struct slash< rfc4648 > : public slash< rfc3548 > {};
    template<>
      struct slash< rfc4880 > : public slash< generic > {};
    template<>
      struct slash< utf7 > : public slash< generic > {};
    template<>
      struct slash< url > : public boost::mpl::char_< '_' > {};

    template< typename Tag >
      struct padding {};
    template<>
      struct padding< generic > : public boost::mpl::char_< '\0' > {};
    template<>
      struct padding< rfc1421 > : public boost::mpl::char_< '=' > {};
    template<>
      struct padding< rfc2045 > : public boost::mpl::char_< '=' > {};
    template<>
      struct padding< rfc3548 > : public boost::mpl::char_< '=' > {};
    template<>
      struct padding< rfc4648 > : public padding< rfc3548 > {};
    template<>
      struct padding< rfc4880 > : public boost::mpl::char_< '=' > {};
    template<>
      struct padding< utf7 > : public padding< generic > {};
    template<>
      struct padding< url > : public padding< generic > {};

    template< typename Tag >
      struct max_length {};
    template<>
      struct max_length< generic > : public boost::mpl::size_t< 76 > {};
    template<>
      struct max_length< rfc1421 > : public boost::mpl::size_t< 64 > {};
    template<>
      struct max_length< rfc2045 > : public max_length< generic > {};
    template<>
      struct max_length< rfc3548 > : public max_length< generic > {};
    template<>
      struct max_length< rfc4648 > : public max_length< rfc3548 > {};
    template<>
      struct max_length< rfc4880 > : public max_length< generic > {};
    template<>
      struct max_length< utf7 > : public boost::mpl::size_t< boost::integer_traits< size_t >::const_max > {};
    template<>
      struct max_length< url > : public boost::mpl::size_t< boost::integer_traits< size_t >::const_max > {};

    template< typename Tag >
      struct skip_unexpected {};
    template<>
      struct skip_unexpected< generic > : public boost::mpl::bool_< false > {};
    template<>
      struct skip_unexpected< rfc1421 > : public skip_unexpected< generic > {};
    template<>
      struct skip_unexpected< rfc2045 > : public boost::mpl::bool_< true > {};
    template<>
      struct skip_unexpected< rfc3548 > : public skip_unexpected< generic > {};
    template<>
      struct skip_unexpected< rfc4648 > : public skip_unexpected< rfc3548 > {};
    template<>
      struct skip_unexpected< rfc4880 > : public skip_unexpected< generic > {};
    template<>
      struct skip_unexpected< utf7 > : public skip_unexpected< generic > {};
    template<>
      struct skip_unexpected< url > : public skip_unexpected< generic > {};
  
    template< typename Tag >
      struct checksum {};
    template<>
      struct checksum< generic > : public boost::mpl::bool_< false > {};
    template<>
      struct checksum< rfc1421 > : public checksum< generic > {};
    template<>
      struct checksum< rfc2045 > : public checksum< generic > {};
    template<>
      struct checksum< rfc3548 > : public checksum< generic > {};
    template<>
      struct checksum< rfc4648 > : public checksum< rfc3548 > {};
    template<>
      struct checksum< rfc4880 > : public boost::mpl::bool_< true > {};
    template<>
      struct checksum< utf7 > : public checksum< generic > {};
    template<>
      struct checksum< url > : public checksum< generic > {};

  }
}

#endif

