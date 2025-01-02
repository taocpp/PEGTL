// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REP_MIN_MAX_HPP
#define TAO_PEGTL_INTERNAL_REP_MIN_MAX_HPP

#include <cstddef>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "failure.hpp"
#include "not_at.hpp"
#include "seq.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::size_t Min, std::size_t Max, typename... Rules >
   struct rep_min_max
      : rep_min_max< Min, Max, seq< Rules... > >
   {
      static_assert( Min <= Max );
   };

   template< std::size_t Min, std::size_t Max >
   struct rep_min_max< Min, Max >
      : failure
   {
      static_assert( Min <= Max );
   };

   template< typename Rule >
   struct rep_min_max< 0, 0, Rule >
      : not_at< Rule >
   {};

   template< std::size_t Min, std::size_t Max, typename Rule >
   struct rep_min_max< Min, Max, Rule >
   {
      using rule_t = rep_min_max;
      using subs_t = type_list< Rule >;

      static_assert( Min <= Max );

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
         auto m = Control< rep_min_max >::template guard< A, M, Action, Control >( in, st... );

         for( std::size_t i = 0; i != Min; ++i ) {
            if( !Control< Rule >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) ) {
               return false;
            }
         }
         for( std::size_t i = Min; i != Max; ++i ) {
            if( !Control< Rule >::template match< A, rewind_mode::required, Action, Control >( in, st... ) ) {
               return m( true );
            }
         }
         return m( Control< not_at< Rule > >::template match< A, rewind_mode::optional, Action, Control >( in, st... ) );  // NOTE that not_at<> will always rewind.
      }
   };

   template< std::size_t Min, std::size_t Max, typename... Rules >
   inline constexpr bool enable_control< rep_min_max< Min, Max, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
