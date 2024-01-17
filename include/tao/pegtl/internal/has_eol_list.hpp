// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_HAS_EOL_LIST_HPP
#define TAO_PEGTL_INTERNAL_HAS_EOL_LIST_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol >
   using has_eol_list_impl = typename Eol::eol_list;

   template< typename, typename = void >
   inline constexpr bool has_eol_list = false;

   template< typename Eol >
   inline constexpr bool has_eol_list< Eol, std::void_t< has_eol_list_impl< Eol > > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
