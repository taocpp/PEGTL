// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_APPLY_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_APPLY_HPP

#include "../config.hpp"

#include "skip_control.hpp"
#include "trivial.hpp"

#include "../analysis/counted.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Action, typename >
         struct apply_single;

         template< typename Action >
         struct apply_single< Action, void >
         {
            template< typename Input, typename... States >
            static bool match( const Input& i2, States&&... st )
            {
               Action::apply( i2, st... );
               return true;
            }
         };

         template< typename Action >
         struct apply_single< Action, bool >
         {
            template< typename Input, typename... States >
            static bool match( const Input& i2, States&&... st )
            {
               return Action::apply( i2, st... );
            }
         };

         template< apply_mode A, typename... Actions >
         struct apply_impl;

         template<>
         struct apply_impl< apply_mode::ACTION >
         {
            template< typename Input, typename... States >
            static bool match( Input&, States&&... )
            {
               return true;
            }
         };

         template< typename... Actions >
         struct apply_impl< apply_mode::ACTION, Actions... >
         {
            template< typename Input, typename... States >
            static bool match( Input& in, States&&... st )
            {
               using action_t = typename Input::action_t;
               const action_t i2( in.iterator(), in );  // No data -- range is from begin to begin.
#ifdef __cpp_fold_expressions
               return ( apply_single< Actions, decltype( Actions::apply( i2, st... ) ) >::match( i2, st... ) && ... );
#else
               bool result = true;
               using swallow = bool[];
               (void)swallow{ result = result && apply_single< Actions, decltype( Actions::apply( i2, st... ) ) >::match( i2, st... )... };
               return result;
#endif
            }
         };

         template< typename... Actions >
         struct apply_impl< apply_mode::NOTHING, Actions... >
         {
            template< typename Input, typename... States >
            static bool match( Input&, States&&... )
            {
               return true;
            }
         };

         template< typename... Actions >
         struct apply
         {
            using analyze_t = analysis::counted< analysis::rule_type::ANY, 0 >;

            template< apply_mode A,
                      rewind_mode M,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               return apply_impl< A, Actions... >::match( in, st... );
            }
         };

         template< typename... Actions >
         struct skip_control< apply< Actions... > > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
