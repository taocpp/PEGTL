// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_CHECK_COUNT_HPP
#define TAO_PEGTL_CONTRIB_CHECK_COUNT_HPP

#include <string>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#if defined( __cpp_exceptions )
#include "../demangle.hpp"
#include "../parse_error.hpp"
#else
#include <cstdio>
#include <exception>
#endif

namespace TAO_PEGTL_NAMESPACE
{
   template< std::size_t Maximum >
   struct check_count
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      static bool match( ParseInput& in, States&&... st )
      {
         const auto* start = in.current();
         if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... ) ) {
            if( std::size_t( in.current() - start ) > Maximum ) {
#if defined( __cpp_exceptions )
               const std::string d( demangle< Rule >() );
               throw_parse_error( "consumption limit exceeded for " + d, in );
#else
               std::fputs( "parse error: consumption limit exceeded\n", stderr );
               std::terminate();
#endif
            }
            return true;
         }
         return false;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
