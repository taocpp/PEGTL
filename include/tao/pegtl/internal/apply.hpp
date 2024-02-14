// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_APPLY_HPP
#define TAO_PEGTL_INTERNAL_APPLY_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "action_input.hpp"
#include "apply_impl.hpp"
#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Actions >
   struct apply
   {
      using rule_t = apply;
      using subs_t = empty_list;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( [[maybe_unused]] ParseInput& in, [[maybe_unused]] States&&... st )
      {
         if constexpr( ( A == apply_mode::action ) && ( sizeof...( Actions ) > 0 ) ) {
            const action_input< ParseInput > i2( in.rewind_position(), in );  // No data -- range is from begin to begin.
            return ( apply_impl< Actions >::apply( i2, st... ) && ... );
         }
         else {
#if defined( _MSC_VER )
            ( (void)st, ... );
#endif
            return true;
         }
      }
   };

   template< typename... Actions >
   inline constexpr bool enable_control< apply< Actions... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
