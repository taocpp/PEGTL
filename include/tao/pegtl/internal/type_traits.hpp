// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TYPE_TRAITS_HPP
#define TAO_PEGTL_INTERNAL_TYPE_TRAITS_HPP

#include <type_traits>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   inline constexpr bool is_simple_type_v = std::is_integral_v< T > || std::is_enum_v< T >;

   template< typename, typename, template< typename... > class, typename... >
   inline constexpr bool has_apply = false;

   template< typename C, template< typename... > class Action, typename... S >
   inline constexpr bool has_apply< C, decltype( C::template apply< Action >( std::declval< S >()... ) ), Action, S... > = true;

   template< typename, typename, template< typename... > class, typename... >
   inline constexpr bool has_apply0 = false;

   template< typename C, template< typename... > class Action, typename... S >
   inline constexpr bool has_apply0< C, decltype( C::template apply0< Action >( std::declval< S >()... ) ), Action, S... > = true;

   template< typename T >
   using has_eol_char_rule_impl = typename T::eol_char_rule;

   template< typename, typename = void >
   inline constexpr bool has_eol_char_rule = false;

   template< typename T >
   inline constexpr bool has_eol_char_rule< T, std::void_t< has_eol_char_rule_impl< T > > > = true;

   template< typename T >
   using has_eol_lazy_peek_impl = typename T::eol_lazy_peek;

   template< typename, typename = void >
   inline constexpr bool has_eol_lazy_peek = false;

   template< typename T >
   inline constexpr bool has_eol_lazy_peek< T, std::void_t< has_eol_lazy_peek_impl< T > > > = true;

   template< typename T >
   using has_eol_rule_impl = typename T::eol_rule;

   template< typename, typename = void >
   inline constexpr bool has_eol_rule = false;

   template< typename T >
   inline constexpr bool has_eol_rule< T, std::void_t< has_eol_rule_impl< T > > > = true;

   template< typename, typename = void >
   inline constexpr bool has_current_position = false;

   template< typename C >
   inline constexpr bool has_current_position< C, decltype( (void)std::declval< C >().current_position(), void() ) > = true;

   // The (void) is to shut up a warning from GCC 9 and 10 about the return value of the nodiscard-function current_position() being ignored.

   template< typename Rule, typename = const char* >
   inline constexpr bool has_error_message = false;

   template< typename Rule >
   inline constexpr bool has_error_message< Rule, std::decay_t< decltype( Rule::error_message ) > > = true;

   template< typename C, typename... S >
   inline constexpr bool has_operator = false;

   template< typename C, typename... S >
   inline constexpr bool has_operator< C, decltype( std::declval< C >().operator()( std::declval< S >()... ) ), S... > = true;

   template< typename Input, typename = void >
   inline constexpr bool has_restart = false;

   template< typename Input >
   inline constexpr bool has_restart< Input, decltype( (void)std::declval< Input >().restart() ) > = true;

   template< typename Input, typename = void >
   inline constexpr bool has_start = false;

   template< typename Input >
   inline constexpr bool has_start< Input, decltype( (void)std::declval< Input >().start() ) > = true;

   template< typename, typename... >
   inline constexpr bool has_unwind = false;

   template< typename C, typename... S >
   inline constexpr bool has_unwind< C, decltype( C::unwind( std::declval< S >()... ) ), S... > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif

