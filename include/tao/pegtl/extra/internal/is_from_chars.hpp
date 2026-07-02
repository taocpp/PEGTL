// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_IS_FROM_CHARS_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_IS_FROM_CHARS_HPP

#include <type_traits>

#include "../../config.hpp"

#include "charconv.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename >
   struct is_from_chars
      : std::false_type
   {};

   template< typename Integral, std::uint8_t Base, overflow_mode Over >
   struct is_from_chars< Integral, from_chars_rule< void, Base, Over > >
      : std::true_type
   {};

   template< typename Integral, std::uint8_t Base, overflow_mode Over >
   struct is_from_chars< Integral, from_chars_rule< Integral, Base, Over > >
      : std::true_type
   {};

   template< typename Object, typename Rule >
   inline constexpr bool is_from_chars_v = is_from_chars< Object, Rule >::value;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
