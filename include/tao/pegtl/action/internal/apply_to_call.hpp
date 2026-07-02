// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_CALL_HPP
#define TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_CALL_HPP

#include <type_traits>

#include "../../config.hpp"

#include "auto_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T, T, typename A, typename = void >
   struct apply_to_wrap;

   template< typename C, typename T, T C::*P >
   struct apply_to_wrap< T C::*, P, auto_list<>, std::enable_if_t< std::is_member_object_pointer_v< T C::* > > >
   {
      template< typename U >
      static decltype( auto ) call( C& target, U&& u )
      {
         return ( target.*P ) = std::forward< U >( u );
      }
   };

   template< typename C, typename T, T C::*P, auto A >
   struct apply_to_wrap< T C::*, P, auto_list< A >, std::enable_if_t< std::is_member_object_pointer_v< T C::* > > >
   {
      static decltype( auto ) call( C& target )
      {
         return ( target.*P ) = A;
      }
   };

#if defined( __GNUC__ ) || defined( __clang__ )

   template< bool N, typename R, typename C, typename... Ts, R ( *P )( C&, Ts... ) noexcept( N ), auto... As >
   struct apply_to_wrap< R ( * )( C&, Ts... ) noexcept( N ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return P( target, std::forward< Us >( us )..., As... );
      }
   };

   template< bool N, typename R, typename C, typename... Ts, R ( C::*P )( Ts... ) noexcept( N ), auto... As >
   struct apply_to_wrap< R ( C::* )( Ts... ) noexcept( N ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return ( target.*P )( std::forward< Us >( us )..., As... );
      }
   };

#else  // MSVC can't deduce the N in template< bool N ..... noexcept( N ) >

   template< typename R, typename C, typename... Ts, R ( *P )( C&, Ts... ) noexcept( true ), auto... As >
   struct apply_to_wrap< R ( * )( C&, Ts... ) noexcept( true ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return P( target, std::forward< Us >( us )..., As... );
      }
   };

   template< typename R, typename C, typename... Ts, R ( *P )( C&, Ts... ) noexcept( false ), auto... As >
   struct apply_to_wrap< R ( * )( C&, Ts... ) noexcept( false ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return P( target, std::forward< Us >( us )..., As... );
      }
   };

   template< typename R, typename C, typename... Ts, R ( C::*P )( Ts... ) noexcept( true ), auto... As >
   struct apply_to_wrap< R ( C::* )( Ts... ) noexcept( true ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return ( target.*P )( std::forward< Us >( us )..., As... );
      }
   };

   template< typename R, typename C, typename... Ts, R ( C::*P )( Ts... ) noexcept( false ), auto... As >
   struct apply_to_wrap< R ( C::* )( Ts... ) noexcept( false ), P, auto_list< As... > >
   {
      template< typename... Us >
      static decltype( auto ) call( C& target, Us&&... us )
      {
         return ( target.*P )( std::forward< Us >( us )..., As... );
      }
   };

#endif

   template< auto S, auto... As >
   struct apply_to_call
      : apply_to_wrap< decltype( S ), S, auto_list< As... > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
