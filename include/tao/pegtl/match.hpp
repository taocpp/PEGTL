// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_MATCH_HPP
#define TAO_PEGTL_MATCH_HPP

#include <type_traits>
#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "rewind_mode.hpp"

#include "internal/dusel_mode.hpp"
#include "internal/duseltronik.hpp"
#include "internal/has_apply.hpp"
#include "internal/has_apply0.hpp"
#include "internal/has_match.hpp"
#include "internal/skip_control.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] bool match( Input& in, States&&... st )
      {
         constexpr bool use_control = !internal::skip_control< Rule >;
         constexpr bool use_apply = use_control && ( A == apply_mode::action );

         constexpr bool has_apply_void = use_apply && internal::has_apply< Control< Rule >, void, Action, const typename Input::iterator_t&, const Input&, States... >::value;
         constexpr bool has_apply_bool = use_apply && internal::has_apply< Control< Rule >, bool, Action, const typename Input::iterator_t&, const Input&, States... >::value;

         constexpr bool has_apply0_void = use_apply && internal::has_apply0< Control< Rule >, void, Action, const Input&, States... >::value;
         constexpr bool has_apply0_bool = use_apply && internal::has_apply0< Control< Rule >, bool, Action, const Input&, States... >::value;

         static_assert( !( ( has_apply_void || has_apply_bool ) && ( has_apply0_void || has_apply0_bool ) ), "both apply() and apply0() defined" );

         constexpr auto mode = static_cast< dusel_mode >( use_control + has_apply_void + 2 * has_apply_bool + 3 * has_apply0_void + 4 * has_apply0_bool );
         return internal::duseltronik< Rule, A, M, Action, Control, mode >::match( in, st... );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
