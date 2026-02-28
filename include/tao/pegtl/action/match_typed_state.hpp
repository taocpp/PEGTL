// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_MATCH_TYPED_STATE_HPP
#define TAO_PEGTL_ACTION_MATCH_TYPED_STATE_HPP

#include <tuple>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Typed >
   struct match_typed_state
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, std::get< Typed& >( std::tie( st... ) ) );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
