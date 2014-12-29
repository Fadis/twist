#ifndef TWIST_ITERATOR_ADAPTOR_JSON_STRING_DECODING_HPP
#define TWIST_ITERATOR_ADAPTOR_JSON_STRING_DECODING_HPP

#include <iterator>
#include <functional>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/exceptions.hpp>

namespace twist {
  namespace iterator {
    namespace adaptor {
      template<
        typename BaseIterator,
        typename Enable = void
      > class json_string_decoding_iterator {};

      template< typename BaseIterator >
      struct json_string_decoding_iterator_value_type {
        typedef char32_t type;
      };

      template< typename BaseIterator, typename Enable = void >
      struct json_string_decoding_iterator_base_type {};

      template< typename BaseIterator >
      struct json_string_decoding_iterator_base_type<
        BaseIterator
      > {
        typedef boost::iterator_adaptor<
          json_string_decoding_iterator< BaseIterator >,
          BaseIterator,
          typename json_string_decoding_iterator_value_type< BaseIterator >::type,
          boost::forward_traversal_tag,
          typename json_string_decoding_iterator_value_type< BaseIterator >::type
        > type;
      };

      template< typename Iterator >
      std::pair< char32_t, Iterator > dereference_json_string( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t first_char = *iter_;
        char32_t result;
        if( first_char != U'\\' ) {
          result = first_char;
        }
        else {
          ++iter_;
          const char32_t second_char = *iter_;
          if( second_char == U'"' ) {
            result = U'"';
            ++iter_;
          }
          else if( second_char == U'\'' ) {
            result = U'\'';
            ++iter_;
          }
          else if( second_char == U'/' ) {
            result = U'/';
            ++iter_;
          }
          else if( second_char == U'b' ) {
            result = U'\b';
            ++iter_;
          }
          else if( second_char == U'n' ) {
            result = U'\n';
            ++iter_;
          }
          else if( second_char == U'f' ) {
            result = U'\f';
            ++iter_;
          }
          else if( second_char == U'r' ) {
            result = U'\r';
            ++iter_;
          }
          else if( second_char == U't' ) {
            result = U'\t';
            ++iter_;
          }
          else if( second_char == U'\n' ) {
            result = U'\n';
            ++iter_;
          }
          else if( second_char == U'u' ) {
            ++iter_;
            const auto end = std::next( iter_ , 4 );
            namespace qi = boost::spirit::qi;
            qi::uint_parser< char32_t, 16, 1, 4 > hex4_p;
            char32_t high_surrogate;
            if( qi::parse( iter_, end, hex4_p, high_surrogate ) ) {
              if( ( high_surrogate & 0xFC00 ) == 0xD800 ) {
                if( *iter_ == U'\\' ) {
                  ++iter_;
                  if( *iter_ == U'u' ) {
                    ++iter_;
                    char32_t low_surrogate;
                    if( qi::parse( iter_, end, hex4_p, low_surrogate ) ) {
                      if( ( low_surrogate & 0xFC00 ) == 0xDC00 ) {
                        result = ( ( ( high_surrogate & 0x03FF ) << 10 ) |
                                 ( low_surrogate & 0x03FF ) ) + 0x10000;
                      }
                      else
                        throw invalid_json_string_sequence();
                    }
                    else
                      throw invalid_json_string_sequence();
                  }
                  else
                    throw invalid_json_string_sequence();
                }
                else
                  throw invalid_json_string_sequence();
              }
              else
                result = high_surrogate; 
            }
            else
              throw invalid_json_string_sequence();
          }
          else
            return dereference_json_string( iter_ );
        }
        return std::make_pair( result, iter_ );
      }

