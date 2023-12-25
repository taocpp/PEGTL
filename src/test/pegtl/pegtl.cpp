// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/add_guard.hpp>
#include <tao/pegtl/add_state.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/analyze_traits.hpp>
#include <tao/pegtl/apply_mode.hpp>
#include <tao/pegtl/ascii.hpp>
#include <tao/pegtl/buffer.hpp>
#include <tao/pegtl/change_action.hpp>
#include <tao/pegtl/change_action_and_state.hpp>
#include <tao/pegtl/change_action_and_states.hpp>
#include <tao/pegtl/change_control.hpp>
#include <tao/pegtl/change_state.hpp>
#include <tao/pegtl/change_states.hpp>
#include <tao/pegtl/config.hpp>
#include <tao/pegtl/control_action.hpp>
#include <tao/pegtl/count_position.hpp>
#include <tao/pegtl/demangle.hpp>
#include <tao/pegtl/disable_action.hpp>
#include <tao/pegtl/discard_input.hpp>
#include <tao/pegtl/discard_input_on_failure.hpp>
#include <tao/pegtl/discard_input_on_success.hpp>
#include <tao/pegtl/enable_action.hpp>
#include <tao/pegtl/enums.hpp>
#include <tao/pegtl/eol.hpp>
#include <tao/pegtl/forward.hpp>
#include <tao/pegtl/inputs.hpp>
#include <tao/pegtl/int16.hpp>
#include <tao/pegtl/int32.hpp>
#include <tao/pegtl/int64.hpp>
#include <tao/pegtl/int8.hpp>
#include <tao/pegtl/invert_traits.hpp>
#include <tao/pegtl/line_view_at.hpp>
#include <tao/pegtl/match.hpp>
#include <tao/pegtl/member.hpp>
#include <tao/pegtl/must_if.hpp>
#include <tao/pegtl/normal.hpp>
#include <tao/pegtl/nothing.hpp>
#include <tao/pegtl/parse.hpp>
#include <tao/pegtl/parse_error.hpp>
#include <tao/pegtl/parse_error_base.hpp>
#include <tao/pegtl/pointer_position.hpp>
#include <tao/pegtl/position_with_source.hpp>
#include <tao/pegtl/print.hpp>
#include <tao/pegtl/remove_first_state.hpp>
#include <tao/pegtl/remove_last_states.hpp>
#include <tao/pegtl/require_apply.hpp>
#include <tao/pegtl/require_apply0.hpp>
#include <tao/pegtl/rewind_mode.hpp>
#include <tao/pegtl/rules.hpp>
#include <tao/pegtl/tags.hpp>
#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/text_position_with_source.hpp>
#include <tao/pegtl/type_list.hpp>
#include <tao/pegtl/uint16.hpp>
#include <tao/pegtl/uint32.hpp>
#include <tao/pegtl/uint64.hpp>
#include <tao/pegtl/uint8.hpp>
#include <tao/pegtl/utf16.hpp>
#include <tao/pegtl/utf32.hpp>
#include <tao/pegtl/utf8.hpp>
#include <tao/pegtl/version.hpp>
#include <tao/pegtl/visit.hpp>

int main()
{
   static_assert( ' ' == 32 );
   static_assert( 'a' == 97 );
   static_assert( 'Z' == 90 );
   static_assert( '~' == 126 );

   static_assert( sizeof( char ) == 1 );
   static_assert( sizeof( char16_t ) == 2 );
   static_assert( sizeof( char32_t ) == 4 );

   return 0;
}

