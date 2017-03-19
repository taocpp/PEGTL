cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

include (${CMAKE_CURRENT_SOURCE_DIR}/pegtl.cmake)

# modify flags for testing
set (CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /utf-8")
else ()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wshadow -Werror")
endif ()

# testing
enable_testing ()
file (GLOB testsources unit_tests/*.cc)
foreach (testsourcefile ${testsources})
  get_filename_component (exename ${testsourcefile} NAME_WE)
  add_executable (${exename} ${testsourcefile})
  target_link_libraries (${exename} PEGTL)
  add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${exename})
endforeach (testsourcefile)