      template< typename Iterator >
      std::pair< char32_t, Iterator > dereference_json_string_with_end( const Iterator &iter, const Iterator &end ) {
        Iterator iter_ = iter;
        if( iter_ == end )
          throw invalid_json_string_sequence();
        const char32_t first_char = *iter_;
        char32_t result;
        if( first_char != U'\\' ) {
          result = first_char;
        }
        else {
          ++iter_;
          if( iter_ == end )
            throw invalid_json_string_sequence();
          const char32_t second_char = *iter_;
          if( second_char == U'"' ) {
            result = U'"';
            ++iter_;
          }
          else if( second_char == U'\'' ) {
            result = U'\'';
            ++iter_;
          }
          else if( second_char == U'/' ) {
            result = U'/';
            ++iter_;
          }
          else if( second_char == U'b' ) {
            result = U'\b';
            ++iter_;
          }
          else if( second_char == U'n' ) {
            result = U'\n';
            ++iter_;
          }
          else if( second_char == U'f' ) {
            result = U'\f';
            ++iter_;
          }
          else if( second_char == U'r' ) {
            result = U'\r';
            ++iter_;
          }
          else if( second_char == U't' ) {
            result = U'\t';
            ++iter_;
          }
          else if( second_char == U'\n' ) {
            result = U'\n';
            ++iter_;
          }
          else if( second_char == U'u' ) {
            ++iter_;
            namespace qi = boost::spirit::qi;
            qi::uint_parser< char32_t, 16, 1, 4 > hex4_p;
            char32_t high_surrogate;
            if( qi::parse( iter_, end, hex4_p, high_surrogate ) ) {
              if( ( high_surrogate & 0xFC00 ) == 0xD800 ) {
                if( iter_ == end )
                  throw invalid_json_string_sequence();
                else if( *iter_ == U'\\' ) {
                  ++iter_;
                  if( iter_ == end )
                    throw invalid_json_string_sequence();
                  else if( *iter_ == U'u' ) {
                    ++iter_;
                    char32_t low_surrogate;
                    if( qi::parse( iter_, end, hex4_p, low_surrogate ) ) {
                      if( ( low_surrogate & 0xFC00 ) == 0xDC00 ) {
                        result = ( ( ( high_surrogate & 0x03FF ) << 10 ) |
                                 ( low_surrogate & 0x03FF ) ) + 0x10000;
                      }
                      else
                        throw invalid_json_string_sequence();
                    }
                    else
                      throw invalid_json_string_sequence();
                  }
                  else
                    throw invalid_json_string_sequence();
                }
                else
                  throw invalid_json_string_sequence();
              }
              else
                result = high_surrogate; 
            }
            else
              throw invalid_json_string_sequence();
          }
          else
            return dereference_json_string_with_end( iter_, end );
        }
        return std::make_pair( result,  iter_ );
      }

      template< typename Iterator >
      Iterator next_json_string_iterator_with_end( const Iterator &iter, const Iterator &end ) {
        Iterator iter_ = iter;
        if( iter_ == end )
          throw invalid_json_string_sequence();
        const char32_t first_char = *iter_;
        if( first_char != U'\\' ) {
          return std::next( iter_, 1 );
        }
        else {
          ++iter_;
          if( iter_ == end )
            throw invalid_json_string_sequence();
          const char32_t second_char = *iter_;
          if( second_char == U'"' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'\'' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'/' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'b' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'n' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'f' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'r' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U't' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'\n' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'u' ) {
            ++iter_;
            namespace qi = boost::spirit::qi;
            qi::uint_parser< char32_t, 16, 1, 4 > hex4_p;
            char32_t high_surrogate;
            if( qi::parse( iter_, end, hex4_p, high_surrogate ) ) {
              if( ( high_surrogate & 0xFC00 ) == 0xD800 ) {
                if( iter_ == end )
                  throw invalid_json_string_sequence();
                else if( *iter_ == U'\\' ) {
                  ++iter_;
                  if( iter_ == end )
                    throw invalid_json_string_sequence();
                  if( *iter_ == U'u' ) {
                    ++iter_;
                    char32_t low_surrogate;
                    if( qi::parse( iter_, end, hex4_p, low_surrogate ) ) {
                      if( ( low_surrogate & 0xFC00 ) == 0xDC00 )
                        return iter_;
                      else
                        throw invalid_json_string_sequence();
                    }
                    else
                      throw invalid_json_string_sequence();
                  }
                  else
                    throw invalid_json_string_sequence();
                }
                else
                  throw invalid_json_string_sequence();
              }
              else
                return iter_;
            }
            else
              throw invalid_json_string_sequence();
          }
          else
            return next_json_string_iterator_with_end( iter_, end );
        }
      }

