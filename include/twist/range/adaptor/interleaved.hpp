#ifndef TWIST_RANGE_ADAPTOR_INTERLEAVED_HPP
#define TWIST_RANGE_ADAPTOR_INTERLEAVED_HPP

#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/algorithm/auxiliary/copy.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/iterator.hpp>
#include <twist/iterator/adaptor/interleaved.hpp>
#include <twist/fusion/get_vector_of.hpp>
#include <twist/fusion/get_begin_sequence.hpp>
#include <twist/fusion/get_end_sequence.hpp>
#include <twist/utils/is_range.hpp>
namespace twist {
  namespace range {
    namespace adaptor {
      template< typename ... Ranges >
      boost::iterator_range<
        twist::iterator::adaptor::interleaved<
          typename twist::fusion::get_vector_of<
            typename fusion::get_begin_sequence< Ranges... >::result_type
          >::type
        >
      > interleave( Ranges &... ranges ) {
        typename twist::fusion::get_vector_of<
          typename fusion::get_begin_sequence< Ranges... >::result_type
        >::type begin_;
        boost::fusion::copy( fusion::get_begin_sequence< Ranges... >()( ranges... ), begin_ );
        typename twist::fusion::get_vector_of<
          typename fusion::get_end_sequence< Ranges... >::result_type
        >::type end_;
        boost::fusion::copy( fusion::get_end_sequence< Ranges... >()( ranges... ), end_ );
        return boost::make_iterator_range(
          twist::iterator::adaptor::interleave( begin_ ),
          twist::iterator::adaptor::interleave( end_ )
        );
      }
      template< typename ... Ranges >
      boost::iterator_range<
        twist::iterator::adaptor::interleaved<
          typename twist::fusion::get_vector_of<
            typename fusion::get_begin_sequence< Ranges... >::result_type
          >::type
        >
      > interleave( const Ranges &... ranges ) {
        typename twist::fusion::get_vector_of<
          typename fusion::get_begin_sequence< Ranges... >::result_type
        >::type begin_;
        boost::fusion::copy( fusion::get_begin_sequence< Ranges... >()( ranges... ), begin_ );
        typename twist::fusion::get_vector_of<
          typename fusion::get_end_sequence< Ranges... >::result_type
        >::type end_;
        boost::fusion::copy( fusion::get_end_sequence< Ranges... >()( ranges... ), end_ );
        return boost::make_iterator_range(
          twist::iterator::adaptor::interleave( begin_ ),
          twist::iterator::adaptor::interleave( end_ )
        );
      }
    }
  }
}

#endif

