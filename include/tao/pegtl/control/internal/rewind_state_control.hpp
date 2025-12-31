// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_INTERNAL_REWIND_STATE_CONTROL_HPP
#define TAO_PEGTL_CONTROL_INTERNAL_REWIND_STATE_CONTROL_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../rewind_mode.hpp"

#include "state_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename, typename, typename >
   class rewind_state_control_guard;

   template< typename Rule >
   struct rewind_state_control_wrapper
   {};

   template< typename RewindGuard, typename Rule, typename State, typename ParseInput, typename... States >
   class rewind_state_control_guard< RewindGuard, rewind_state_control_wrapper< Rule >, std::tuple< State, ParseInput, States... > >
      : public RewindGuard
   {
   public:
      rewind_state_control_guard( RewindGuard&& rg, rewind_state_control_wrapper< Rule > /*unused*/, const std::tuple< State, ParseInput, States... >& st )
         : RewindGuard( std::move( rg ) ),
           m_tuple( st )
      {
         std::apply( []( State& s, auto&... as ) { s.template prep_rewind< Rule >( as... ); }, m_tuple );
      }

      rewind_state_control_guard( rewind_state_control_guard&& ) = delete;
      rewind_state_control_guard( const rewind_state_control_guard& ) = delete;

      void operator=( rewind_state_control_guard&& ) = delete;
      void operator=( const rewind_state_control_guard& ) = delete;

      ~rewind_state_control_guard()
      {
         if( this->active() ) {
            std::apply( []( State& s, auto&... as ) { s.template will_rewind< Rule >( as... ); }, m_tuple );
         }
         else {
            std::apply( []( State& s, auto&... as ) { s.template wont_rewind< Rule >( as... ); }, m_tuple );
         }
      }

   private:
      const std::tuple< State, ParseInput, States... > m_tuple;
   };

   template< typename RewindGuard, typename Rule, typename Tuple >
   rewind_state_control_guard( RewindGuard&&, rewind_state_control_wrapper< Rule >, const Tuple& ) -> rewind_state_control_guard< RewindGuard, rewind_state_control_wrapper< Rule >, Tuple >;

   template< template< typename... > class Control, typename Rule >
   struct rewind_state_control
      : state_control< Control, Rule >
   {
      using base = state_control< Control, Rule >;

      template< apply_mode A, rewind_mode M, template< typename... > class Action, template< typename... > class, typename ParseInput, typename State, typename... States >
      static auto guard( ParseInput& in, State&& state, States&&... st )
      {
         if constexpr( M == rewind_mode::optional ) {
            return base::template guard< A, M, Action, Control >( in, state, st... );
         }
         else {
            return internal::rewind_state_control_guard( base::template guard< A, M, Action, Control >( in, state, st... ), internal::rewind_state_control_wrapper< Rule >(), std::tie( state, in, st... ) );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
