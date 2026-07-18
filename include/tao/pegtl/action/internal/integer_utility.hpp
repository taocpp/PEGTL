// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_INTEGER_UTILITY_HPP
#define TAO_PEGTL_ACTION_INTERNAL_INTEGER_UTILITY_HPP

#include "../../config.hpp"
#include "../../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   inline constexpr bool is_proper_signed_v = type_list_contains_v< T, type_list< signed char, signed short, signed int, signed long, signed long long > >;

   template< typename T >
   inline constexpr bool is_proper_unsigned_v = type_list_contains_v< T, type_list< unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long > >;

   template< typename T >
   inline constexpr bool is_proper_integer_v = is_proper_signed_v< T > || is_proper_unsigned_v< T >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
