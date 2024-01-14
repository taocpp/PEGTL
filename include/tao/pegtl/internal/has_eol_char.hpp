// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_HAS_EOL_CHAR_HPP
#define TAO_PEGTL_INTERNAL_HAS_EOL_CHAR_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol >
   using has_eol_char_impl = std::enable_if_t< bool( Eol::eol_char ) >;

   template< typename, typename = void >
   inline constexpr bool has_eol_char = false;

   template< typename Eol >
   inline constexpr bool has_eol_char< Eol, std::void_t< has_eol_char_impl< Eol > > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
