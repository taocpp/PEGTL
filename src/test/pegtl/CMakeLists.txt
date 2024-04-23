cmake_minimum_required(VERSION 3.8...3.19)

set(test_sources
  action_add_guard.cpp
  action_apply_1.cpp
  action_apply_2.cpp
  action_apply_3.cpp
  action_change_action_and_state.cpp
  action_change_action_and_states.cpp
  action_change_state.cpp
  action_change_states.cpp
  action_control_action.cpp
  action_enable_disable.cpp
  action_match_lazy.cpp
  action_match_text.cpp
  apply_mode.cpp
  argv_input.cpp
  ascii_atomics.cpp
  ascii_chars.cpp
  ascii_classes.cpp
  ascii_eol.cpp
  ascii_eolf.cpp
  ascii_identifier.cpp
  ascii_istring.cpp
  ascii_keyword.cpp
  ascii_not_one.cpp
  ascii_not_range.cpp
  ascii_one.cpp
  ascii_range.cpp
  ascii_shebang.cpp
  ascii_string.cpp
  ascii_three.cpp
  ascii_two.cpp
  buffer_buffer_common.cpp
  buffer_cstream_input.cpp
  buffer_cstring_input.cpp
  buffer_discard_input.cpp
  buffer_dynamic_buffer.cpp
  buffer_istream_input.cpp
  buffer_rule_discard.cpp
  buffer_rule_is_buffer.cpp
  buffer_rule_require.cpp
  buffer_static_buffer.cpp
  contrib_alphabet.cpp
  contrib_check_count.cpp
  contrib_check_depth.cpp
  contrib_http.cpp
  contrib_if_then.cpp
  contrib_input_with_offset.cpp
  contrib_integer.cpp
  contrib_iri.cpp
  contrib_json.cpp
  contrib_limit_count.cpp
  contrib_nested_exceptions.cpp
  contrib_raw_string.cpp
  contrib_record.cpp
  contrib_rep_one_min_max.cpp
  contrib_rep_string.cpp
  contrib_separated_seq.cpp
  contrib_type_to_string.cpp
  contrib_unescape.cpp
  contrib_uri.cpp
  control_remove_first_state.cpp
  control_remove_last_states.cpp
  control_rewind_control.cpp
  control_rewind_state_control.cpp
  control_state_control.cpp
  control_unwind.cpp
  debug_analyze.cpp
  debug_analyze_traits_enum.cpp
  debug_contains.cpp
  debug_coverage.cpp
  debug_print.cpp
  debug_trace_1.cpp
  debug_trace_2.cpp
  debug_trace_3.cpp
  debug_visit.cpp
  demangle.cpp
  enable_control.cpp
  error_message_1.cpp
  error_message_2.cpp
  error_message_3.cpp
  file_input.cpp
  icu_utf16_base.cpp
  icu_utf32_base.cpp
  icu_utf8_base.cpp
  include.cpp
  internal_action_input.cpp
  internal_any.cpp
  internal_ascii_utility.cpp
  internal_base_inputs.cpp
  internal_byteswap.cpp
  internal_char_scan_traits.cpp
  internal_copy_input.cpp
  internal_data_and_size.cpp
  internal_endian.cpp
  internal_extract_position.cpp
  internal_file_input.cpp
  internal_file_mapper.cpp
  internal_file_opener.cpp
  internal_get_eol_rule_char.cpp
  internal_inputs_lines.cpp
  internal_integer_size.cpp
  internal_lazy_inputs.cpp
  internal_lazy_scan_traits.cpp
  internal_math_utility.cpp
  internal_mmap_input.cpp
  internal_peek_ascii.cpp
  internal_peek_data.cpp
  internal_peek_direct.cpp
  internal_peek_endian16.cpp
  internal_peek_endian8.cpp
  internal_peek_masked.cpp
  internal_peek_member.cpp
  internal_peek_signed.cpp
  internal_peek_unsigned.cpp
  internal_peek_utf16.cpp
  internal_peek_utf32.cpp
  internal_peek_utf8.cpp
  internal_read_file_stdio.cpp
  internal_read_input.cpp
  internal_set_stack_guard.cpp
  internal_stream_to_string.cpp
  internal_text_inputs.cpp
  internal_text_rule_traits.cpp
  internal_text_scan_traits.cpp
  internal_type_traits.cpp
  internal_unhex_utility.cpp
  internal_unwind_guard.cpp
  internal_utf32_to_utf8.cpp
  internal_utf_one.cpp
  internal_value_list_concat.cpp
  internal_vector_stack_guard.cpp
  internal_view_input.cpp
  mmap_input.cpp
  parse_error.cpp
  pegtl_string_t.cpp
  read_input.cpp
  rewind_mode.cpp
  rule_action.cpp
  rule_apply.cpp
  rule_apply0.cpp
  rule_ascii_many.cpp
  rule_at.cpp
  rule_bof.cpp
  rule_bol.cpp
  rule_combine.cpp
  rule_consume.cpp
  rule_control.cpp
  rule_disable.cpp
  rule_enable.cpp
  rule_eof.cpp
  rule_everything.cpp
  rule_failure.cpp
  rule_function.cpp
  rule_if_apply.cpp
  rule_if_must.cpp
  rule_if_must_else.cpp
  rule_if_then_else.cpp
  rule_invert.cpp
  rule_list.cpp
  rule_list_must.cpp
  rule_list_tail.cpp
  rule_minus.cpp
  rule_must.cpp
  rule_nested.cpp
  rule_not_at.cpp
  rule_opt.cpp
  rule_opt_must.cpp
  rule_pad.cpp
  rule_pad_opt.cpp
  rule_partial.cpp
  rule_plus.cpp
  rule_raise.cpp
  rule_rematch.cpp
  rule_rep.cpp
  rule_rep_max.cpp
  rule_rep_min.cpp
  rule_rep_min_max.cpp
  rule_rep_opt.cpp
  rule_seq.cpp
  rule_sor.cpp
  rule_star.cpp
  rule_star_must.cpp
  rule_star_partial.cpp
  rule_state.cpp
  rule_strict.cpp
  rule_success.cpp
  rule_try_catch_raise_nested.cpp
  rule_try_catch_return_false.cpp
  rule_unicode_string.cpp
  rule_until.cpp
  rule_utf8_string.cpp
  test_empty.cpp
  test_result.cpp
  test_setup.cpp
  text_position.cpp
  uint16_general.cpp
  uint32_general.cpp
  uint64_general.cpp
  uint8_general.cpp
  unaligned_access.cpp
  utf16_general.cpp
  utf32_general.cpp
  utf8_general.cpp
)

