#ifndef TWIST_ITERATOR_ADAPTOR_JSON_DECODING_HPP
#define TWIST_ITERATOR_ADAPTOR_JSON_DECODING_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/utility.hpp>
#ifndef BOOST_SPIRIT_USE_PHOENIX_V3
# define BOOST_SPIRIT_USE_PHOENIX_V3
#endif
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/json/json.hpp>
#include <twist/exceptions.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class json_decoding_iterator {};

      template< typename BaseIterator >
      struct json_decoding_iterator_value_type {
        typedef json::json< boost::iterator_range< BaseIterator > > type;
      };

      template< typename BaseIterator >
      struct json_decoding_iterator_base_type {
        typedef boost::iterator_adaptor<
          json_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename json_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename json_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename Iterator >
      bool skip_space( Iterator &iter ) {
        bool force_string = false;
        for( ;; ++iter ) {
          switch( *iter ) {
          case U':':
            force_string = true;
          case U' ':
          case U',':
          case U'\t':
          case U'\x0a':
          case U'\x0d':
            continue;
          default:
            return force_string;
          };
        }
        return force_string;
      }

      template< typename Iterator >
      bool skip_space( Iterator &iter, const Iterator &end ) {
        bool force_string = false;
        for( ;; ++iter ) {
          if( iter == end )
            return false;
          switch( *iter ) {
          case U':':
            force_string = true;
          case U' ':
          case U',':
          case U'\t':
          case U'\x0a':
          case U'\x0d':
            continue;
          default:
            return force_string;
          };
        }
        return force_string;
      }

      template< typename Iterator >
      std::pair< json::json< boost::iterator_range< Iterator > >, Iterator > parse_json_string( const Iterator &iter, char32_t delimitor ) {
        auto iter_ = iter;
        const auto begin = iter_;
        bool escaped = false;
        for( ;; ++iter_ ) {
          if( *iter_ == delimitor ) {
            if( !escaped )
              break;
            else
              escaped = false;
          }
          else if( *iter_ == U'\\' )
            escaped = true;
          else
            escaped = false;
        }
        const auto end = iter_;
        ++iter_;
        skip_space( iter_ );
        return std::make_pair( json::json< boost::iterator_range< Iterator > >(
          json::json_event::escaped_string,
          boost::make_iterator_range( begin, end )
        ), iter_ );
      }
      
      template< typename Iterator >
      std::pair< json::json< boost::iterator_range< Iterator > >, Iterator > parse_json_string( const Iterator &iter, const Iterator &end, char32_t delimitor ) {
        auto iter_ = iter;
        const auto begin = iter_;
        bool escaped = false;
        for( ; ; ++iter_ ) {
          if( iter_ == end )
            throw unexpected_end();
          if( *iter_ == delimitor ) {
            if( !escaped )
              break;
            else
              escaped = false;
          }
          else if( *iter_ == U'\\' )
            escaped = true;
          else
            escaped = false;
        }
        const auto end_ = iter_;
        ++iter_;
        skip_space( iter_, end );
        return std::make_pair( json::json< boost::iterator_range< Iterator > >(
          json::json_event::escaped_string,
          boost::make_iterator_range( begin, end_ )
        ), iter_ );
      }

      template< typename Iterator >
      std::pair< json::json< boost::iterator_range< Iterator > >, Iterator > parse_json_number( const Iterator &begin, const Iterator &end, const Iterator &next ) {
        
        const std::vector< char > copied( begin, end );
        auto iter = boost::begin( copied );
        auto end_ = boost::end( copied );
        double value;
        json::qi::json_number< std::vector< char >::const_iterator, double > rule;
        if( boost::spirit::qi::parse( iter, end_, rule, value ) && iter == end_ )
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::number,
            boost::make_iterator_range( begin, end )
          ), next );
        else
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::string,
            boost::make_iterator_range( begin, end )
          ), next );
      }

      template< typename Iterator >
      std::pair< typename json_decoding_iterator_value_type< Iterator >::type, Iterator > dereference_json( const Iterator &iter ) {
        Iterator iter_ = iter;
        skip_space( iter_ );
        const char32_t first_char = *iter_;
        ++iter_;
        if( first_char == U'[' ) {
          auto end_ = iter_;
          skip_space( iter_ );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::begin_array,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U']' ) {
          auto end_ = iter_;
          skip_space( iter_ );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::end_array,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U'{' ) {
          auto end_ = iter_;
          skip_space( iter_ );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::begin_struct,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U'}' ) {
          auto end_ = iter_;
          skip_space( iter_ );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::end_struct,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U':' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U',' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U' ' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U'\t' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U'\x0a' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U'\x0c' ) {
          return dereference_json( iter_ );
        }
        else if( first_char == U'"' ) {
          return parse_json_string( iter_, U'"' );
        }
        else if( first_char == U'\'' ) {
          return parse_json_string( iter_, U'\'' );
        }
        else if( first_char == U'/' ) {
          const uint32_t second_char = *iter_;
          ++iter_;
          const auto begin = iter_;
          if( second_char == U'/' ) {
            for( ;; ++iter_ ) {
              if( *iter_ == '\x0a' )
                break;
              else if( *iter_ == '\x0d' )
                break;
            }
            const auto end_ = iter_;
            skip_space( iter_ );
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::comment,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          else if( second_char == U'*' ) {
            bool star_was_detected = false;
            for( ;; ++iter_ ) {
              if( *iter_ == '*' )
                star_was_detected = true;
              else if( *iter_ == '/' && star_was_detected )
                break;
              else
                star_was_detected = false;
            }
            const auto end_ = iter_;
            skip_space( iter_ );
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::comment,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          else
            throw invalid_json();
        }
        else {
          auto begin = iter;
          skip_space( begin );
          for( ;; ++iter_ ) {
            if( *iter_ == U':' )
              break;
            else if( *iter_ == U'[' )
              break;
            else if( *iter_ == U']' )
              break;
            else if( *iter_ == U'\'' )
              break;
            else if( *iter_ == U'"' )
              break;
            else if( *iter_ == U'{' )
              break;
            else if( *iter_ == U'}' )
              break;
            else if( *iter_ == U' ' )
              break;
            else if( *iter_ == U'\t' )
              break;
            else if( *iter_ == U'\x0a' )
              break;
            else if( *iter_ == U'\x0d' )
              break;
            else if( *iter_ == U',' )
              break;
          }
          const auto end_ = iter_;
          bool force_string = false;
          skip_space( iter_ );
          if( force_string ) {
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::string,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          switch( first_char ) {
            case U'n': {
              if( std::distance( begin, end_ ) == 4u && std::equal( begin, end_, U"null" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::null,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            case U'+':
            case U'-':
            case U'.':
            case U'I':
            case U'i':
            case U'N':
            case U'0':
            case U'1':
            case U'2':
            case U'3':
            case U'4':
            case U'5':
            case U'6':
            case U'7':
            case U'8':
            case U'9':
              return parse_json_number( begin, end_, iter_ );
            case U't': {
              if( std::distance( begin, end_ ) == 4u && std::equal( begin, end_, U"true" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::boolean,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            case U'f': {
              if( std::distance( begin, end_ ) == 5u && std::equal( begin, end_, U"false" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::boolean,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            default: {
              return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                json::json_event::string,
                boost::make_iterator_range( begin, end_ )
              ), iter_ );
            }
          };
        }
      }
      template< typename Iterator >
      std::pair< typename json_decoding_iterator_value_type< Iterator >::type, Iterator > dereference_json_with_end( const Iterator &iter, const Iterator &end ) {
        Iterator iter_ = iter;
        skip_space( iter_, end );
        if( iter_ == end )
          throw unexpected_end();
        const char32_t first_char = *iter_;
        ++iter_;
        if( first_char == U'[' ) {
          auto end_ = iter_;
          skip_space( iter_, end );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::begin_array,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U']' ) {
          auto end_ = iter_;
          skip_space( iter_, end );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::end_array,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U'{' ) {
          auto end_ = iter_;
          skip_space( iter_, end );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::begin_struct,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U'}' ) {
          auto end_ = iter_;
          skip_space( iter_, end );
          return std::make_pair( json::json< boost::iterator_range< Iterator > >(
            json::json_event::end_struct,
            boost::make_iterator_range( iter, end_ )
          ), iter_ );
        }
        else if( first_char == U':' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U',' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U' ' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U'\t' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U'\x0a' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U'\x0c' ) {
          return dereference_json_with_end( iter_, end );
        }
        else if( first_char == U'"' ) {
          return parse_json_string( iter_, end, U'"' );
        }
        else if( first_char == U'\'' ) {
          return parse_json_string( iter_, end, U'\'' );
        }
        else if( first_char == U'/' ) {
          if( iter_ == end )
            throw unexpected_end();
          const uint32_t second_char = *iter_;
          ++iter_;
          const auto begin = iter_;
          if( second_char == U'/' ) {
            for( ;; ++iter_ ) {
              if( iter_ == end )
                break;
              else if( *iter_ == '\x0a' )
                break;
              else if( *iter_ == '\x0d' )
                break;
            }
            const auto end_ = iter_;
            skip_space( iter_ );
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::comment,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          else if( second_char == U'*' ) {
            std::u32string close_seq( U"*/" );
            const auto end_ = std::search( begin, end, close_seq.begin(), close_seq.end() );
            iter_ = boost::next( end_, 2 );
            skip_space( iter_ );
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::comment,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          else
            throw invalid_json();
        }
        else {
          auto begin = iter;
          skip_space( begin, end );
          for( ;; ++iter_ ) {
            if( iter_ == end )
              throw unexpected_end();
            if( *iter_ == U':' )
              break;
            else if( *iter_ == U'[' )
              break;
            else if( *iter_ == U']' )
              break;
            else if( *iter_ == U'\'' )
              break;
            else if( *iter_ == U'"' )
              break;
            else if( *iter_ == U'{' )
              break;
            else if( *iter_ == U'}' )
              break;
            else if( *iter_ == U' ' )
              break;
            else if( *iter_ == U'\t' )
              break;
            else if( *iter_ == U'\x0a' )
              break;
            else if( *iter_ == U'\x0d' )
              break;
            else if( *iter_ == U',' )
              break;
          }
          const auto end_ = iter_;
          bool force_string = false;
          skip_space( iter_, end );
          if( force_string ) {
            return std::make_pair( json::json< boost::iterator_range< Iterator > >(
              json::json_event::string,
              boost::make_iterator_range( begin, end_ )
            ), iter_ );
          }
          switch( first_char ) {
            case U'n': {
              if( std::distance( begin, end_ ) == 4u && std::equal( begin, end_, U"null" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::null,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            case U'+':
            case U'-':
            case U'.':
            case U'I':
            case U'i':
            case U'N':
            case U'0':
            case U'1':
            case U'2':
            case U'3':
            case U'4':
            case U'5':
            case U'6':
            case U'7':
            case U'8':
            case U'9':
              return parse_json_number( begin, end_, iter_ );
            case U't': {
              if( std::distance( begin, end_ ) == 4u && std::equal( begin, end_, U"true" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::boolean,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            case U'f': {
              if( std::distance( begin, end_ ) == 5u && std::equal( begin, end_, U"false" ) )
                return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                  json::json_event::boolean,
                  boost::make_iterator_range( begin, end_ )
                ), iter_ );
            }
            default: {
              return std::make_pair( json::json< boost::iterator_range< Iterator > >(
                json::json_event::string,
                boost::make_iterator_range( begin, end_ )
              ), iter_ );
            }
          };
        }
      }

      template< typename BaseIterator >
      class json_decoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public json_decoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename json_decoding_iterator_base_type< BaseIterator >::type base_type;
        json_decoding_iterator() {}
        json_decoding_iterator( const BaseIterator &iter ) :
          base_type( iter ) {
          skip_space( base_type::base_reference() );
          dereference_ = std::bind( &dereference_json< BaseIterator >, std::placeholders::_1 );
        }
        json_decoding_iterator( const BaseIterator &iter, const BaseIterator &end ) :
          base_type( iter ) {
          skip_space( base_type::base_reference() );
          dereference_ = std::bind( &dereference_json_with_end< BaseIterator >, std::placeholders::_1, end );
        }
        typename json_decoding_iterator_value_type< BaseIterator >::type dereference() const {
          const auto dereferenced = dereference_( base_type::base_reference() );
          next_iter = dereferenced.second;
          return dereferenced.first;
        }
        void increment() {
          if( !next_iter ) {
            base_type::base_reference() = *next_iter;
            next_iter = boost::none;
          }
          else {
            base_type::base_reference() = dereference_( base_type::base_reference() ).second;
          }
        }
        bool equal( const json_decoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
      private:
        std::function<
          std::pair<
            typename json_decoding_iterator_value_type< BaseIterator >::type,
            BaseIterator
          >( const BaseIterator& )
        > dereference_;
        mutable boost::optional< BaseIterator > next_iter;
      };

      template< typename BaseIterator >
      json_decoding_iterator< BaseIterator > decode_json( const BaseIterator &iter ) {
        return json_decoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      json_decoding_iterator< BaseIterator > decode_json( const BaseIterator &iter, const BaseIterator &end ) {
        return json_decoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

