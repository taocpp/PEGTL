cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)

project(pegtl VERSION 2.8.0 LANGUAGES CXX)

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

# features required by the PEGTL
target_compile_features(pegtl INTERFACE
  cxx_alias_templates
  cxx_auto_type
  cxx_constexpr
  cxx_decltype
  cxx_default_function_template_args
  cxx_defaulted_functions
  cxx_delegating_constructors
  cxx_deleted_functions
  cxx_explicit_conversions
  cxx_generalized_initializers
  cxx_inheriting_constructors
  cxx_inline_namespaces
  cxx_noexcept
  cxx_nonstatic_member_init
  cxx_nullptr
  cxx_range_for
  cxx_rvalue_references
  cxx_static_assert
  cxx_strong_enums
  cxx_template_template_parameters
  cxx_trailing_return_types
  cxx_uniform_initialization
  cxx_variadic_macros
  cxx_variadic_templates
)

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

# install and export target
install(TARGETS pegtl EXPORT pegtl-targets)

install(EXPORT pegtl-targets
  FILE pegtl-config.cmake
  NAMESPACE taocpp::
  DESTINATION ${PEGTL_INSTALL_CMAKE_DIR}
)

install(DIRECTORY include/ DESTINATION ${PEGTL_INSTALL_INCLUDE_DIR})
install(FILES LICENSE DESTINATION ${PEGTL_INSTALL_DOC_DIR})
