// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_TRACE_HPP
#define TAO_PEGTL_CONTRIB_TRACE_HPP

#include <iostream>
#include <type_traits>

#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"

#include "../control/state_control.hpp"

#include "trace_state.hpp"
#include "trace_traits.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename TraceTraits,
             typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool generic_trace( ParseInput&& in, States&&... st )
   {
      trace_state< TraceTraits, std::decay_t< ParseInput > > tr( std::cerr, in );
      return parse< Rule, Action, state_control< Control >::template type >( in, st..., tr );
   }

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool standard_trace( ParseInput&& in, States&&... st )
   {
      return generic_trace< standard_trace_traits, Rule, Action, Control >( in, st... );
   }

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool complete_trace( ParseInput&& in, States&&... st )
   {
      return generic_trace< complete_trace_traits, Rule, Action, Control >( in, st... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
