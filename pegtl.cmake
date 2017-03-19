cmake_minimum_required (VERSION 3.2.0 FATAL_ERROR)

# set project and version
project (PEGTL VERSION 2.0.0 LANGUAGES CXX)

# set C++ language standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# define a header-only library
add_library (PEGTL INTERFACE)
target_include_directories (PEGTL INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# TODO: install
