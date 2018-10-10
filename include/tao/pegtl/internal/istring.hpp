// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_ISTRING_HPP
#define TAO_PEGTL_INTERNAL_ISTRING_HPP

#include <type_traits>

#include "../config.hpp"

#include "bump_help.hpp"
#include "result_on_found.hpp"
#include "skip_control.hpp"
#include "trivial.hpp"

#include "../analysis/counted.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< char C >
         inline constexpr bool is_alpha = ( ( 'a' <= C ) && ( C <= 'z' ) ) || ( ( 'A' <= C ) && ( C <= 'Z' ) );

         template< char C >
         bool ichar_equal( const char c ) noexcept
         {
            if constexpr( is_alpha< C > ) {
               return ( C | 0x20 ) == ( c | 0x20 );
            }
            else {  // NOLINT
               return c == C;
            }
         }

         template< char... Cs >
         bool istring_equal( const char* r ) noexcept
         {
            return ( ichar_equal< Cs >( *r++ ) && ... );
         }

         template< char... Cs >
         struct istring;

         template<>
         struct istring<>
            : trivial< true >
         {
         };

         template< char... Cs >
         struct istring
         {
            using analyze_t = analysis::counted< analysis::rule_type::ANY, sizeof...( Cs ) >;

            template< typename Input >
            static bool match( Input& in ) noexcept( noexcept( in.size( 0 ) ) )
            {
               if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
                  if( istring_equal< Cs... >( in.current() ) ) {
                     bump_help< result_on_found::SUCCESS, Input, char, Cs... >( in, sizeof...( Cs ) );
                     return true;
                  }
               }
               return false;
            }
         };

         template< char... Cs >
         inline constexpr bool skip_control< istring< Cs... > > = true;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
