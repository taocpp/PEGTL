// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_NESTED_HPP
#define TAO_PEGTL_INTERNAL_NESTED_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../parse.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "input_with_fakes.hpp"
#include "input_with_funcs.hpp"
#include "input_with_start.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Peek, typename Rule >
   struct nested
   {
      using peek_t = Peek;
      using data_t = typename Peek::data_t;

      using rule_t = nested;
      using subs_t = empty_list;

      using value_t = typename data_t::value_type;
      using input_t = input_with_fakes< input_with_funcs< input_with_start< view_input< value_t > > > >;

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
         if( const auto p = Peek::peek( in ) ) {
            input_t i2( p.data().data(), p.data().size() );
            if( parse_nested< Rule, Action, Control, A, rewind_mode::optional >( in, i2, st... ) ) {
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
