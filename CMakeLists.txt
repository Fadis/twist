cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/modules/" /opt/sprout/share/cmake/Modules)

PROJECT(twist)

INCLUDE (CheckIncludeFiles)
include (FindPkgConfig)
ENABLE_TESTING()
# build type
set(CMAKE_BUILD_TYPE Release)

if (CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

# compile option (Debug) 
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -pedantic -std=c++11 ")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-unused-parameter -pedantic -Werror")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -pedantic -Werror -std=c++11 ")
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -Wno-array-bounds -pedantic -Werror")
endif()

# compile option (Release)
if (CMAKE_COMPILER_IS_CLANGXX)
  set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -Wno-unused-parameter -pedantic -std=c++11 -O3 ")
  set(CMAKE_C_FLAGS_RELEASE "-W -Wall -Wextra -Wno-unused-parameter -pedantic -Werror -O3")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -Wextra -Wno-array-bounds -pedantic -Werror -std=c++11 -O3 ")
  set(CMAKE_C_FLAGS_RELEASE "-W -Wall -Wextra -Wno-array-bounds -pedantic -Werror -O3")
endif()

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.49.0 COMPONENTS thread filesystem system unit_test_framework REQUIRED )
if (NOT Boost_FOUND)
  message( FATAL_ERROR "Boost not found")
endif (NOT Boost_FOUND)

#find_package(ZLIB REQUIRED)
#if (NOT ZLIB_FOUND)
#  message( FATAL_ERROR "zlib not found")
#endif (NOT ZLIB_FOUND)
#find_package(Sprout REQUIRED)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include ${Boost_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIRS}
)
#LINK_DIRECTORIES( ${LLVM_LIB_DIR}  )
subdirs( include tests )
