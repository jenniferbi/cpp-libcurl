include_directories("${CMAKE_SOURCE_DIR}/lib/include")

# Wildcard additions to add sources
file(GLOB SOURCES "./*.cpp")

# Add executable that's built from ./*.cpp files
foreach (testsourcefile ${SOURCES})
  get_filename_component(testname ${testsourcefile} NAME_WE)
  add_executable ( ${testname} ${testsourcefile} )
  target_link_libraries( ${testname} LINK_PUBLIC Curlpp)
  endforeach (testsourcefile ${SOURCES})
#add_executable(simple simple.cpp)

# Link to Curlpp library
#target_link_libraries(simple LINK_PUBLIC Curlpp)
