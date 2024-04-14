// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UNTIL_HPP
#define TAO_PEGTL_INTERNAL_UNTIL_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "eof.hpp"
#include "not_at.hpp"
#include "seq.hpp"
#include "star.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Cond, typename... Rules >
   struct until
      : until< Cond, seq< Rules... > >
   {};

   template< typename Cond >
   struct until< Cond >
   {
      using rule_t = until;
      using subs_t = type_list< Cond >;

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
         auto m = Control< until >::template guard< A, M, Action, Control >( in, st... );

         while( !Control< Cond >::template match< A, rewind_mode::required, Action, Control >( in, st... ) ) {
            if( in.empty() ) {
               return false;
            }
            in.template consume< until >( 1 );
         }
         return m( true );
      }
   };

   template< typename Cond, typename Rule >
   struct until< Cond, Rule >
   {
      using rule_t = until;
      using subs_t = type_list< Cond, Rule >;

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
         auto m = Control< until >::template guard< A, M, Action, Control >( in, st... );

         while( !Control< Cond >::template match< A, rewind_mode::required, Action, Control >( in, st... ) ) {
            if( !Control< Rule >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) ) {
               return false;
            }
         }
         return m( true );
      }
   };

   template< typename Cond, typename... Rules >
   inline constexpr bool enable_control< until< Cond, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
