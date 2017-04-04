cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

# Android stuff
if (ANDROID_NDK)
  # TODO: why do we need 3.6.0?
  cmake_minimum_required (VERSION 3.6.0 FATAL_ERROR)

  # TODO: shouldn't this file be somewhere in the NDK? Where's the source for this?
  set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/test/pegtl/android.toolchain.cmake")

  set (ANDROID_ABI "x86" CACHE STRING "")
  set (ANDROID_TOOLCHAIN "clang" CACHE STRING "")
  set (ANDROID_NATIVE_API_LEVEL "android-16" CACHE STRING "")
endif ()

# set project and version
if (ANDROID_NDK)
  project (PEGTL VERSION 2.0.0)
else ()
  project (PEGTL VERSION 2.0.0 LANGUAGES CXX)
endif ()

# options
option (PEGTL_BUILD_TESTS "Build test programs" ON)
option (PEGTL_BUILD_EXAMPLES "Build example programs" ON)

# set C++ language standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# define a header-only library
add_library (PEGTL INTERFACE)
target_include_directories (PEGTL INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# testing
if (PEGTL_BUILD_TESTS)
  enable_testing ()
  add_subdirectory (src/test/pegtl)
endif ()

# examples
if (PEGTL_BUILD_EXAMPLES)
  add_subdirectory (src/example/pegtl)
endif ()

# install
install (DIRECTORY include/ DESTINATION include)
install (FILES LICENSE DESTINATION include/tao/pegtl)
