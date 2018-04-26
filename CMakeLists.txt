cmake_minimum_required(VERSION 3.11.1)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(cpp-libcurl)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -ftemplate-depth-256 -std=c++14 -DASIO_STANDALONE")
# These linker flags are not called here
# set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-g -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt")

# Verbose makefile
set (CMAKE_VERBOSE_MAKEFILE OFF)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/bin")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build/results")

# for subdirectory cmakelists reference see: https://cmake.org/examples/
add_subdirectory(lib)
add_subdirectory(examples)
