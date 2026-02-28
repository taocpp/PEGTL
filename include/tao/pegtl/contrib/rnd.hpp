// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_RND_HPP
#define TAO_PEGTL_CONTRIB_RND_HPP

#include <bitset>
#include <cstddef>
#include <utility>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "../debug/analyze_traits.hpp"
#include "../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< bool Optional, typename... Rules >
      struct rnd;

      template< bool Optional >
      struct rnd< Optional >
         : success
      {};

      template< bool Optional, typename... Rules >
      struct rnd
      {
         using rule_t = rnd;
         using subs_t = type_list< Rules... >;

         static constexpr std::size_t rule_count = sizeof...( Rules );

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   std::size_t... Indices,
                   typename ParseInput,
                   typename... States >
         [[nodiscard]] static bool match( std::index_sequence< Indices... > /*unused*/, ParseInput& in, States&&... st )
         {
            std::bitset< rule_count > b;

            auto m = Control< rnd >::template guard< A, M, Action, Control >( in, st... );

            for( std::size_t i = 0; i < rule_count; ++i ) {
               if( !( ( ( b.test( Indices ) == false ) && ( Control< Rules >::template match< A, ( ( Indices == ( sizeof...( Rules ) - 1 ) ) ? rewind_mode::optional : rewind_mode::required ), Action, Control >( in, st... ) ) && ( b.set( Indices ), true ) ) || ... ) ) {
                  return m( Optional );
               }
            }
            return m( true );
         }

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename ParseInput,
                   typename... States >
         [[nodiscard]] static bool match( ParseInput& in, States&&... st )
         {
            if constexpr( sizeof...( Rules ) == 1 ) {
               return Control< Rules... >::template match< A, M, Action, Control >( in, st... );
            }
            else {
               return match< A, M, Action, Control >( std::index_sequence_for< Rules... >(), in, st... );
            }
         }
      };

      template< bool Optional, typename... Rules >
      inline constexpr bool enable_control< rnd< Optional, Rules... > > = false;

   }  // namespace internal

   template< typename... Rules >
   struct rnd
      : internal::rnd< false, Rules... >
   {};

   template< typename... Rules >
   struct rnd_opt
      : internal::rnd< true, Rules... >
   {};

   template< typename Name, bool Optional, typename... Rules >
   struct analyze_traits< Name, internal::rnd< Optional, Rules... > >
      : std::conditional_t< Optional, analyze_opt_traits< Rules... >, analyze_seq_traits< Rules... > >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
