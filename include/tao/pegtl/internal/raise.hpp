// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RAISE_HPP
#define TAO_PEGTL_INTERNAL_RAISE_HPP

#if !defined( __cpp_exceptions )
#error "Exception support required for tao/pegtl/internal/raise.hpp"
#else

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   struct raise
   {
      using rule_t = raise;
      using subs_t = empty_list;

      template< apply_mode,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename... States >
      [[noreturn]] static bool match( ParseInput& in, States&&... st )
      {
         Control< T >::raise( static_cast< const ParseInput& >( in ), st... );
      }
   };

   template< typename T >
   inline constexpr bool enable_control< raise< T > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
