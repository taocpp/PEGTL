// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_REMOVE_FIRST_STATE_HPP
#define TAO_PEGTL_CONTRIB_REMOVE_FIRST_STATE_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"
#include "../type_list.hpp"

#include "../debug/analyze_traits.hpp"
#include "../internal/enable_control.hpp"
#include "../internal/seq.hpp"
#include "../internal/success.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename... Rules >
      struct remove_first_state
         : remove_first_state< internal::seq< Rules... > >
      {};

      template<>
      struct remove_first_state<>
         : internal::success
      {};

      template< typename Rule >
      struct remove_first_state< Rule >
      {
         using rule_t = remove_first_state;
         using subs_t = type_list< Rule >;

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename ParseInput,
                   typename State,
                   typename... States >
         [[nodiscard]] static bool match( ParseInput& in, State&& /*unused*/, States&&... st )
         {
            return Control< Rule >::template match< A, M, Action, Control >( in, st... );
         }
      };

      template< typename... Rules >
      inline constexpr bool enable_control< remove_first_state< Rules... > > = false;

   }  // namespace internal

   template< typename... Rules >
   struct remove_first_state
      : internal::remove_first_state< Rules... >
   {};

   template< typename Name, typename... Rules >
   struct analyze_traits< Name, internal::remove_first_state< Rules... > >
      : analyze_seq_traits< Rules... >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
