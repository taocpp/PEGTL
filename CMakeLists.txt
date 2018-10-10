cmake_minimum_required (VERSION 3.8.0 FATAL_ERROR)

project (pegtl VERSION 3.0.0 LANGUAGES CXX)

# installation directories
set (PEGTL_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set (PEGTL_INSTALL_DOC_DIR "share/doc/tao/pegtl" CACHE STRING "The installation doc directory")
set (PEGTL_INSTALL_CMAKE_DIR "share/pegtl/cmake" CACHE STRING "The installation cmake directory")

# define a header-only library
add_library (pegtl INTERFACE)
add_library (taocpp::pegtl ALIAS pegtl)
target_include_directories (pegtl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${PEGTL_INSTALL_INCLUDE_DIR}>
)

# require C++17
target_compile_features (pegtl INTERFACE cxx_std_17)

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

# install and export target
install (TARGETS pegtl EXPORT pegtl-targets)

install (EXPORT pegtl-targets
  FILE pegtl-config.cmake
  NAMESPACE taocpp::
  DESTINATION ${PEGTL_INSTALL_CMAKE_DIR}
)

install (DIRECTORY include/ DESTINATION ${PEGTL_INSTALL_INCLUDE_DIR})
install (FILES LICENSE DESTINATION ${PEGTL_INSTALL_DOC_DIR})
