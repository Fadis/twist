ADD_EXECUTABLE( resizable_array resizable_array.cpp )
target_link_libraries( resizable_array ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
add_test( resizable_array resizable_array )
