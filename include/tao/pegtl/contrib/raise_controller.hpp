// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_RAISE_CONTROLLER_HPP
#define TAO_PEGTL_CONTRIB_RAISE_CONTROLLER_HPP

#include "../config.hpp"
#include "../normal.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename T, typename Rule, typename = void >
      inline constexpr bool raise_on_failure = ( T::template message< Rule > != nullptr );

      template< typename T, typename Rule >
      inline constexpr bool raise_on_failure< T, Rule, decltype( T::template raise_on_failure< Rule >, void() ) > = T::template raise_on_failure< Rule >;

   }  // namespace internal

   template< typename T, bool RequireMessage = true, template< typename... > class Base = normal >
   struct raise_controller
   {
      template< typename Rule >
      struct control
         : Base< Rule >
      {
         template< typename Input, typename... States >
         static void failure( const Input& in, States&&... st ) noexcept( !internal::raise_on_failure< T, Rule > && noexcept( Base< Rule >::failure( in, st... ) ) )
         {
            if constexpr( internal::raise_on_failure< T, Rule > ) {
               raise( in, st... );
            }
            else {
               Base< Rule >::failure( in, st... );
            }
         }

         template< typename Input, typename... States >
         [[noreturn]] static void raise( const Input& in, States&&... st )
         {
            if constexpr( RequireMessage ) {
               static_assert( T::template message< Rule > != nullptr );
            }
            if constexpr( T::template message< Rule > != nullptr ) {
               constexpr const char* p = T::template message< Rule >;
               throw parse_error( p, in );
#if defined( _MSC_VER )
               (void)( (void)st, ... );
#endif
            }
            else {
               Base< Rule >::raise( in, st... );
            }
         }
      };
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