      template< typename Iterator >
      Iterator next_json_string_iterator( const Iterator &iter ) {
        Iterator iter_ = iter;
        const char32_t first_char = *iter_;
        if( first_char != U'\\' ) {
          return std::next( iter_, 1 );
        }
        else {
          ++iter_;
          const char32_t second_char = *iter_;
          if( second_char == U'"' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'\'' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'/' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'b' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'n' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'f' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'r' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U't' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'\n' ) {
            return std::next( iter_, 1 );
          }
          else if( second_char == U'u' ) {
            ++iter_;
            const auto end = std::next( iter_ , 4 );
            namespace qi = boost::spirit::qi;
            qi::uint_parser< char32_t, 16, 1, 4 > hex4_p;
            char32_t high_surrogate;
            if( qi::parse( iter_, end, hex4_p, high_surrogate ) ) {
              if( ( high_surrogate & 0xFC00 ) == 0xD800 ) {
                if( *iter_ == U'\\' ) {
                  ++iter_;
                  if( *iter_ == U'u' ) {
                    ++iter_;
                    char32_t low_surrogate;
                    if( qi::parse( iter_, end, hex4_p, low_surrogate ) ) {
                      if( ( low_surrogate & 0xFC00 ) == 0xDC00 )
                        return iter_;
                      else
                        throw invalid_json_string_sequence();
                    }
                    else
                      throw invalid_json_string_sequence();
                  }
                  else
                    throw invalid_json_string_sequence();
                }
                else
                  throw invalid_json_string_sequence();
              }
              else
                return iter_;
            }
            else
              throw invalid_json_string_sequence();
          }
          else
            return next_json_string_iterator( iter_ );
        }
      }
      
      template< typename BaseIterator >
      class json_string_decoding_iterator<
        BaseIterator,
        typename boost::enable_if<
          twist::iterator::is_convertible_to_forward_traversal< BaseIterator >
        >::type
      > :
        public json_string_decoding_iterator_base_type< BaseIterator >::type {
      public:
        typedef typename json_string_decoding_iterator_base_type< BaseIterator >::type base_type;
        json_string_decoding_iterator() {}
        json_string_decoding_iterator( const BaseIterator &iter ) :
          base_type( iter ) {
          dereference_ = std::bind( &dereference_json_string< BaseIterator >, std::placeholders::_1 );
          next_ = std::bind( &next_json_string_iterator< BaseIterator >, std::placeholders::_1 );
        }
        json_string_decoding_iterator( const BaseIterator &iter, const BaseIterator &end ) :
          base_type( iter ) {
          dereference_ = std::bind( &dereference_json_string_with_end< BaseIterator >, std::placeholders::_1, end );
          next_ = std::bind( &next_json_string_iterator_with_end< BaseIterator >, std::placeholders::_1, end );
        }
        typename json_string_decoding_iterator_value_type< BaseIterator >::type dereference() const {
          const auto dereferenced = dereference_( base_type::base_reference() );
          next_iter = dereferenced.second;
          return dereferenced.first;
        }
        void increment() {
          if( !next_iter ) {
            base_type::base_reference() = *next_iter;
            next_iter = boost::none;
          }
          else
            base_type::base_reference() = next_( base_type::base_reference() );
        }
        bool equal( const json_string_decoding_iterator< BaseIterator > &y ) const {
          return y.base_reference() == base_type::base_reference();
        }
      private:
        std::function<
          std::pair<
            typename json_string_decoding_iterator_value_type< BaseIterator >::type,
            BaseIterator
          >( const BaseIterator& )
        > dereference_;
        std::function< BaseIterator( const BaseIterator& ) >
          next_;
        mutable boost::optional< BaseIterator > next_iter;
      };

      template< typename BaseIterator >
      json_string_decoding_iterator< BaseIterator > decode_json_string( const BaseIterator &iter ) {
        return json_string_decoding_iterator< BaseIterator >( iter );
      }
      template< typename BaseIterator >
      json_string_decoding_iterator< BaseIterator > decode_json_string( const BaseIterator &iter, const BaseIterator &end ) {
        return json_string_decoding_iterator< BaseIterator >( iter, end );
      }
    }
  }
}

#endif

