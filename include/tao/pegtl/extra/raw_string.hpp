// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_RAW_STRING_HPP
#define TAO_PEGTL_EXTRA_RAW_STRING_HPP

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"
#include "../internal/until.hpp"

#include "internal/raw_string.hpp"

#include "remove_first_state.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // raw_string matches Lua-style long literals.
   //
   // The following description was taken from the Lua documentation
   // (see http://www.lua.org/docs.html):
   //
   // - An "opening long bracket of level n" is defined as an opening square
   //   bracket followed by n equal signs followed by another opening square
   //   bracket. So, an opening long bracket of level 0 is written as `[[`,
   //   an opening long bracket of level 1 is written as `[=[`, and so on.
   // - A "closing long bracket" is defined similarly; for instance, a closing
   //   long bracket of level 4 is written as `]====]`.
   // - A "long literal" starts with an opening long bracket of any level and
   //   ends at the first closing long bracket of the same level. It can
   //   contain any text except a closing bracket of the same level.
   // - Literals in this bracketed form can run for several lines, do not
   //   interpret any escape sequences, and ignore long brackets of any other
   //   level.
   // - For convenience, when the opening long bracket is eagerly followed
   //   by a newline, the newline is not included in the string.
   //
   // Note that unlike Lua's long literal, a raw_string is customizable to use
   // other characters than `[`, `=` and `]` for matching. Also note that Lua
   // introduced newline-specific replacements in Lua 5.2, which we do not
   // support on the grammar level.

   // An action should usually not be attached to a raw_string but to its
   // content so that the action_input the apply() function receives only
   // contains the actual content of the string without the opening and
   // closing sequences. The content type is also used for error-reporting
   // when a raw string is not closed properly or has invalid content.

   template< char Open, char Marker, char Close, typename... Contents >
   struct raw_string
      : internal::raw_string< Open, Marker, Close, internal::until< internal::at_raw_string_close< Marker, Close >, remove_first_state< Contents... > > >
   {};

   template< char Open, char Marker, char Close >
   struct raw_string< Open, Marker, Close >
      : internal::raw_string< Open, Marker, Close, internal::until< internal::at_raw_string_close< Marker, Close > > >
   {};

   template< typename Name, char Open, char Marker >
   struct analyze_traits< Name, internal::raw_string_open< Open, Marker > >
      : analyze_any_traits<>
   {};

   template< typename Name, char Marker, char Close >
   struct analyze_traits< Name, internal::at_raw_string_close< Marker, Close > >
      : analyze_opt_traits<>
   {};

   template< typename Name, char Open, char Marker, char Close, typename Content >
   struct analyze_traits< Name, internal::raw_string< Open, Marker, Close, Content > >
      : analyze_seq_traits< internal::raw_string_open< Open, Marker >, Content >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
