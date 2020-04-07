// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_CONTROL_HPP
#define TAO_PEGTL_INTERNAL_CONTROL_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "skip_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< template< typename... > class Control, typename... Rules >
   struct control
      : control< Control, seq< Rules... > >
   {
      using rule_t = control;
      using subs_t = rule_list< Rules... >;
   };

   template< template< typename... > class Control, typename Rule >
   struct control< Control, Rule >
   {
      using rule_t = control;
      using subs_t = rule_list< Rule >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         return Control< Rule >::template match< A, M, Action, Control >( in, st... );
      }
   };

   template< template< typename... > class Control, typename... Rules >
   inline constexpr bool skip_control< control< Control, Rules... > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
