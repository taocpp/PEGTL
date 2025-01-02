// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTROL_REWIND_CONTROL_HPP
#define TAO_PEGTL_CONTROL_REWIND_CONTROL_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "../internal/type_traits.hpp"

#include "shuffle_states.hpp"
#include "state_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename, typename, typename >
      class rewind_control_guard;

      template< typename RewindGuard >
      struct rewind_control_wrapper
      {};

      template< typename RewindGuard, typename RewindControl, typename ParseInput, typename... States >
      class rewind_control_guard< RewindGuard, rewind_control_wrapper< RewindControl >, std::tuple< ParseInput, States... > >
         : public RewindGuard
      {
      public:
         rewind_control_guard( RewindGuard&& rg, rewind_control_wrapper< RewindControl > /*unused*/, const std::tuple< ParseInput, States... >& st )
            : RewindGuard( std::move( rg ) ),
              m_tuple( st )
         {
            std::apply( []( auto&... as ) { RewindControl::prep_rewind( as... ); }, m_tuple );
         }

         rewind_control_guard( rewind_control_guard&& ) = delete;
         rewind_control_guard( const rewind_control_guard& ) = delete;

         void operator=( rewind_control_guard&& ) = delete;
         void operator=( const rewind_control_guard& ) = delete;

         ~rewind_control_guard()
         {
            if( this->active() ) {
               std::apply( []( auto&... as ) { RewindControl::will_rewind( as... ); }, m_tuple );
            }
            else {
               std::apply( []( auto&... as ) { RewindControl::wont_rewind( as... ); }, m_tuple );
            }
         }

      private:
         const std::tuple< ParseInput, States... > m_tuple;
      };

      template< typename RewindGuard, typename RewindControl, typename Tuple >
      rewind_control_guard( RewindGuard&&, rewind_control_wrapper< RewindControl >, const Tuple& ) -> rewind_control_guard< RewindGuard, rewind_control_wrapper< RewindControl >, Tuple >;

   }  // namespace internal

   template< template< typename... > class Control >
   struct rewind_control
   {
      template< typename Rule >
      struct type
         : Control< Rule >
      {
         using base = Control< Rule >;

         template< apply_mode A, rewind_mode M, template< typename... > class Action, template< typename... > class RewindControl, typename ParseInput, typename... States >
         static auto guard( ParseInput& in, States&&... st )
         {
            if constexpr( M == rewind_mode::optional ) {
               return base::template guard< A, M, Action, Control >( in, st... );
            }
            else {
               return internal::rewind_control_guard( base::template guard< A, M, Action, Control >( in, st... ), internal::rewind_control_wrapper< RewindControl< Rule > >(), std::tie( in, st... ) );
            }
         }
      };
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
