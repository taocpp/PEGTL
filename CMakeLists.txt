cmake_minimum_required( VERSION 2.8.0 FATAL_ERROR )
project( PEGTL )

message( STATUS "The C++ compiler identification is: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "The underlying C++ compiler is: ${CMAKE_CXX_COMPILER}" )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /W4" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    set( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )
    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -g" )
    set( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )
    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -DNDEBUG -g" )
    set( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror" )
else()
    message( FATAL_ERROR "Unknown C++ compiler." )
endif()

enable_testing()

include_directories( ${CMAKE_SOURCE_DIR} )

file( GLOB CC_SOURCES unit_tests/*.cc )

foreach( testsourcefile ${CC_SOURCES} )
    get_filename_component( exename ${testsourcefile} NAME_WE )
    add_executable( ${exename} ${testsourcefile} )
	add_test( NAME ${exename} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${CMAKE_BINARY_DIR}/${exename} INPUT_FILE ${CMAKE_SOURCE_DIR}/unit_tests/file_data.txt )
endforeach( testsourcefile ${APP_SOURCES} )
