// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_PARTIAL_APPLY1_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_PARTIAL_APPLY1_HPP

#include <type_traits>

#include "../../apply_mode.hpp"
#include "../../config.hpp"
#include "../../nothing.hpp"
#include "../../rewind_mode.hpp"
#include "../../type_list.hpp"

#include "../../action/require_apply.hpp"
#include "../../action/require_apply0.hpp"

#include "../../internal/enable_control.hpp"
#include "../../internal/has_apply.hpp"
#include "../../internal/has_apply0.hpp"
#include "../../internal/missing_apply.hpp"
#include "../../internal/missing_apply0.hpp"
#include "../../internal/partial.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule,
             template< typename... > class Action,
             template< typename... > class Control,
             typename ParseInput,
             typename... States >
   [[nodiscard]] bool partial_apply1_apply( [[maybe_unused]] const typename ParseInput::rewind_position_t& begin, [[maybe_unused]] const ParseInput& in, [[maybe_unused]] States&&... st )
   {
      using rewind_position_t = typename ParseInput::rewind_position_t;

      constexpr bool has_apply_void = has_apply< Control< Rule >, void, Action, const rewind_position_t&, const ParseInput&, States... >;
      constexpr bool has_apply_bool = has_apply< Control< Rule >, bool, Action, const rewind_position_t&, const ParseInput&, States... >;
      constexpr bool has_apply = has_apply_void || has_apply_bool;

      constexpr bool has_apply0_void = has_apply0< Control< Rule >, void, Action, const ParseInput&, States... >;
      constexpr bool has_apply0_bool = has_apply0< Control< Rule >, bool, Action, const ParseInput&, States... >;
      constexpr bool has_apply0 = has_apply0_void || has_apply0_bool;

      static_assert( !( has_apply_void && has_apply_bool ), "Both void and bool apply() detected in action!" );
      static_assert( !( has_apply0_void && has_apply0_bool ), "Both void and bool apply0() detected in action!" );
      static_assert( !( has_apply && has_apply0 ), "Both apply() and apply0() detected in action!" );

      constexpr bool is_nothing = std::is_base_of_v< nothing< Rule >, Action< Rule > >;
      constexpr bool is_exact_nothing = std::is_same_v< nothing< Rule >, Action< Rule > >;

      static_assert( !( has_apply && is_nothing ), "Unexpected apply() detected in action!" );
      static_assert( !( has_apply0 && is_nothing ), "Unexpected apply0() detected in action!" );

      if constexpr( !has_apply && std::is_base_of_v< require_apply, Action< Rule > > ) {
         missing_apply< Control< Rule >, Action >( in, st... );
      }
      if constexpr( !has_apply0 && std::is_base_of_v< require_apply0, Action< Rule > > ) {
         missing_apply0< Control< Rule >, Action >( in, st... );
      }
      if constexpr( has_apply_void ) {
         Control< Rule >::template apply< Action >( begin, in, st... );
         return true;
      }
      else if constexpr( has_apply_bool ) {
         return Control< Rule >::template apply< Action >( begin, in, st... );
      }
      else if constexpr( has_apply0_void ) {
         Control< Rule >::template apply0< Action >( in, st... );
         return true;
      }
      else if constexpr( has_apply0_bool ) {
         return Control< Rule >::template apply0< Action >( in, st... );
      }
      else {
         static_assert( is_exact_nothing, "partial_apply1 requires apply() or apply0() for the selected rule!" );
         return true;
      }
   }

   template< typename Last, typename... Rules >
   struct partial_apply1_match;

   template< typename Last >
   struct partial_apply1_match< Last >
   {
      template< template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( const typename ParseInput::rewind_position_t& begin, ParseInput& in, States&&... st )
      {
         return partial_apply1_apply< Last, Action, Control >( begin, static_cast< const ParseInput& >( in ), st... );
      }
   };

   template< typename Last, typename Rule, typename... Rules >
   struct partial_apply1_match< Last, Rule, Rules... >
   {
      template< template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( const typename ParseInput::rewind_position_t& begin, ParseInput& in, States&&... st )
      {
         if( Control< Rule >::template match< apply_mode::disabled, rewind_mode::required, Action, Control >( in, st... ) ) {
            return partial_apply1_match< Rule, Rules... >::template match< Action, Control >( begin, in, st... );
         }
         return partial_apply1_apply< Last, Action, Control >( begin, static_cast< const ParseInput& >( in ), st... );
      }
   };

   template< typename... Rules >
   struct partial_apply1;

   template< typename Rule, typename... Rules >
   struct partial_apply1< Rule, Rules... >
   {
      using rule_t = partial_apply1;
      using subs_t = type_list< Rule, Rules... >;

      static_assert( sizeof...( Rules ) > 0 );

      template< apply_mode A,
                rewind_mode M,
                template< typename... > class Action,
                template< typename... > class Control,
                typename ParseInput,
                typename... States >
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         if constexpr( A == apply_mode::disabled ) {
            if( Control< Rule >::template match< apply_mode::disabled, M, Action, Control >( in, st... ) ) {
               return partial< Rules... >::template match< apply_mode::disabled, rewind_mode::optional, Action, Control >( in, st... );
            }
            return false;
         }
         else {
            auto m = Control< partial_apply1 >::template guard< A, rewind_mode::required, Action, Control >( in, st... );

            if( Control< Rule >::template match< apply_mode::disabled, rewind_mode::optional, Action, Control >( in, st... ) ) {
               return m( partial_apply1_match< Rule, Rules... >::template match< Action, Control >( m.rewind_position(), in, st... ) );
            }
            return m( false );
         }
      }
   };

   template< typename... Rules >
   inline constexpr bool enable_control< partial_apply1< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
