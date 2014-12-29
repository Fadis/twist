#ifndef TWIST_BASE64_ENCODER_HPP
#define TWIST_BASE64_ENCODER_HPP

#include <array>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/crc.hpp>
#include <twist/iterator/traversal.hpp>
#include <twist/base64/base64_traits.hpp>

namespace twist {
  namespace base64 {
    template< typename Base64Traits >
    class encoding_table {
    public:
      char operator()( uint8_t value ) const {
        return convert( value ); 
      }
    private:
      static char convert( uint8_t value ) {
        static const std::array< char, 64u > map{{
          'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
          'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
          'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', twist::base64::plus< Base64Traits >::value, twist::base64::slash< Base64Traits >::value
        }};
        return map[ static_cast< size_t >( value ) ];
      }
    };

    namespace state {
      struct value;
      struct padding;
      struct cr;
      struct lf;
      struct crc_pre;
      struct crc;
      struct end;
      struct check_line_length;
      struct check_crc_length;
      struct check_padding_length;
    }
    namespace callback {
      template< typename Fsm >
      struct dereference_base {
        typedef char result_type;
        virtual result_type operator()( const Fsm & ) const = 0;
      };
      template< typename Fsm, typename State, typename Enable = void >
      struct dereference {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          throw std::out_of_range( "out_of_range" );
        }
      };

      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::value, State > >::type > : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm &fsm ) const {
          uint8_t raw = 0;
          if( fsm.step % 4u == 0u )
            raw = static_cast< uint8_t >( *fsm.iter ) >> 2;
          else if( fsm.step % 4u == 3u )
            raw = static_cast< uint8_t >( *fsm.iter ) & 0x3F;
          else if( std::next( fsm.iter ) == fsm.end ) {
            if( fsm.step % 4u == 1u )
              raw = ( static_cast< uint8_t >( *fsm.iter ) << 4 ) & 0x3F;
            else if( fsm.step % 4u == 2u )
              raw = ( static_cast< uint8_t >( *fsm.iter ) << 2 ) & 0x3F;
          }
          else {
            const auto next_fragment = std::next( fsm.iter );
            if( fsm.step % 4u == 1u )
              raw = ( ( static_cast< uint8_t >( *fsm.iter ) << 4 ) & 0x3F ) |
                    ( static_cast< uint8_t >( *next_fragment ) >> 4 );
            else if( fsm.step % 4u == 2u )
              raw = ( ( static_cast< uint8_t >( *fsm.iter ) << 2 ) & 0x3F )|
                    ( static_cast< uint8_t >( *next_fragment ) >> 6 );
          }
          return fsm.table( raw );
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::padding, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          return twist::base64::padding< typename Fsm::Base64Traits >::value;
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::cr, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          return '\x0d';
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::lf, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          return '\x0a';
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::crc_pre, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          return '=';
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::crc, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm &fsm ) const {
          return fsm.table( ( fsm.crc() >> ( 6 * ( 3 - fsm.step ) ) ) & 0x3F );
        }
      };
      template< typename Fsm, typename State >
      struct dereference< Fsm, State, typename boost::enable_if< boost::is_same< state::end, State > >::type >  : public dereference_base< Fsm > {
        typedef char result_type;
        result_type operator()( const Fsm & ) const {
          throw std::out_of_range( "out_of_range" );
        }
      };
      template< typename Fsm >
      struct local_pos_base {
        typedef size_t result_type;
        virtual result_type operator()( const Fsm & ) const = 0;
      };
      template< typename Fsm, typename State, typename Enable = void >
      struct local_pos  : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm & ) const {
          throw std::out_of_range( "out_of_range" );
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::value, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm &fsm ) const {
          return ( 0u << 2 ) || ( fsm.step % 4u );
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::padding, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm &fsm ) const {
          return ( 1u << 2 ) || ( fsm.step % 4u );
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::cr, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm & ) const {
          return 2u << 2;
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::lf, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm & ) const {
          return 3u << 2;
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::crc_pre, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm & ) const {
          return 4u << 2;
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::crc, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm &fsm ) const {
          return ( 5u << 2 ) || ( fsm.step % 4u );
        }
      };
      template< typename Fsm, typename State >
      struct local_pos< Fsm, State, typename boost::enable_if< boost::is_same< state::end, State > >::type > : public local_pos_base< Fsm > {
        typedef size_t result_type;
        result_type operator()( const Fsm & ) const {
          return 0u;
        }
      };
    }
    namespace event {
      struct next{};
    }
    namespace action {
      template <typename TargetState, typename Fsm >
      void update_callbacks( boost::msm::back::state_machine< Fsm >& fsm ) {
        static const auto dereference = callback::dereference< Fsm, TargetState >();
        static const auto local_pos = callback::local_pos< Fsm, TargetState >();
        fsm.dereference = &dereference;
        fsm.local_pos = &local_pos;
      }
      struct next_value_with_crc {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
          if( fsm.step % 4u ) {
            fsm.crc( *fsm.iter );
            ++fsm.iter;
          }
          ++fsm.step;
        }
      };
      struct next_value {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm& fsm, SourceState&, TargetState & ) const {
          if( fsm.step % 4u )
            ++fsm.iter;
          ++fsm.step;
        }
      };
      struct next_crc {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm& fsm, SourceState&, TargetState & ) const {
          ++fsm.step;
        }
      };
      struct next_padding {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm& fsm, SourceState&, TargetState & ) const {
          ++fsm.step;
        }
      };
      struct reset_step {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
          fsm.step = 0u;
        }
      };
      struct out_of_range {
        template <class Event, class Fsm, class SourceState, class TargetState>
        void operator()( Event const&, Fsm&, SourceState&, TargetState& ) const {
          throw std::out_of_range( "out_of_range" );
        }
      };
    }
    namespace state {
      struct value : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< value >( fsm );
        }
      };
      struct padding : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< padding >( fsm );
        }
      };
      struct cr : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< cr >( fsm );
        }
      };
      struct lf : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< lf >( fsm );
        }
      };
      struct crc_pre : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< crc_pre >( fsm );
        }
      };
      struct crc : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< crc >( fsm );
        }
      };
      struct end : public boost::msm::front::state<>{
        template< typename Event, typename Fsm >
        void on_entry( Event const&, Fsm& fsm ) {
          action::update_callbacks< end >( fsm );
        }
      };
      struct check_line_length : public boost::msm::front::state<>{};
      struct check_crc_length : public boost::msm::front::state<>{};
      struct check_padding_length : public boost::msm::front::state<>{};
    }
    namespace guard {
      template<typename Base64Traits>
        struct newline {
          template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
              return ( fsm.step % twist::base64::max_length< Base64Traits >::value ) == 0u;
            }
        };
      struct end {
        template <class Event, class Fsm, class SourceState, class TargetState>
          bool operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
            return fsm.iter == fsm.end;
          }
      };
      struct crc_end {
        template <class Event, class Fsm, class SourceState, class TargetState>
          bool operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
            return fsm.step == 4u;
          }
      };
      struct padding_end {
        template <class Event, class Fsm, class SourceState, class TargetState>
        bool operator()( Event const&, Fsm& fsm, SourceState&, TargetState& ) const {
          return ( fsm.step % 4u ) == 0u;
        }
      };
    }
    typedef boost::msm::front::none none;
    template< typename Base64Traits, typename Iterator >
    struct end_sequence : public boost::mpl::vector<
      boost::msm::front::Row< state::end, event::next, state::end, action::out_of_range >
    > {};
    template< typename Base64Traits, typename Iterator, typename Enable = void >
    struct crc_sequence {};
    template< typename Base64Traits, typename Iterator >
    struct crc_sequence<
      Base64Traits, Iterator,
      typename boost::enable_if<
        twist::base64::checksum< Base64Traits >
      >::type
    > : public boost::mpl::vector<
      boost::msm::front::Row< state::crc_pre, event::next, state::crc, action::reset_step >,
      boost::msm::front::Row< state::crc, event::next, state::check_crc_length, action::next_crc >,
      boost::msm::front::Row< state::check_crc_length, none, state::crc, none, boost::msm::front::euml::Not_< guard::crc_end > >,
      boost::msm::front::Row< state::check_crc_length, none, state::end, none, guard::crc_end >
    > {};
    template< typename Base64Traits, typename Iterator >
    struct crc_sequence<
      Base64Traits, Iterator,
      typename boost::disable_if<
        twist::base64::checksum< Base64Traits >
      >::type
    > : public boost::mpl::vector<> {};
    template< typename Base64Traits, typename Iterator, typename Enable = void >
    struct newline_sequence {};
    template< typename Base64Traits, typename Iterator >
    struct newline_sequence<
      Base64Traits, Iterator,
      typename boost::enable_if<
        boost::mpl::not_equal_to<
          twist::base64::max_length< Base64Traits >,
          boost::mpl::size_t< boost::integer_traits< size_t >::const_max >
        >
      >::type
    > : public boost::mpl::vector<
      boost::msm::front::Row< state::cr, event::next, state::lf, none >,
      boost::msm::front::Row< state::lf, event::next, state::value, none, boost::msm::front::euml::Not_< guard::end > >,
      boost::msm::front::Row< state::lf, event::next,
        typename boost::mpl::if_< twist::base64::checksum< Base64Traits >, state::crc_pre, state::end >::type,
      none, guard::end >,
      boost::msm::front::Row< state::check_line_length, none, state::value, none, boost::msm::front::euml::Not_< guard::end > >,
      boost::msm::front::Row< state::check_line_length, none,
        typename boost::mpl::if_<
          boost::mpl::not_equal_to<
            twist::base64::padding< Base64Traits >,
            boost::mpl::char_< '\0' >
          >,
          state::check_padding_length,
          state::end
        >::type,
        none, guard::end
      >,
      boost::msm::front::Row< state::check_line_length, none, state::cr, none, guard::newline< Base64Traits > >
    > {};
    template< typename Base64Traits, typename Iterator >
    struct newline_sequence<
      Base64Traits, Iterator,
      typename boost::disable_if<
        boost::mpl::not_equal_to<
          twist::base64::max_length< Base64Traits >,
          boost::mpl::size_t< boost::integer_traits< size_t >::const_max >
        >
      >::type
    > : public boost::mpl::vector<
      boost::msm::front::Row< state::check_line_length, none, state::value, none, boost::msm::front::euml::Not_< guard::end > >,
      boost::msm::front::Row< state::check_line_length, none,
        typename boost::mpl::if_<
          boost::mpl::not_equal_to<
            twist::base64::padding< Base64Traits >,
            boost::mpl::char_< '\0' >
          >,
          state::check_padding_length,
          state::end
        >::type,
        none, guard::end
      >
    > {};
    template< typename Base64Traits, typename Iterator, typename Enable = void >
    struct padding_sequence {};
    template< typename Base64Traits, typename Iterator >
    struct padding_sequence<
      Base64Traits, Iterator,
      typename boost::enable_if<
        boost::mpl::not_equal_to<
          twist::base64::padding< Base64Traits >,
          boost::mpl::char_< '\0' >
        >
      >::type
    > : public boost::mpl::vector<
      boost::msm::front::Row< state::padding, event::next, state::check_padding_length, action::next_padding >,
      boost::msm::front::Row< state::check_padding_length, none, state::padding, none, boost::msm::front::euml::Not_< guard::padding_end > >,
      boost::msm::front::Row< state::check_padding_length, none,
        typename boost::mpl::if_< twist::base64::checksum< Base64Traits >,
          typename boost::mpl::if_<
            boost::mpl::not_equal_to<
              twist::base64::max_length< Base64Traits >,
              boost::mpl::size_t< boost::integer_traits< size_t >::const_max >
            >,
            state::cr,
            state::crc_pre
          >::type,
          state::end
        >::type, none, guard::padding_end
      >
    > {};
    template< typename Base64Traits, typename Iterator >
    struct padding_sequence<
      Base64Traits, Iterator,
      typename boost::disable_if<
        boost::mpl::not_equal_to<
          twist::base64::padding< Base64Traits >,
          boost::mpl::char_< '\0' >
        >
      >::type
    > : public boost::mpl::vector<> {};
    template< typename Base64Traits_, typename Iterator, typename Enable = void >
    struct state_machine {};
    template< typename Base64Traits_, typename Iterator >
    struct state_machine<
      Base64Traits_, Iterator,
      typename boost::enable_if<
        twist::base64::checksum< Base64Traits_ >
      >::type
    > : boost::msm::front::state_machine_def< state_machine< Base64Traits_, Iterator > > {
      typedef Base64Traits_ Base64Traits;
      typedef state_machine< Base64Traits, Iterator > Fsm;
      state_machine(
        size_t step_,
        const Iterator &iter_,
        const Iterator &end_
      ) : step( step_ ), iter( iter_ ), end( end_ ) {
        static const auto dereference_ = callback::dereference< Fsm, initial_state >();
        static const auto local_pos_ = callback::local_pos< Fsm, initial_state >();
        dereference = &dereference_;
        local_pos = &local_pos_;
      }
      const callback::dereference_base< Fsm > *dereference;
      const callback::local_pos_base< Fsm > *local_pos;
      char operator*() const {
        return (*dereference)( *this );
      }
      bool operator==( const Fsm &y ) const {
        return iter == y.iter && (*local_pos)( *this ) == (*y.local_pos)( y );
      }
      bool operator!=( const Fsm &y ) const {
        return iter != y.iter || (*local_pos)( *this ) != (*y.local_pos)( y );
      }
      typedef boost::mpl::vector<
        boost::msm::front::Row< state::value, event::next, state::check_line_length, action::next_value_with_crc >
      > transition_table_0;
      typedef typename boost::mpl::insert_range<
        transition_table_0,
        typename boost::mpl::end< transition_table_0 >::type,
        crc_sequence< Base64Traits, Iterator >
      >::type transition_table_1;
      typedef typename boost::mpl::insert_range<
        transition_table_1,
        typename boost::mpl::end< transition_table_1 >::type,
        end_sequence< Base64Traits, Iterator >
      >::type transition_table_2;
      typedef typename boost::mpl::insert_range<
        transition_table_2,
        typename boost::mpl::end< transition_table_2 >::type,
        newline_sequence< Base64Traits, Iterator >
      >::type transition_table_3;
      typedef typename boost::mpl::insert_range<
        transition_table_3,
        typename boost::mpl::end< transition_table_3 >::type,
        padding_sequence< Base64Traits, Iterator >
      >::type transition_table;
      typedef state::value initial_state;
      size_t step;
      Iterator iter;
      Iterator end;
      encoding_table< Base64Traits > table;
      boost::crc_optimal< 24, 0x864CFB, 0xB704CE, 0, false, false> crc;
    };
    template< typename Base64Traits_, typename Iterator >
    struct state_machine<
      Base64Traits_, Iterator,
      typename boost::enable_if<
        boost::mpl::not_< twist::base64::checksum< Base64Traits_ > >
      >::type
    > : boost::msm::front::state_machine_def< state_machine< Base64Traits_, Iterator > > {
      typedef Base64Traits_ Base64Traits;
      typedef state_machine< Base64Traits, Iterator > Fsm;
      state_machine(
        size_t step_,
        const Iterator &iter_,
        const Iterator &end_
      ) : step( step_ ), iter( iter_ ), end( end_ ) {
        static const auto dereference_ = callback::dereference< Fsm, initial_state >();
        static const auto local_pos_ = callback::local_pos< Fsm, initial_state >();
        dereference = &dereference_;
        local_pos = &local_pos_;
      } 
      const callback::dereference_base< Fsm > *dereference;
      const callback::local_pos_base< Fsm > *local_pos;
      char operator*() const {
        return (*dereference)( *this );
      }
      bool operator==( const Fsm &y ) const {
        return iter == y.iter && (*local_pos)( *this ) == (*y.local_pos)( y );
      }
      bool operator!=( const Fsm &y ) const {
        return iter != y.iter || (*local_pos)( *this ) != (*y.local_pos)( y );
      }
      typedef boost::mpl::vector<
        boost::msm::front::Row< state::value, event::next, state::check_line_length, action::next_value >
      > transition_table_1;
      typedef typename boost::mpl::insert_range<
        transition_table_1,
        typename boost::mpl::end< transition_table_1 >::type,
        end_sequence< Base64Traits, Iterator >
      >::type transition_table_2;
      typedef typename boost::mpl::insert_range<
        transition_table_2,
        typename boost::mpl::end< transition_table_2 >::type,
        newline_sequence< Base64Traits, Iterator >
      >::type transition_table_3;
      typedef typename boost::mpl::insert_range<
        transition_table_3,
        typename boost::mpl::end< transition_table_3 >::type,
        padding_sequence< Base64Traits, Iterator >
      >::type transition_table;
      typedef state::value initial_state;
      size_t step;
      Iterator iter;
      Iterator end;
      encoding_table< Base64Traits > table;
    };
  }
}

#endif

