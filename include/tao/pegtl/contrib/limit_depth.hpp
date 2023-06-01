// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_LIMIT_DEPTH_HPP
#define TAO_PEGTL_CONTRIB_LIMIT_DEPTH_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#else
#include <cstdio>
#include <exception>
#endif

namespace TAO_PEGTL_NAMESPACE
{
   template< std::size_t Maximum >
   struct limit_depth
      : maybe_nothing
   {
      static constexpr const char* error_message = "maximum parser rule nesting depth exceeded";

      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( Control< Rule >::enable ) {
            const auto dg( in.make_depth_guard() );
            if( dg.current_depth() > Maximum ) {
#if defined( __cpp_exceptions )
               Control< limit_depth >::raise( in );
#else
               std::fputs( "maximum parser rule nesting depth exceeded\n", stderr );
               std::terminate();
#endif
            }
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
         else {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
