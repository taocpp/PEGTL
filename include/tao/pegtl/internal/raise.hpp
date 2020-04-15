// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_RAISE_HPP
#define TAO_PEGTL_INTERNAL_RAISE_HPP

#include <stdexcept>

#include "../config.hpp"

#include "enable_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

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
