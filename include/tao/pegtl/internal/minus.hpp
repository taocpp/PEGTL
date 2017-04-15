// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_MINUS_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_MINUS_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename R, typename S >
         struct minus
         {
            using analyze_t = typename R::analyze_t;  // NOTE: S is currently ignored for analyze().

            template< apply_mode A,
                      rewind_mode,
                      template< typename... > class Action,
                      template< typename... > class Control,
                      typename Input,
                      typename... States >
            static bool match( Input& in, States&&... st )
            {
               auto m = in.template mark< rewind_mode::REQUIRED >();

               if( !Control< R >::template match< A, rewind_mode::ACTIVE, Action, Control >( in, st... ) ) {
                  return false;
               }
               using memory_t = typename Input::memory_t;
               memory_t i2( m.iterator(), in.begin(), in.source() );

               if( !Control< S >::template match< apply_mode::NOTHING, rewind_mode::ACTIVE, Action, Control >( i2, st... ) ) {
                  return m( true );
               }
               return m( !i2.empty() );
            }
         };

         template< typename R, typename S >
         struct skip_control< minus< R, S > > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
