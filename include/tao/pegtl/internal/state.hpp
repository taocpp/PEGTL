// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_STATE_HPP
#define TAO_PEGTL_INTERNAL_STATE_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "seq.hpp"
#include "success.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename State, typename... Rules >
   struct state
      : state< State, seq< Rules... > >
   {};

   template< typename State >
   struct state< State >
      : success
   {};

   template< typename State, typename Rule >
   struct state< State, Rule >
   {
      using rule_t = state;
      using subs_t = type_list< Rule >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         State s( static_cast< const ParseInput& >( in ), st... );
         if( Control< Rule >::template match< A, M, Action, Control >( in, s ) ) {
            s.success( static_cast< const ParseInput& >( in ), st... );
            return true;
         }
         return false;
      }
   };

   template< typename State, typename... Rules >
   inline constexpr bool enable_control< state< State, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
