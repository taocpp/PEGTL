// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PLUS_HPP
#define TAO_PEGTL_INTERNAL_PLUS_HPP

#include <type_traits>

#include "../config.hpp"

#include "opt.hpp"
#include "seq.hpp"
#include "skip_control.hpp"
#include "star.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   // While plus<> could easily be implemented with
   // seq< Rule, Rules ..., star< Rule, Rules ... > > we
   // provide an explicit implementation to optimise away
   // the otherwise created input mark.

   template< typename Rule, typename... Rules >
   struct plus
      : plus< seq< Rule, Rules... > >
   {
      using rule_t = plus;
      using subs_t = rule_list< Rules... >;
   };

   template< typename Rule >
   struct plus< Rule >
   {
      using rule_t = plus;
      using subs_t = rule_list< Rule >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         return Control< Rule >::template match< A, M, Action, Control >( in, st... ) && Control< star< Rule > >::template match< A, M, Action, Control >( in, st... );
      }
   };

   template< typename Rule, typename... Rules >
   inline constexpr bool skip_control< plus< Rule, Rules... > > = true;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
