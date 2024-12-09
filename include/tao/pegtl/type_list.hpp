// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TYPE_LIST_HPP
#define TAO_PEGTL_TYPE_LIST_HPP

#include <cstddef>
#include <type_traits>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Ts >
   struct type_list
   {
      static constexpr std::size_t size = sizeof...( Ts );
   };

   using empty_list = type_list<>;

   template< typename... >
   struct type_list_concat;

   template<>
   struct type_list_concat<>
   {
      using type = empty_list;
   };

   template< typename... Ts >
   struct type_list_concat< type_list< Ts... > >
   {
      using type = type_list< Ts... >;
   };

   template< typename... T0s, typename... T1s, typename... Ts >
   struct type_list_concat< type_list< T0s... >, type_list< T1s... >, Ts... >
      : type_list_concat< type_list< T0s..., T1s... >, Ts... >
   {};

   template< typename... Ts >
   using type_list_concat_t = typename type_list_concat< Ts... >::type;

   template< typename Type, typename... Types >
   inline constexpr bool type_list_contains_v = ( std::is_same_v< Type, Types > || ... );

   template< typename Type, typename... Types >
   struct type_list_contains
      : std::bool_constant< type_list_contains_v< Type, Types... > >
   {};

   template< typename Type, typename... Types >
   struct type_list_contains< Type, type_list< Types... > >
      : type_list_contains< Type, Types... >
   {};

   template< typename >
   struct is_type_list
      : std::false_type
   {};

   template< typename... Ts >
   struct is_type_list< type_list< Ts... > >
      : std::true_type
   {};

   template< typename T >
   inline constexpr bool is_type_list_v = is_type_list< T >::value;

}  // namespace TAO_PEGTL_NAMESPACE

#endif  // FCOV_EXCL_FILE
