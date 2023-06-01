// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_STRICT_HPP
#define TAO_PEGTL_INTERNAL_STRICT_HPP

#include "enable_control.hpp"
#include "seq.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename... Rules >
   struct strict
   {
      using rule_t = strict;
      using subs_t = type_list< Rule, Rules... >;

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
         auto m = in.template auto_rewind< M >();
         using m_t = decltype( m );

         if( Control< Rule >::template match< A, rewind_mode::required, Action, Control >( in, st... ) ) {
            if( Control< seq< Rules... > >::template match< A, m_t::next_rewind_mode, Action, Control >( in, st... ) ) {
               return m( true );
            }
            return m( false );
         }
         return m( true );
      }
   };

   template< typename... Rules >
   inline constexpr bool enable_control< strict< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
