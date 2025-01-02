// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REMOVE_LAST_STATES_HPP
#define TAO_PEGTL_CONTROL_REMOVE_LAST_STATES_HPP

#include <cstddef>
#include <tuple>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "../internal/type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // The last N states are removed for most of the control functions forwarded to Base.
   // The call to match() is unchanged to keep an even playing field when calling other rules.

   template< typename Base, std::size_t N >
   struct remove_last_states
      : Base
   {
      template< typename ParseInput, typename Tuple, std::size_t... Is >
      static void start_impl( const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::start( in, std::get< Is >( t )... ) ) )
      {
         Base::start( in, std::get< Is >( t )... );
      }

      template< typename ParseInput, typename... States >
      static void start( const ParseInput& in, States&&... st ) noexcept( noexcept( start_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) ) )
      {
         start_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< typename ParseInput, typename Tuple, std::size_t... Is >
      static void success_impl( const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::success( in, std::get< Is >( t )... ) ) )
      {
         Base::success( in, std::get< Is >( t )... );
      }

      template< typename ParseInput, typename... States >
      static void success( const ParseInput& in, States&&... st ) noexcept( noexcept( success_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) ) )
      {
         success_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< typename ParseInput, typename Tuple, std::size_t... Is >
      static void failure_impl( const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::failure( in, std::get< Is >( t )... ) ) )
      {
         Base::failure( in, std::get< Is >( t )... );
      }

      template< typename ParseInput, typename... States >
      static void failure( const ParseInput& in, States&&... st ) noexcept( noexcept( failure_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) ) )
      {
         failure_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< apply_mode A, rewind_mode M, template< typename... > class Action, template< typename... > class Control, typename ParseInput, typename Tuple, std::size_t... Is >
      static auto guard_impl( ParseInput&& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ )
      {
         return Base::template guard< A, M, Action, Control >( in, std::get< Is >( t )... );
      }

      template< apply_mode A, rewind_mode M, template< typename... > class Action, template< typename... > class Control, typename ParseInput, typename... States >
      static auto guard( ParseInput&& in, States&&... st )
      {
         return guard_impl< A, M, Action, Control >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< typename ParseInput, typename Tuple, std::size_t... Is >
      [[noreturn]] static void raise_impl( const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ )
      {
         Base::raise( in, std::get< Is >( t )... );
      }

      template< typename ParseInput, typename... States >
      [[noreturn]] static void raise( const ParseInput& in, States&&... st )
      {
         raise_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< typename Ambient, typename ParseInput, typename Tuple, std::size_t... Is >
      [[noreturn]] static void raise_nested_impl( const Ambient& am, const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ )
      {
         Base::raise_nested( am, in, std::get< Is >( t )... );
      }

      template< typename Ambient, typename ParseInput, typename... States >
      [[noreturn]] static void raise_nested( const Ambient& am, const ParseInput& in, States&&... st )
      {
         raise_nested_impl( am, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< typename ParseInput, typename Tuple, std::size_t... Is >
      static auto unwind_impl( const ParseInput& am, const Tuple& t, std::index_sequence< Is... > /*unused*/ )
         -> std::enable_if_t< internal::has_unwind< Base, void, const ParseInput&, std::tuple_element_t< Is, Tuple >... > >
      {
         Base::unwind( am, std::get< Is >( t )... );
      }

      template< typename ParseInput, typename... States >
      static auto unwind( const ParseInput& in, States&&... st )
         -> decltype( unwind_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) )
      {
         unwind_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< template< typename... > class Action, typename RewindPosition, typename ParseInput, typename Tuple, std::size_t... Is >
      static auto apply_impl( const RewindPosition& begin, const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::template apply< Action >( begin, in, std::get< Is >( t )... ) ) )
         -> decltype( Base::template apply< Action >( begin, in, std::get< Is >( t )... ) )
      {
         return Base::template apply< Action >( begin, in, std::get< Is >( t )... );
      }

      template< template< typename... > class Action, typename RewindPosition, typename ParseInput, typename... States >
      static auto apply( const RewindPosition& begin, const ParseInput& in, States&&... st ) noexcept( noexcept( apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) ) )
         -> decltype( apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) )
      {
         return apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }

      template< template< typename... > class Action, typename ParseInput, typename Tuple, std::size_t... Is >
      static auto apply0_impl( const ParseInput& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::template apply0< Action >( in, std::get< Is >( t )... ) ) )
         -> decltype( Base::template apply0< Action >( in, std::get< Is >( t )... ) )
      {
         return Base::template apply0< Action >( in, std::get< Is >( t )... );
      }

      template< template< typename... > class Action, typename ParseInput, typename... States >
      static auto apply0( const ParseInput& in, States&&... st ) noexcept( noexcept( apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) ) )
         -> decltype( apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() ) )
      {
         return apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - N >() );
      }
   };

   template< typename Base >
   using remove_last_state = remove_last_states< Base, 1 >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
