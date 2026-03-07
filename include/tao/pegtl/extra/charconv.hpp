// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_CHARCONV_HPP
#define TAO_PEGTL_EXTRA_CHARCONV_HPP

#include <cstdint>

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"

#include "internal/charconv.hpp"

#include "overflow_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // The following types can be used as rule and/or action.
   // When the parse or action input can't be parsed as integer
   // it returns false in both roles, i.e. a local failure.
   // For integer overflow the overflow_mode determines whether
   // to return false or throw a parse_error exception.

   template< typename Integral, std::uint8_t Base, overflow_mode Over >
   struct from_chars_combo
      : internal::from_chars_rule< Integral, Base, Over >,
        internal::from_chars_action< Integral, Base, Over >
   {
      static_assert( Base != 1 );
   };

   template< typename Integral, std::uint8_t Base = 10 >
   struct from_chars_throws
      : from_chars_combo< Integral, Base, overflow_mode::global_failure >
   {};

   template< typename Integral, std::uint8_t Base = 10 >
   struct from_chars_nothrow
      : from_chars_combo< Integral, Base, overflow_mode::local_failure >
   {};

   template< typename Integral >
   struct from_xchars_throws
      : from_chars_throws< Integral, 16 >
   {};

   template< typename Integral >
   struct from_xchars_nothrow
      : from_chars_nothrow< Integral, 16 >
   {};

   template< typename Name, typename Integral, std::uint8_t Base, overflow_mode Over >
   struct analyze_traits< Name, internal::from_chars_rule< Integral, Base, Over > >
      : analyze_any_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
