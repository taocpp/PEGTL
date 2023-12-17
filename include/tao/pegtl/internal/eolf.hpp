// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_EOLF_HPP
#define TAO_PEGTL_INTERNAL_EOLF_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "eol.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol >
   struct eolf
   {
      using rule_t = eolf;
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
         return in.empty() || eol< Eol >::template match< A, M, Action, Control >( in, st... );
      }
   };

   template< typename Eol >
   inline constexpr bool enable_control< eolf< Eol > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
