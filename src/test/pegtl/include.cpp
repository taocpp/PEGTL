// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/analyze_traits.hpp>
#include <tao/pegtl/apply_mode.hpp>
#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/buffer.hpp>
#include <tao/pegtl/config.hpp>
#include <tao/pegtl/count_position.hpp>
#include <tao/pegtl/demangle.hpp>
#include <tao/pegtl/enums.hpp>
#include <tao/pegtl/forward.hpp>
#include <tao/pegtl/inputs.hpp>
#include <tao/pegtl/int16.hpp>
#include <tao/pegtl/int32.hpp>
#include <tao/pegtl/int64.hpp>
#include <tao/pegtl/int8.hpp>
#include <tao/pegtl/match.hpp>
#include <tao/pegtl/member.hpp>
#include <tao/pegtl/normal.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/parse.hpp>
#include <tao/pegtl/parse_error.hpp>
#include <tao/pegtl/parse_error_base.hpp>
#include <tao/pegtl/pegtl_string.hpp>
#include <tao/pegtl/pointer_position.hpp>
#include <tao/pegtl/position_with_source.hpp>
#include <tao/pegtl/print.hpp>
#include <tao/pegtl/rewind_mode.hpp>
#include <tao/pegtl/rules.hpp>
#include <tao/pegtl/system.hpp>
#include <tao/pegtl/tags.hpp>
#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/text_position_with_source.hpp>
#include <tao/pegtl/type_list.hpp>
#include <tao/pegtl/uint16.hpp>
#include <tao/pegtl/uint32.hpp>
#include <tao/pegtl/uint64.hpp>
#include <tao/pegtl/uint8.hpp>
#include <tao/pegtl/unicode.hpp>
#include <tao/pegtl/utf16.hpp>
#include <tao/pegtl/utf32.hpp>
#include <tao/pegtl/utf8.hpp>
#include <tao/pegtl/version.hpp>
#include <tao/pegtl/visit.hpp>

#include <tao/pegtl/action/add_guard.hpp>
#include <tao/pegtl/action/add_state.hpp>
#include <tao/pegtl/action/change_action.hpp>
#include <tao/pegtl/action/change_action_and_state.hpp>
#include <tao/pegtl/action/change_action_and_states.hpp>
#include <tao/pegtl/action/change_control.hpp>
#include <tao/pegtl/action/change_state.hpp>
#include <tao/pegtl/action/change_states.hpp>
#include <tao/pegtl/action/control_action.hpp>
#include <tao/pegtl/action/disable_action.hpp>
#include <tao/pegtl/action/enable_action.hpp>
#include <tao/pegtl/action/require_apply.hpp>
#include <tao/pegtl/action/require_apply0.hpp>

#include <tao/pegtl/buffer/analyze_traits.hpp>
#include <tao/pegtl/buffer/buffer_common.hpp>
#include <tao/pegtl/buffer/buffer_input.hpp>
#include <tao/pegtl/buffer/buffer_input_with_source.hpp>
#include <tao/pegtl/buffer/buffers.hpp>
#include <tao/pegtl/buffer/cstream_reader.hpp>
#include <tao/pegtl/buffer/cstring_reader.hpp>
#include <tao/pegtl/buffer/discard.hpp>
#include <tao/pegtl/buffer/discard_input.hpp>
#include <tao/pegtl/buffer/discard_input_on.hpp>
#include <tao/pegtl/buffer/dynamic_buffer.hpp>
#include <tao/pegtl/buffer/endless_reader.hpp>
#include <tao/pegtl/buffer/has_direct_reader.hpp>
#include <tao/pegtl/buffer/inputs.hpp>
#include <tao/pegtl/buffer/is_buffer.hpp>
#include <tao/pegtl/buffer/istream_reader.hpp>
#include <tao/pegtl/buffer/require.hpp>
#include <tao/pegtl/buffer/static_buffer.hpp>
#include <tao/pegtl/buffer/text_buffer_input.hpp>
#include <tao/pegtl/buffer/text_buffer_input_with_source.hpp>

#include <tao/pegtl/control/input_control.hpp>
#include <tao/pegtl/control/must_if.hpp>
#include <tao/pegtl/control/remove_first_state.hpp>
#include <tao/pegtl/control/remove_last_states.hpp>
#include <tao/pegtl/control/shuffle_states.hpp>
#include <tao/pegtl/control/state_control.hpp>

int main()
{
   static_assert( ' ' == 32 );
   static_assert( '0' == 48 );
   static_assert( 'a' == 97 );
   static_assert( 'Z' == 90 );
   static_assert( '~' == 126 );

   static_assert( sizeof( char ) == 1 );
   static_assert( sizeof( char16_t ) == 2 );
   static_assert( sizeof( char32_t ) == 4 );

   return 0;
}

