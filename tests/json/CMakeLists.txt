ADD_EXECUTABLE( json json.cpp )
target_link_libraries( json ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${ZLIB_LIBRARIES} )
add_test( json json )
