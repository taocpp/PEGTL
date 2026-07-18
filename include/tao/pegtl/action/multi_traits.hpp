// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_MULTI_TRAITS_HPP
#define TAO_PEGTL_ACTION_MULTI_TRAITS_HPP

#include <tuple>
#include <utility>

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   struct multi_traits
   {
      using fields_t = type_list< T >;
   };

   template< typename T, typename U >
   struct multi_traits< std::pair< T, U > >
   {
      using fields_t = type_list< T, U >;
   };

   template< typename... Ts >
   struct multi_traits< std::tuple< Ts... > >
   {
      using fields_t = type_list< Ts... >;
   };

   // TODO: Document make() for custom types!

}  // namespace TAO_PEGTL_NAMESPACE

#endif
