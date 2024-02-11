// Copyright (c) 2019-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REMATCH_HPP
#define TAO_PEGTL_INTERNAL_REMATCH_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "rematch_input.hpp"
#include "seq.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Head, typename... Rules >
   struct rematch;

   template< typename Head >
   struct rematch< Head >
      : seq< Head >
   {};

   template< typename Head, typename Rule, typename... Rules >
   struct rematch< Head, Rule, Rules... >
   {
      using rule_t = rematch;
      using subs_t = type_list< Head, Rule, Rules... >;

      template< apply_mode A,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         auto m = in.template make_rewind_guard< rewind_mode::required >();

         if( Control< Head >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) ) {
            rematch_input i2( m, in );
            return m( ( Control< Rule >::template match< A, rewind_mode::optional, Action, Control >( i2, st... ) && ... && ( i2.restart(), Control< Rules >::template match< A, rewind_mode::optional, Action, Control >( i2, st... ) ) ) );
         }
         return false;
      }
   };

   template< typename Head, typename... Rules >
   inline constexpr bool enable_control< rematch< Head, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
