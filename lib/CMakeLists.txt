# Bring headers into project
include_directories(include)

# Wildcard additions to add sources
file(GLOB SOURCES "src/*.cpp")

# Create library called Curlpp which includes all .cpp files in lib
add_library(Curlpp ${SOURCES})

# Make sure teh compiler can find include files for Curlpp library
# when other libraries or executables link to Curlpp
target_include_directories(Curlpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
