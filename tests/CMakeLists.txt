find_package(Boost REQUIRED COMPONENTS timer)

include_directories("${CMAKE_SOURCE_DIR}/lib/include" ${Boost_INCLUDE_DIRS})

# Wildcard additions to add sources
file(GLOB SOURCES "./*.cpp")

# Add executable that's built from ./*.cpp files
foreach (testsourcefile ${SOURCES})
  get_filename_component(testname ${testsourcefile} NAME_WE)
  add_executable ( ${testname} ${testsourcefile} )
  target_link_libraries( ${testname} LINK_PUBLIC Curlpp curl ${Boost_LIBRARIES})
  endforeach (testsourcefile ${SOURCES})
