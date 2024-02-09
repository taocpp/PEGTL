// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_HAS_EOL_LAZY_PEEK_HPP
#define TAO_PEGTL_INTERNAL_HAS_EOL_LAZY_PEEK_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   using has_eol_lazy_peek_impl = typename T::eol_lazy_peek;

   template< typename, typename = void >
   inline constexpr bool has_eol_lazy_peek = false;

   template< typename T >
   inline constexpr bool has_eol_lazy_peek< T, std::void_t< has_eol_lazy_peek_impl< T > > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
