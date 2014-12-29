#ifndef TWIST_UTILS_BIT_QUEUE_HPP
#define TWIST_UTILS_BIT_QUEUE_HPP

#include <boost/integer.hpp>

namespace twist {
  namespace utils {
    template< size_t from_size, size_t to_size, size_t buffer_size = from_size + to_size >
      class bit_queue {
        public:
          struct not_ready {};
          typedef typename boost::uint_t< buffer_size >::least internal;
          bit_queue() : queue( 0ul ), length( 0ul ) {}
          bool full() const {
            return buffer_size - length < from_size;
          }
          bool empty() const {
            return length < to_size;
          }
          size_t size() const {
            return length;
          }
          void push( typename boost::uint_t< from_size >::least value ) {
            if( full() )
              throw not_ready();
            length += from_size;
            queue <<= from_size;
            queue |= static_cast< internal >( value ) & ( ( 1u << from_size ) - 1u );
          }
          typename boost::uint_t< to_size >::least top() const {
            if( empty() )
              throw not_ready();
            return ( queue >> ( length - to_size ) ) & ( ( 1u << to_size ) - 1u );
          }
          void pop() {
            if( !empty() )
            length -= to_size;
          }
          void swap( bit_queue< from_size, to_size, buffer_size > &right ) {
            std::swap( queue, right.queue );
            std::swap( length, right.length );
          }
        private:
          internal queue;
          size_t length;
      };
  }
}

#endif

