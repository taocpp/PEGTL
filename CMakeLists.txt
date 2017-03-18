cmake_minimum_required (VERSION 3.6.0 FATAL_ERROR)

if (PEGTL_PLATFORM STREQUAL "Android")
  # this must be before project()
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/android.toolchain.cmake")
  set(ANDROID_ABI "x86" CACHE STRING "")
  set(ANDROID_TOOLCHAIN "clang" CACHE STRING "")
  set(ANDROID_NATIVE_API_LEVEL "android-16" CACHE STRING "")
endif()

# set project and version
project (PEGTL VERSION 2.0.0 LANGUAGES CXX)

# set C++ language standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# set warning options
if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /utf-8")
else ()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
endif ()

# define a header-only library
add_library (PEGTL INTERFACE)
target_include_directories (PEGTL INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# TODO: installation

# testing
enable_testing ()
file (GLOB testsources unit_tests/*.cc)
foreach (testsourcefile ${testsources})
  get_filename_component (exename ${testsourcefile} NAME_WE)
  add_executable (${exename} ${testsourcefile})
  target_link_libraries (${exename} PEGTL)
  if (PEGTL_PLATFORM STREQUAL "Android")
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/run-on-android.sh ${CMAKE_BINARY_DIR}/${exename})
  else ()
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND ${CMAKE_BINARY_DIR}/${exename})
  endif ()
endforeach (testsourcefile)
