cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

set(example_sources
  abnf2pegtl.cpp
  analyze.cpp
  calculator.cpp
  chomsky_hierarchy.cpp
  coverage.cpp
  csv1.cpp
  csv2.cpp
  dynamic_match.cpp
  hello_world.cpp
  indent_aware.cpp
  json_build.cpp
  json_count.cpp
  json_parse.cpp
  lua53_parse.cpp
  modulus_match.cpp
  parse_tree.cpp
  parse_tree_user_state.cpp
  print.cpp
  print_rt.cpp
  print_with_subs.cpp
  proto3.cpp
  recover.cpp
  s_expression.cpp
  sum.cpp
  symbol_table.cpp
  unescape.cpp
  uri.cpp
  uri_trace.cpp
)

# file(GLOB ...) is used to validate the above list of test_sources
file(GLOB glob_example_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach(examplesourcefile ${example_sources})
  if(${examplesourcefile} IN_LIST glob_example_sources)
    list(REMOVE_ITEM glob_example_sources ${examplesourcefile})
  else()
    message(SEND_ERROR "File ${examplesourcefile} is missing from src/example/pegtl")
  endif()

  get_filename_component(exename pegtl-example-${examplesourcefile} NAME_WE)
  add_executable(${exename} ${examplesourcefile})
  target_link_libraries(${exename} PRIVATE taocpp::pegtl)
  set_target_properties(${exename} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  if(MSVC)
    target_compile_options(${exename} PRIVATE /W4 /WX /utf-8)
  else()
    target_compile_options(${exename} PRIVATE -pedantic -Wall -Wextra -Wshadow -Werror)
  endif()
endforeach(examplesourcefile)

if(glob_example_sources)
  foreach(ignored_source_file ${glob_example_sources})
    message(SEND_ERROR "File ${ignored_source_file} in src/example/pegtl is ignored")
  endforeach(ignored_source_file)
endif()
