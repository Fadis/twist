#include <vector>
#include <twist/utils/bit_queue.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utils_bit_queue
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE( bit_queue ) {
  twist::utils::bit_queue< 8, 6 > queue;
  queue.push( 0xFE );
  BOOST_CHECK_EQUAL( queue.top(), 0x3F );
  queue.pop();
  queue.push( 0x58 );
  BOOST_CHECK_EQUAL( queue.top(), 0x25 );
  queue.pop();
  queue.push( 0xF0 );
  BOOST_CHECK_EQUAL( queue.top(), 0x23 );
  queue.pop();
  BOOST_CHECK_EQUAL( queue.top(), 0x30 );
  queue.pop();
}

