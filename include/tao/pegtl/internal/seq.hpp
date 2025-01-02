// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SEQ_HPP
#define TAO_PEGTL_INTERNAL_SEQ_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct seq
   {
      using rule_t = seq;
      using subs_t = type_list< Rules... >;

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
         if constexpr( sizeof...( Rules ) == 1 ) {
            return Control< Rules... >::template match< A, M, Action, Control >( in, st... );
         }
         else {
            auto m = Control< seq >::template guard< A, M, Action, Control >( in, st... );
            return m( ( Control< Rules >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) && ... ) );
         }
      }
   };

   template<>
   struct seq<>
      : success
   {};

   template< typename... Rules >
   inline constexpr bool enable_control< seq< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
