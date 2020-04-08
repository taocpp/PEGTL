// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ACTION_HPP
#define TAO_PEGTL_INTERNAL_ACTION_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "skip_control.hpp"
#include "success.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< typename... > class Action, typename... Rules >
   struct action
      : action< Action, seq< Rules... > >
   {};

   template< template< typename... > class Action >
   struct action< Action >
      : success
   {};

   template< template< typename... > class Action, typename Rule >
   struct action< Action, Rule >
   {
      using rule_t = action;
      using subs_t = rule_list< Rule >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         return Control< Rule >::template match< A, M, Action, Control >( in, st... );
      }
   };

   template< template< typename... > class Action, typename... Rules >
   inline constexpr bool skip_control< action< Action, Rules... > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
