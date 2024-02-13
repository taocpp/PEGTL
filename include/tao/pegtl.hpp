// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_HPP
#define TAO_PEGTL_HPP

#include "pegtl/ascii.hpp"
#include "pegtl/config.hpp"
#include "pegtl/demangle.hpp"
#include "pegtl/forward.hpp"
#include "pegtl/inputs.hpp"
#include "pegtl/parse.hpp"
#include "pegtl/print.hpp"
#include "pegtl/rules.hpp"
#include "pegtl/tags.hpp"
#include "pegtl/utf8.hpp"
#include "pegtl/version.hpp"
#include "pegtl/visit.hpp"

#include "pegtl/add_guard.hpp"
#include "pegtl/add_state.hpp"
#include "pegtl/change_action.hpp"
#include "pegtl/change_action_and_state.hpp"
#include "pegtl/change_action_and_states.hpp"
#include "pegtl/change_control.hpp"
#include "pegtl/change_state.hpp"
#include "pegtl/change_states.hpp"
#include "pegtl/control_action.hpp"
#include "pegtl/disable_action.hpp"
#include "pegtl/enable_action.hpp"
#include "pegtl/remove_first_state.hpp"
#include "pegtl/remove_last_states.hpp"

#if defined( __cpp_exceptions )
#include "pegtl/must_if.hpp"
#include "pegtl/parse_error.hpp"
#include "pegtl/parse_error_base.hpp"
#endif

#include "pegtl/count_position.hpp"
#include "pegtl/pointer_position.hpp"
#include "pegtl/position_with_source.hpp"
#include "pegtl/text_position.hpp"
#include "pegtl/text_position_with_source.hpp"

#include "pegtl/pegtl_string.hpp"

#endif
