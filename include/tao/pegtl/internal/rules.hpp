// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RULES_HPP
#define TAO_PEGTL_INTERNAL_RULES_HPP

#include "action.hpp"
#include "any.hpp"
#include "apply.hpp"
#include "apply0.hpp"
#include "ascii_istring.hpp"
#include "ascii_string.hpp"
#include "at.hpp"
#include "bof.hpp"
#include "bol.hpp"
#include "consume.hpp"
#include "control.hpp"
#include "disable.hpp"
#include "enable.hpp"
#include "eof.hpp"
#include "eol.hpp"
#include "eolf.hpp"
#include "everything.hpp"
#include "failure.hpp"
#include "function.hpp"
#include "identifier.hpp"
#include "if_apply.hpp"
#include "if_then_else.hpp"
#include "ione.hpp"
#include "many.hpp"
#include "nested.hpp"
#include "not_at.hpp"
#include "not_ione.hpp"
#include "not_one.hpp"
#include "not_range.hpp"
#include "one.hpp"
#include "opt.hpp"
#include "partial.hpp"
#include "plus.hpp"
#include "range.hpp"
#include "ranges.hpp"
#include "rematch.hpp"
#include "rep.hpp"
#include "rep_min_max.hpp"
#include "rep_opt.hpp"
#include "seq.hpp"
#include "sor.hpp"
#include "star.hpp"
#include "star_partial.hpp"
#include "star_strict.hpp"
#include "state.hpp"
#include "strict.hpp"
#include "success.hpp"
#include "tester.hpp"
#include "unicode_string.hpp"
#include "until.hpp"
#include "utf_one.hpp"
#include "utf_string.hpp"

#include "../config.hpp"

#if defined( __cpp_exceptions )
#include "if_must.hpp"
#include "must.hpp"
#include "raise.hpp"
#include "try_catch_raise_nested.hpp"
#include "try_catch_return_false.hpp"
#endif

// FCOV_EXCL_FILE
#endif
