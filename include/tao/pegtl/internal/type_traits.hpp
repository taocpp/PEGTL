// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TYPE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TYPE_TRAITS_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   constexpr inline bool is_simple_type_v = std::is_integral_v< T > || std::is_enum_v< T >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif

