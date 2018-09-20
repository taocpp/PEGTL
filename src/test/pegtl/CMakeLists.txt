cmake_minimum_required (VERSION 3.3.0 FATAL_ERROR)

set (test_sources
  actions_one.cpp
  actions_three.cpp
  actions_two.cpp
  analyze_cycles.cpp
  argv_input.cpp
  ascii_classes.cpp
  ascii_eol.cpp
  ascii_eolf.cpp
  ascii_forty_two.cpp
  ascii_identifier.cpp
  ascii_istring.cpp
  ascii_keyword.cpp
  ascii_shebang.cpp
  ascii_string.cpp
  ascii_three.cpp
  ascii_two.cpp
  contrib_alphabet.cpp
  contrib_integer.cpp
  contrib_if_then.cpp
  contrib_json.cpp
  contrib_parse_tree.cpp
  contrib_raw_string.cpp
  contrib_rep_one_min_max.cpp
  contrib_to_string.cpp
  contrib_tracer.cpp
  contrib_unescape.cpp
  contrib_uri.cpp
  data_cstring.cpp
  demangle.cpp
  file_cstream.cpp
  file_file.cpp
  file_istream.cpp
  file_mmap.cpp
  file_read.cpp
  internal_endian.cpp
  internal_file_mapper.cpp
  internal_file_opener.cpp
  pegtl_string_t.cpp
  position.cpp
  rule_action.cpp
  rule_apply0.cpp
  rule_apply.cpp
  rule_at.cpp
  rule_bof.cpp
  rule_bol.cpp
  rule_bytes.cpp
  rule_control.cpp
  rule_disable.cpp
  rule_enable.cpp
  rule_eof.cpp
  rule_failure.cpp
  rule_if_apply.cpp
  rule_if_must.cpp
  rule_if_must_else.cpp
  rule_if_then_else.cpp
  rule_list.cpp
  rule_list_must.cpp
  rule_list_tail.cpp
  rule_minus.cpp
  rule_must.cpp
  rule_not_at.cpp
  rule_opt.cpp
  rule_opt_must.cpp
  rule_pad.cpp
  rule_pad_opt.cpp
  rule_plus.cpp
  rule_rep.cpp
  rule_rep_max.cpp
  rule_rep_min.cpp
  rule_rep_min_max.cpp
  rule_rep_opt.cpp
  rule_require.cpp
  rule_seq.cpp
  rule_sor.cpp
  rule_star.cpp
  rule_star_must.cpp
  rule_state.cpp
  rule_success.cpp
  rule_try_catch.cpp
  rule_until.cpp
  tester.cpp
  uint16_general.cpp
  uint32_general.cpp
  uint64_general.cpp
  uint8_general.cpp
  utf16_general.cpp
  utf32_general.cpp
  utf8_general.cpp
)

# file (GLOB ...) is used to validate the above list of test_sources
file (GLOB glob_test_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach (testsourcefile ${test_sources})
  if (${testsourcefile} IN_LIST glob_test_sources)
    list (REMOVE_ITEM glob_test_sources ${testsourcefile})
  else ()
    message (SEND_ERROR "File ${testsourcefile} is missing from src/test/pegtl")
  endif ()

  get_filename_component (exename ${testsourcefile} NAME_WE)
  set (exename "pegtl-test-${exename}")
  add_executable (${exename} ${testsourcefile})
  target_link_libraries (${exename} PRIVATE taocpp::pegtl)
  set_target_properties (${exename} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )
  if (MSVC)
    target_compile_options (${exename} PRIVATE /W4 /WX /utf-8)
  else ()
    target_compile_options (${exename} PRIVATE -pedantic -Wall -Wextra -Wshadow -Werror)
  endif ()
  if (ANDROID)
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../.. COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} "-DTEST_RESOURCES=src/test/pegtl/data;src/test/pegtl/file_data.txt;Makefile" -DTEST_RESOURCES_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../../.. -DUNITTEST=${CMAKE_CURRENT_BINARY_DIR}/${exename} -DTEST_PARAMETER=-all -P ${CMAKE_CURRENT_SOURCE_DIR}/ExecuteOnAndroid.cmake)
  else ()
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../.. COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${exename})
  endif ()
endforeach (testsourcefile)

if (glob_test_sources)
  foreach (ignored_source_file ${glob_test_sources})
    message (SEND_ERROR "File ${ignored_source_file} in src/test/pegtl is ignored")
  endforeach (ignored_source_file)
endif ()
