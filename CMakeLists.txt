cmake_minimum_required(VERSION 3.11.1)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(cpp-libcurl)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -ftemplate-depth-256 -std=c++14 -DASIO_STANDALONE")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-g -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt")

# Verbose makefile
set (CMAKE_VERBOSE_MAKEFILE OFF)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin")

# for subdirectory cmakelists reference see: https://cmake.org/examples/
add_subdirectory(lib)
add_subdirectory(examples)

# Brings the headers into project
#include_directories(include)

# Wildcard additions to add sources
#file(GLOB SOURCES "src/*.cpp")

#add_library(http SHARED ${SOURCES})
#add_executable(simple ${SOURCES})
