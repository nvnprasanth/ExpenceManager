cmake_minimum_required(VERSION 2.8)

project("tinyxml")

# enable fortran, c, and c++ language
enable_language(C CXX)

# project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

# example how to set c++ compiler flags for GNU
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
	set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL  -Wall -Wno-unknown-pragmas -Wno-format")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

# location of header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml)

# build executable
add_library(tinyxml tinyxml/tinyxml.cpp tinyxml/tinystr.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp )
target_include_directories (tinyxml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
