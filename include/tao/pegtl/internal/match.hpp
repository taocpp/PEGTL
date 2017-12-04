// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_MATCH_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_MATCH_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

#include "dusel_mode.hpp"
#include "duseltronik.hpp"
#include "has_apply.hpp"
#include "has_apply0.hpp"
#include "skip_control.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Rule, bool = skip_control< Rule >::value >
         struct dusel
         {
            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               return duseltronik< Rule, A, M, Action, Control, dusel_mode::NOTHING >::match( in, st... );
            }
         };

         template< typename Rule >
         struct dusel< Rule, false >
         {
            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               constexpr bool use_action = ( A == apply_mode::ACTION ) && ( !is_nothing< Action, Rule >::value );
               constexpr char use_apply_void = use_action && has_apply< Action< Rule >, void, typename Input::action_t, States... >::value;
               constexpr char use_apply_bool = use_action && has_apply< Action< Rule >, bool, typename Input::action_t, States... >::value;
               constexpr char use_apply0_void = use_action && has_apply0< Action< Rule >, void, States... >::value;
               constexpr char use_apply0_bool = use_action && has_apply0< Action< Rule >, bool, States... >::value;
               static_assert( !use_action || use_apply_bool || use_apply_void || use_apply0_bool || use_apply0_void, "actions not disabled but no apply() or apply0() found" );
               static_assert( use_apply_void + use_apply_bool + use_apply0_void + use_apply0_bool < 2, "both apply() and apply0() defined" );
               constexpr dusel_mode mode = static_cast< dusel_mode >( 1 + use_apply_void + 2 * use_apply_bool + 3 * use_apply0_void + 4 * use_apply0_bool );
               return duseltronik< Rule, A, M, Action, Control, mode >::match( in, st... );
            }
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
