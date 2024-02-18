// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_TRACE_HPP
#define TAO_PEGTL_CONTRIB_TRACE_HPP

#include <iostream>
#include <tuple>
#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "../control/state_control.hpp"

#include "trace_state.hpp"
#include "trace_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename TraceTraits >
   struct trace_action
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         using trace_state_t = trace_state< TraceTraits, std::decay_t< ParseInput > >;

         if constexpr( sizeof...( st ) == 0 ) {
            trace_state_t tr( std::cerr, in );
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, state_control< Control >::template type >( in, st..., tr );
         }
         else if constexpr( !std::is_same_v< std::tuple_element_t< sizeof...( st ) - 1, std::tuple< States... > >, trace_state_t& > ) {
            trace_state_t tr( std::cerr, in );
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, state_control< Control >::template type >( in, st..., tr );
         }
         else {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      }
   };

   using trace_standard = trace_action< standard_trace_traits >;
   using trace_complete = trace_action< complete_trace_traits >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
