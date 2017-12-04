// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_NORMAL_HPP
#define TAOCPP_PEGTL_INCLUDE_NORMAL_HPP

#include <utility>

#include "apply_mode.hpp"
#include "config.hpp"
#include "parse_error.hpp"
#include "rewind_mode.hpp"

#include "internal/demangle.hpp"
#include "internal/match.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule >
      struct normal
      {
         template< typename Input, typename... States >
         static void start( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void success( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void failure( const Input&, States&&... ) noexcept
         {
         }

         template< typename Input, typename... States >
         static void raise( const Input& in, States&&... )
         {
            throw parse_error( "parse error matching " + internal::demangle< Rule >(), in );
         }

         template< template< typename... > class Action, typename Input, typename... States >
         static auto apply0( const Input&, States&&... st ) -> decltype( Action< Rule >::apply0( st... ) )
         {
            return Action< Rule >::apply0( st... );
         }

         template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
         static auto apply( const Iterator& begin, const Input& in, States&&... st ) -> decltype( Action< Rule >::apply( std::declval< typename Input::action_t >(), st... ) )
         {
            using action_t = typename Input::action_t;
            const action_t action_input( begin, in );
            return Action< Rule >::apply( action_input, st... );
         }

         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename Input,
                   typename... States >
         static bool match( Input& in, States&&... st )
         {
            return internal::dusel< Rule >::template match< A, M, Action, Control >( in, st... );
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
