// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_EXAMPLE_ABNF_ERRORS_HPP
#define TAO_PEGTL_SRC_EXAMPLE_ABNF_ERRORS_HPP

#include <tao/pegtl/config.hpp>
#include <tao/pegtl/control/must_if.hpp>

namespace TAO_PEGTL_NAMESPACE::abnf
{
   // Using must_if<> we define a control class which is used for
   // the parsing run instead of the default control class.

   // This improves the errors reported to the user.

   // The following turns local errors into global errors, i.e.
   // if one of the rules for which a custom error message is
   // defined fails, it throws a parse_error exception (aka global
   // failure) instead of returning false (aka local failure).

   // clang-format off
   template< typename > inline constexpr const char* error_message = nullptr;

   template<> inline constexpr auto error_message< comment_cont > = "unterminated comment";

   template<> inline constexpr auto error_message< quoted_string_cont > = "unterminated string (missing '\"')";
   template<> inline constexpr auto error_message< prose_val_cont > = "unterminated prose description (missing '>')";

   template<> inline constexpr auto error_message< hex_val::value > = "expected hexadecimal value";
   template<> inline constexpr auto error_message< dec_val::value > = "expected decimal value";
   template<> inline constexpr auto error_message< bin_val::value > = "expected binary value";
   template<> inline constexpr auto error_message< num_val_choice > = "expected base specifier (one of 'bBdDxX')";

   template<> inline constexpr auto error_message< option_close > = "unterminated option (missing ']')";
   template<> inline constexpr auto error_message< group_close > = "unterminated group (missing ')')";

   template<> inline constexpr auto error_message< req_repetition > = "expected element";
   template<> inline constexpr auto error_message< concatenation > = "expected element";
   template<> inline constexpr auto error_message< alternative > = "expected element";

   template<> inline constexpr auto error_message< defined_as > = "expected '=' or '=/'";
   template<> inline constexpr auto error_message< req_c_nl > = "unterminated rule";
   template<> inline constexpr auto error_message< rule > = "expected rule";
   // clang-format on

   struct error
   {
      template< typename Rule >
      static constexpr auto message = error_message< Rule >;
   };

   template< typename Rule >
   using control = must_if_n< error >::type< Rule >;

}  // namespace TAO_PEGTL_NAMESPACE::abnf

#endif
