// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_REMATCH_HPP
#define TAO_PEGTL_INTERNAL_REMATCH_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../apply_mode.hpp"
#include "../memory_input.hpp"
#include "../rewind_mode.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         [[nodiscard]] inline const char* source_pointer( const char* source ) noexcept
         {
            return source;
         }

         [[nodiscard]] inline const char* source_pointer( const std::string& source ) noexcept
         {
            return source.c_str();
         }

         template< typename R, typename S >
         struct rematch
         {
            using analyze_t = typename R::analyze_t;  // NOTE: S is ignored for analyze().

            template< apply_mode A,
                      rewind_mode,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename Input,
                      typename... States >
            [[nodiscard]] static bool match( Input& in, States&&... st )
            {
               auto m = in.template mark< rewind_mode::required >();

               if( Control< R >::template match< A, rewind_mode::active, Action, Control >( in, st... ) ) {
                  memory_input< Input::tracking_mode_v, typename Input::eol_t, const char* > i2( m.iterator(), in.current(), source_pointer( in.source() ) );
                  return m( Control< S >::template match< A, rewind_mode::active, Action, Control >( i2, st... ) );
               }
               return false;
            }
         };

         template< typename R, typename S >
         inline constexpr bool skip_control< rematch< R, S > > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
