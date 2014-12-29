ADD_EXECUTABLE( bit_queue bit_queue.cpp )
target_link_libraries( bit_queue ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
add_test( bit_queue bit_queue )
