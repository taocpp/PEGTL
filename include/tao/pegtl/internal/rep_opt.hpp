// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REP_OPT_HPP
#define TAO_PEGTL_INTERNAL_REP_OPT_HPP

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
   template< std::size_t Max, typename... Rules >
   struct rep_opt
      : rep_opt< Max, seq< Rules... > >
   {};

   template< std::size_t Max >
   struct rep_opt< Max >
      : success
   {};

   template< typename... Rules >
   struct rep_opt< 0, Rules... >
      : success
   {};

   template< std::size_t Max, typename Rule >
   struct rep_opt< Max, Rule >
   {
      using rule_t = rep_opt;
      using subs_t = type_list< Rule >;

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
         for( std::size_t i = 0; ( i != Max ) && Control< Rule >::template match< A, rewind_mode::required, Action, Control >( in, st... ); ++i ) {
         }
         return true;
      }
   };

   template< std::size_t Max, typename... Rules >
   inline constexpr bool enable_control< rep_opt< Max, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
