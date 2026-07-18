// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TYPE_LIST_HPP
#define TAO_PEGTL_TYPE_LIST_HPP

#include <cstddef>
#include <type_traits>

#include "config.hpp"

#include "internal/dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Ts >
   struct type_list
   {
      static constexpr std::size_t size = sizeof...( Ts );
   };

   using empty_list = type_list<>;

   template< typename... >
   struct type_list_append;

   template< typename... Ts, typename... As >
   struct type_list_append< type_list< Ts... >, As... >
   {
      using type = type_list< Ts..., As... >;
   };

   template< typename... Ts >
   using type_list_append_t = typename type_list_append< Ts... >::type;

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

   template< typename T, typename... Ts >
   struct type_list_contains
   {
      static constexpr bool value = ( std::is_same_v< T, Ts > || ... );
   };

   template< typename T, typename... Ts >
   struct type_list_contains< T, type_list< Ts... > >
   {
      static constexpr bool value = type_list_contains< T, Ts... >::value;
   };

   template< typename T, typename... Ts >
   struct type_list_contains< T, type_list< type_list< Ts... > > >
   {
      static_assert( internal::dependent_false< T, Ts... > );
   };

   template< typename... Ts, typename... Us >
   struct type_list_contains< type_list< Ts... >, Us... >
   {
      static_assert( internal::dependent_false< Ts..., Us... > );
   };

   template< typename... Ts, typename... Us >
   struct type_list_contains< type_list< Ts... >, type_list< type_list< Us... > > >
   {
      static_assert( internal::dependent_false< Ts..., Us... > );
   };

   template< typename T, typename... Ts >
   inline constexpr bool type_list_contains_v = type_list_contains< T, Ts... >::value;

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

   template< typename... >
   struct types_are_unique;

   template<>
   struct types_are_unique<>
      : std::true_type
   {};

   template< typename... Ts >
   struct types_are_unique< type_list< Ts... > >
      : types_are_unique< Ts... >
   {};

   template< typename T, typename... Ts >
   struct types_are_unique< T, Ts... >
      : std::bool_constant< ( !std::is_same_v< T, Ts > && ... ) && types_are_unique< Ts... >::value >
   {};

   template< typename... Ts >
   inline constexpr bool types_are_unique_v = types_are_unique< Ts... >::value;

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
