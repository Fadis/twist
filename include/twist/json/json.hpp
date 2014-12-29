#ifndef TWIST_JSON_JSON_HPP
#define TWIST_JSON_JSON_HPP

#include <iostream>
#ifndef BOOST_SPIRIT_USE_PHOENIX_V3
# define BOOST_SPIRIT_USE_PHOENIX_V3
#endif
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <twist/range/adaptor/json_string_decoding.hpp>
#include <twist/range/adaptor/pure_transformed.hpp>

namespace twist {
  namespace json {
    struct unexpected_json_event {};
    enum class json_event {
      null,
      boolean,
      number,
      string,
      escaped_string,
      begin_array,
      end_array,
      begin_struct,
      end_struct,
      comment
    };

    template< typename Range >
    class json {
    public:
      json() {}
      json( const json_event &event__, const Range &data__ ) : event_( event__ ), data_( data__ ) {}
      json( const json& ) = default;
      json( json&& ) = default;
      json &operator=( const json& ) = default;
      json &operator=( json&& ) = default;
      const json_event &event() const { return event_; }
      const Range &data() const { return data_; }
    private:
      json_event event_;
      Range data_;
    };

    template< typename LeftRange, typename RightRange >
    bool operator==( const json< LeftRange > &left, const json< RightRange > &right ) {
      std::cout << __LINE__ << " " << int( left.event() ) << " " << int( right.event() ) << std::endl;
      if( left.event() == right.event() ) {
        std::cout << __LINE__ << " " << std::string( left.data().begin(), left.data().end() ) << std::endl;
        std::cout << __LINE__ << " " << std::string( right.data().begin(), right.data().end() ) << std::endl;
        return boost::equal( left.data(), right.data() );
      }
      else
        return false;
    }
    template< typename LeftRange, typename RightRange >
    bool operator!=( const json< LeftRange > &left, const json< RightRange > &right ) {
      std::cout << __LINE__ << " " << int( left.event() ) << " " << int( right.event() ) << std::endl;
      if( left.event() == right.event() ) {
        std::cout << __LINE__ << " " << std::string( left.data().begin(), left.data().end() ) << std::endl;
        std::cout << __LINE__ << " " << std::string( right.data().begin(), right.data().end() ) << std::endl;
        return !boost::equal( left.data(), right.data() );
      }
      else
        return true;
    }

    template< typename Range >
    auto get_string(
      const json< Range > &json_
    ) -> decltype( json_.data() ) {
      if( json_.event() == json_event::string || json_.event() == json_event::comment )
        return json_.data_;
      else
        throw unexpected_json_event();
    }
    
    template< typename Range >
    auto get_escaped_string(
      const json< Range > &json_
    ) -> decltype( json_.data()|range::adaptor::decode_json_string ) {
      if( json_.event() == json_event::escaped_string || json_.event() == json_event::comment )
        return json_.data_|range::adaptor::decode_json_string;
      else
        throw unexpected_json_event();
    }

    template< typename Range >
    bool get_boolean(
      const json< Range > &json_
    ) {
      if( json_.event() == json_event::boolean )
        return *boost::begin( json_.data() ) == U't';
      else
        throw unexpected_json_event();
    }

    namespace qi {
      template< typename Iterator, typename Type >
      class json_number : public boost::spirit::qi::grammar<
        Iterator, Type()
      > {
      public:
        json_number() : json_number::base_type( root ) {
          namespace qi = boost::spirit::qi;
          namespace phx = boost::phoenix;
          hex_value = ( -qi::char_( "+-" ) >> "0x" >> hex_p )[
            qi::_val = phx::bind( &json_number::to_double, qi::_1, qi::_2 )
          ];
          bin_value = ( -qi::char_( "+-" ) >> "0b" >> bin_p )[
            qi::_val = phx::bind( &json_number::to_double, qi::_1, qi::_2 )
          ];
          oct_value = ( -qi::char_( "+-" ) >> "0" >> oct_p )[
            qi::_val = phx::bind( &json_number::to_double, qi::_1, qi::_2 )
          ];
          root = hex_value|bin_value|oct_value|qi::double_;
        }
      private:
        static double to_double( const boost::optional< char > &sign, uint64_t value ) {
          if( sign )
            return ( ( *sign == '-' ) ? -1.0 : 1.0 ) * double( value );
          else
            return double( value );
        }
        boost::spirit::qi::rule< Iterator, Type() > hex_value;
        boost::spirit::qi::rule< Iterator, Type() > bin_value;
        boost::spirit::qi::rule< Iterator, Type() > oct_value;
        boost::spirit::qi::rule< Iterator, Type() > root;
        boost::spirit::qi::uint_parser< uint64_t, 16, 1, 16 > hex_p;
        boost::spirit::qi::uint_parser< uint64_t, 8, 1, 20 > oct_p;
        boost::spirit::qi::uint_parser< uint64_t, 2, 1, 64 > bin_p;
      };
    }

    template< typename Range >
    double get_number(
      const json< Range > &json_
    ) {
      if( json_.event() == json_event::number ) {
        const auto range = json_.data()|range::adaptor::pure_transformed( []( char32_t value ) { return char( value ); } );
        const std::vector< char > copied( boost::begin( range ), boost::end( range ) );
        qi::json_number< std::vector< char >::const_iterator, double > rule;
        auto iter = boost::begin( copied );
        double value;
        if( boost::spirit::qi::parse( iter, boost::end( copied ), rule, value ) && iter == boost::end( copied ) )
          return value;
        else
          throw -1;
      }
      else
        throw unexpected_json_event();
    }
  }
}

#endif

