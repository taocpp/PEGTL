// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_UNORDERED_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_UNORDERED_HPP

#include <bitset>
#include <cstddef>
#include <utility>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../rewind_mode.hpp"
#include "../../type_list.hpp"

#include "../../internal/enable_control.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< bool Optional, typename... Rules >
   struct unordered;

   template< bool Optional >
   struct unordered< Optional >
      : success
   {};

   template< bool Optional, typename... Rules >
   struct unordered
   {
      using rule_t = unordered;
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

         auto m = Control< unordered >::template guard< A, M, Action, Control >( in, st... );

         for( std::size_t i = 0; i < rule_count; ++i ) {
            if( !( ( ( b.test( Indices ) == false ) && ( Control< Rules >::template match< A, ( ( Indices == ( rule_count - 1 ) ) ? rewind_mode::optional : rewind_mode::required ), Action, Control >( in, st... ) ) && ( b.set( Indices ), true ) ) || ... ) ) {
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
            return Control< Rules... >::template match< A, M, Action, Control >( in, st... ) || Optional;
         }
         else {
            return match< A, M, Action, Control >( std::index_sequence_for< Rules... >(), in, st... );
         }
      }
   };

   template< bool Optional, typename... Rules >
   inline constexpr bool enable_control< unordered< Optional, Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
