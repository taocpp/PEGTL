// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_PARSE_HPP
#define TAO_PEGTL_PARSE_HPP

#include <cassert>
#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse_error.hpp"
#include "position.hpp"
#include "rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             apply_mode A = apply_mode::action,
             rewind_mode M = rewind_mode::required,
             typename ParseInput,
             typename... States >
   auto parse( ParseInput&& in, States&&... st )
   {
      return Control< Rule >::template match< A, M, Action, Control >( in, st... );
   }

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             apply_mode A = apply_mode::action,
             rewind_mode M = rewind_mode::required,
             typename ParseInput,
             typename... States >
   auto parse_nested( position op, ParseInput&& in, States&&... st )
   {
      try {
         return parse< Rule, Action, Control, A, M >( in, st... );
      }
      catch( parse_error& e ) {
         e.add_position( std::move( op ) );
         throw;
      }
   }

   // NOTE: The oi.position() in the version below can be expensive for lazy
   // inputs, which is why the version below does not simply call the version
   // above with said oi.position() as first parameter.

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             apply_mode A = apply_mode::action,
             rewind_mode M = rewind_mode::required,
             typename OuterInput,
             typename ParseInput,
             typename... States >
   auto parse_nested( const OuterInput& oi, ParseInput&& in, States&&... st )
   {
      try {
         return parse< Rule, Action, Control, A, M >( in, st... );
      }
      catch( parse_error& e ) {
         e.add_position( oi.position() );
         throw;
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