# file(GLOB ...) is used to validate the above list of test_sources
file(GLOB glob_test_sources RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

foreach(testsourcefile ${test_sources})
  if(${testsourcefile} IN_LIST glob_test_sources)
    list(REMOVE_ITEM glob_test_sources ${testsourcefile})
  else()
    message(SEND_ERROR "File ${testsourcefile} is missing from src/test/pegtl")
  endif()

  get_filename_component(exename pegtl-test-${testsourcefile} NAME_WE)
  add_executable(${exename} ${testsourcefile})
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
  if(ANDROID)
    add_test(NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_COMMAND} "-DANDROID_NDK=${ANDROID_NDK}" "-DTEST_RESOURCES_DIR=${CMAKE_SOURCE_DIR}" "-DTEST_RESOURCES=src/test/pegtl/data;src/test/pegtl/file_data.txt;Makefile" "-DUNITTEST=${exename}" -P ${CMAKE_CURRENT_SOURCE_DIR}/ExecuteOnAndroid.cmake)
  else()
    add_test(NAME ${exename} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${exename})
  endif()
endforeach()

if(glob_test_sources)
  foreach(ignored_source_file ${glob_test_sources})
    message(SEND_ERROR "File ${ignored_source_file} in src/test/pegtl is ignored")
  endforeach()
endif()
