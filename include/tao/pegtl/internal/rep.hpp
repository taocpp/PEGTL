// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REP_HPP
#define TAO_PEGTL_INTERNAL_REP_HPP

#include <cstddef>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "seq.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::size_t Cnt, typename... Rules >
   struct rep
      : rep< Cnt, seq< Rules... > >
   {};

   template< std::size_t Cnt >
   struct rep< Cnt >
      : success
   {};

   template< typename Rule >
   struct rep< 0, Rule >
      : success
   {};

   template< std::size_t Cnt, typename Rule >
   struct rep< Cnt, Rule >
   {
      using rule_t = rep;
      using subs_t = type_list< Rule >;

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
         auto m = Control< rep >::template guard< A, M, Action, Control >( in, st... );

         for( std::size_t i = 0; i != Cnt; ++i ) {
            if( !Control< Rule >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) ) {
               return false;
            }
         }
         return m( true );
      }
   };

   template< std::size_t Cnt, typename... Rules >
   inline constexpr bool enable_control< rep< Cnt, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
