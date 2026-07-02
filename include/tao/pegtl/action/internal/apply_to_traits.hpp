// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_TRAITS_HPP
#define TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_TRAITS_HPP

#include <type_traits>

#include "../../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename = void >
   struct apply_to_traits;

   template< typename C, typename T >
   struct apply_to_traits< T C::*, std::enable_if_t< std::is_member_object_pointer_v< T C::* > > >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

#if defined( __GNUC__ ) || defined( __clang__ )

   template< bool N, typename R, typename C >
   struct apply_to_traits< R ( * )( C& ) noexcept( N ) >
   {
      using target_t = C;
   };

   template< bool N, typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( * )( C&, T, Ts... ) noexcept( N ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

   template< bool N, typename R, typename C >
   struct apply_to_traits< R ( C::* )() noexcept( N ) >
   {
      using target_t = C;
   };

   template< bool N, typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( C::* )( T, Ts... ) noexcept( N ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

#else  // MSVC can't deduce the N in template< bool N ..... noexcept( N ) >

   template< typename R, typename C >
   struct apply_to_traits< R ( * )( C& ) noexcept( true ) >
   {
      using target_t = C;
   };

   template< typename R, typename C >
   struct apply_to_traits< R ( * )( C& ) noexcept( false ) >
   {
      using target_t = C;
   };

   template< typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( * )( C&, T, Ts... ) noexcept( true ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

   template< typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( * )( C&, T, Ts... ) noexcept( false ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

   template< typename R, typename C >
   struct apply_to_traits< R ( C::* )() noexcept( true ) >
   {
      using target_t = C;
   };

   template< typename R, typename C >
   struct apply_to_traits< R ( C::* )() noexcept( false ) >
   {
      using target_t = C;
   };

   template< typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( C::* )( T, Ts... ) noexcept( true ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

   template< typename R, typename C, typename T, typename... Ts >
   struct apply_to_traits< R ( C::* )( T, Ts... ) noexcept( false ) >
   {
      using target_t = C;
      using object_t = std::decay_t< T >;
   };

#endif

   template< auto S >
   using apply_to_target_t = typename apply_to_traits< decltype( S ) >::target_t;

   template< auto S >
   using apply_to_object_t = typename apply_to_traits< decltype( S ) >::object_t;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
