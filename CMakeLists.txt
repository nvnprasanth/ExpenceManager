cmake_minimum_required(VERSION 2.8)

project("tally")

# enable c, and c++ language
enable_language(C CXX)

# project version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

# example how to set c++ compiler flags for GNU
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()


# location of header files
include_directories(
    ${PROJECT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tinyxml
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
message( STATUS ${PROJECT_BINARY_DIR} )
#source_dir [binary_dir] [EXCLUDE_FROM_ALL]				 
add_subdirectory (3rdParty)
 
# build executable
add_executable(tally src/main.cpp src/accounts.cpp src/user.cpp)
target_link_libraries(tally tinyxml)