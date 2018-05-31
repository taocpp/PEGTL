# PEGTL cmake config

# Sets the following:
# PEGTL_FOUND - true
# PEGTL_VERSION - Version number in Major.Minor.Release format
# PEGTL_INCLUDE_DIR - include directory
# PEGTL_INCLUDE_DIRS - alias of PEGTL_INCLUDE_DIR

# Leaves the following blank
# PEGTL_DEFINITIONS
# PEGTL_LIBRARY
# PEGTL_LIBRARIES

get_filename_component(PEGTL_INSTALL_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../.." PATH)
set(PEGTL_FOUND TRUE)
set(PEGTL_VERSION "2.5.2")
set(PEGTL_INCLUDE_DIR ${PEGTL_INSTALL_DIR}/include)
set(PEGTL_INCLUDE_DIRS ${PEGTL_INSTALL_DIR}/include)
