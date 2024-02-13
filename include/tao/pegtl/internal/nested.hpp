// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NESTED_HPP
#define TAO_PEGTL_INTERNAL_NESTED_HPP

#include <type_traits>

#include "../config.hpp"
#include "../parse.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "input_with_fakes.hpp"
#include "input_with_funcs.hpp"
#include "input_with_start.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Peek >
   struct nested
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = nested;
      using subs_t = empty_list;

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
         const auto p = Peek::peek( in );
         if( const auto* d = p.pointer() ) {
            using parse_t = std::decay_t< decltype( *( d->data() ) ) >;
            using input_t = input_with_fakes< input_with_funcs< input_with_start< view_input< parse_t > > > >;
            input_t i2( d->data(), d->size() );
            if( parse_nested< Rule, Action, Control, A, M >( in, i2, st... ) ) {
               in.template consume< nested >( 1 );
               return true;
            }
         }
         return false;
      }
   };

   template< typename Rule, typename Peek >
   inline constexpr bool enable_control< nested< Rule, Peek > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
