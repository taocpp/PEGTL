// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_VALUE_LIST_CONCAT_HPP
#define TAO_PEGTL_INTERNAL_VALUE_LIST_CONCAT_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... >
   struct value_list_concat;

   template< typename S >
   struct value_list_concat< S >
   {
      using type = S;
   };

   template< typename C, template< C... > class S, C... Cs, C... Ds, typename... Ts >
   struct value_list_concat< S< Cs... >, S< Ds... >, Ts... >
      : value_list_concat< S< Cs..., Ds... >, Ts... >
   {};

   template< typename... Ts >
   using value_list_concat_t = typename value_list_concat< Ts... >::type;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
