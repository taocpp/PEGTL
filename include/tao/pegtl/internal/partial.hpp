// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PARTIAL_HPP
#define TAO_PEGTL_INTERNAL_PARTIAL_HPP

#include "enable_control.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct partial
   {
      using rule_t = partial;
      using subs_t = type_list< Rules... >;

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
         (void)( Control< Rules >::template match< A, rewind_mode::required, Action, Control >( in, st... ) && ... );
         return true;
      }
   };

   template< typename... Rules >
   inline constexpr bool enable_control< partial< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
