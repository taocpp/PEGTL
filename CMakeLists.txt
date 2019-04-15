cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

# Read version from version.hpp
file(READ "${CMAKE_CURRENT_LIST_DIR}/include/tao/pegtl/version.hpp" version_hpp_data)
string(REGEX MATCH "#define TAO_PEGTL_VERSION \"([^\"]+)\"" _ ${version_hpp_data})
set(PEGTL_VERSION "${CMAKE_MATCH_1}")

project(pegtl VERSION ${PEGTL_VERSION} LANGUAGES CXX)

if(${PROJECT_NAME}_FOUND)
  # Multiple versions of PEGTL can't co-exist
  if(NOT ${PROJECT_NAME}_VERSION STREQUAL ${PROJECT_VERSION})
    message(FATAL_ERROR "Multiple mismatched PEGTL versions")
  endif()

  # Only include if this is the first include
  if(NOT ${PROJECT_NAME}_DIR STREQUAL "${PROJECT_BINARY_DIR}")
    return()
  endif()
endif()

# Keep track of pegtl version
set(${PROJECT_NAME}_FOUND TRUE CACHE BOOL "" FORCE)
set(${PROJECT_NAME}_VERSION "${PROJECT_VERSION}" CACHE STRING "" FORCE)
set(${PROJECT_NAME}_DIR "${PROJECT_BINARY_DIR}" CACHE PATH "" FORCE)

mark_as_advanced(${PROJECT_NAME}_FOUND)
mark_as_advanced(${PROJECT_NAME}_VERSION)
mark_as_advanced(${PROJECT_NAME}_DIR)

# installation directories
set(PEGTL_INSTALL_INCLUDE_DIR "include" CACHE STRING "The installation include directory")
set(PEGTL_INSTALL_DOC_DIR "share/doc/tao/pegtl" CACHE STRING "The installation doc directory")
set(PEGTL_INSTALL_CMAKE_DIR "share/pegtl/cmake" CACHE STRING "The installation cmake directory")

# define a header-only library
add_library(pegtl INTERFACE)
add_library(taocpp::pegtl ALIAS pegtl)
target_include_directories(pegtl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${PEGTL_INSTALL_INCLUDE_DIR}>
)

# require C++17
target_compile_features(pegtl INTERFACE cxx_std_17)

# testing
enable_testing()
option(PEGTL_BUILD_TESTS "Build test programs" ON)
if(PEGTL_BUILD_TESTS)
  add_subdirectory(src/test/pegtl)
endif()

# examples
option(PEGTL_BUILD_EXAMPLES "Build example programs" ON)
if(PEGTL_BUILD_EXAMPLES)
  add_subdirectory(src/example/pegtl)
endif()

# Make package findable
configure_file(cmake/dummy-config.cmake.in pegtl-config.cmake @ONLY)

# install and export target
install(TARGETS pegtl EXPORT pegtl-targets)

install(EXPORT pegtl-targets
  FILE pegtl-config.cmake
  NAMESPACE taocpp::
  DESTINATION ${PEGTL_INSTALL_CMAKE_DIR}
)

install(DIRECTORY include/ DESTINATION ${PEGTL_INSTALL_INCLUDE_DIR})
install(FILES LICENSE DESTINATION ${PEGTL_INSTALL_DOC_DIR})
