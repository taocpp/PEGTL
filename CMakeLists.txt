cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

# Android stuff
if (ANDROID_NDK)
  # TODO: shouldn't this file be somewhere in the NDK? Where's the source for this?
  set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/test/pegtl/android.toolchain.cmake")

  set (ANDROID_ABI "x86" CACHE STRING "")
  set (ANDROID_TOOLCHAIN "clang" CACHE STRING "")
  set (ANDROID_NATIVE_API_LEVEL "android-16" CACHE STRING "")
endif ()

# set project and version
if (ANDROID_NDK)
  project (pegtl VERSION 2.0.0)
else ()
  project (pegtl VERSION 2.0.0 LANGUAGES CXX)
endif ()

# define a header-only library
add_library (pegtl INTERFACE)
add_library (taocpp::pegtl ALIAS pegtl)
target_include_directories (pegtl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# testing
enable_testing ()
option (PEGTL_BUILD_TESTS "Build test programs" ON)
if (PEGTL_BUILD_TESTS)
  add_subdirectory (src/test/pegtl)
endif ()

# examples
option (PEGTL_BUILD_EXAMPLES "Build example programs" ON)
if (PEGTL_BUILD_EXAMPLES)
  add_subdirectory (src/example/pegtl)
endif ()

# installation directories
set (PEGTL_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set (PEGTL_INSTALL_DOC_DIR "share/doc/tao/pegtl" CACHE STRING "The installation doc directory")

# install
install (DIRECTORY include/ DESTINATION ${PEGTL_INSTALL_INCLUDE_DIR})
install (FILES LICENSE DESTINATION ${PEGTL_INSTALL_DOC_DIR})
