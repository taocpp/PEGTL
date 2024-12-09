// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_TRACE_TRAITS_HPP
#define TAO_PEGTL_DEBUG_TRACE_TRAITS_HPP

#include <cstddef>
#include <string_view>

#include "../normal.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< bool HideInternal = true,
             bool UseColor = true,
             bool PrintSourceLine = false,
             std::size_t IndentIncrement = 2,
             std::size_t InitialIndent = 8 >
   struct trace_traits
   {
      template< typename Rule >
      static constexpr bool enable = ( HideInternal ? normal< Rule >::enable : true );

      static constexpr std::size_t initial_indent = InitialIndent;
      static constexpr std::size_t indent_increment = IndentIncrement;

      static constexpr bool print_source_line = PrintSourceLine;

      static constexpr std::string_view ansi_reset = UseColor ? "\033[m" : "";
      static constexpr std::string_view ansi_rule = UseColor ? "\033[36m" : "";
      static constexpr std::string_view ansi_hide = UseColor ? "\033[37m" : "";

      static constexpr std::string_view ansi_position = UseColor ? "\033[1;34m" : "";
      static constexpr std::string_view ansi_success = UseColor ? "\033[32m" : "";
      static constexpr std::string_view ansi_failure = UseColor ? "\033[31m" : "";
      static constexpr std::string_view ansi_raise = UseColor ? "\033[1;31m" : "";
      static constexpr std::string_view ansi_unwind = UseColor ? "\033[31m" : "";
      static constexpr std::string_view ansi_apply = UseColor ? "\033[1;36m" : "";
   };

   using standard_trace_traits = trace_traits< true >;
   using complete_trace_traits = trace_traits< false >;

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
