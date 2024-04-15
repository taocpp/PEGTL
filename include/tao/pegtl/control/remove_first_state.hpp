// Copyright (c) 2019-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REMOVE_FIRST_STATE_HPP
#define TAO_PEGTL_CONTROL_REMOVE_FIRST_STATE_HPP

#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "../internal/type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // The first state is removed for most of the control functions forwarded to Base.
   // The call to match() is unchanged because it can call other grammar rules that require
   // all states when starting their match to keep an even playing field.

   template< typename Base >
   struct remove_first_state
      : Base
   {
      template< typename ParseInput, typename State, typename... States >
      static void start( const ParseInput& in, State&& /*unused*/, States&&... st ) noexcept( noexcept( Base::start( in, st... ) ) )
      {
         Base::start( in, st... );
      }

      template< typename ParseInput, typename State, typename... States >
      static void success( const ParseInput& in, State&& /*unused*/, States&&... st ) noexcept( noexcept( Base::success( in, st... ) ) )
      {
         Base::success( in, st... );
      }

      template< typename ParseInput, typename State, typename... States >
      static void failure( const ParseInput& in, State&& /*unused*/, States&&... st ) noexcept( noexcept( Base::failure( in, st... ) ) )
      {
         Base::failure( in, st... );
      }

      template< apply_mode A, rewind_mode M, template< typename... > class Action, template< typename... > class Control, typename ParseInput, typename State, typename... States >
      static auto guard( ParseInput&& in, State&& /*unused*/, States&&... st )
      {
         return Base::template guard< A, M, Action, Control >( in, st... );
      }

      template< typename ParseInput, typename State, typename... States >
      [[noreturn]] static void raise( const ParseInput& in, State&& /*unused*/, States&&... st )
      {
         Base::raise( in, st... );
      }

      template< typename Ambient, typename ParseInput, typename State, typename... States >
      [[noreturn]] static void raise_nested( const Ambient& am, const ParseInput& in, State&& /*unused*/, States&&... st )
      {
         Base::raise_nested( am, in, st... );
      }

      template< typename ParseInput, typename State, typename... States >
      static auto unwind( const ParseInput& in, State&& /*unused*/, States&&... st )
         -> std::enable_if_t< internal::has_unwind< Base, void, const ParseInput&, States... > >
      {
         Base::unwind( in, st... );
      }

      template< template< typename... > class Action, typename RewindPosition, typename ParseInput, typename State, typename... States >
      static auto apply( const RewindPosition& begin, const ParseInput& in, State&& /*unused*/, States&&... st ) noexcept( noexcept( Base::template apply< Action >( begin, in, st... ) ) )
         -> decltype( Base::template apply< Action >( begin, in, st... ) )
      {
         return Base::template apply< Action >( begin, in, st... );
      }

      template< template< typename... > class Action, typename ParseInput, typename State, typename... States >
      static auto apply0( const ParseInput& in, State&& /*unused*/, States&&... st ) noexcept( noexcept( Base::template apply0< Action >( in, st... ) ) )
         -> decltype( Base::template apply0< Action >( in, st... ) )
      {
         return Base::template apply0< Action >( in, st... );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
