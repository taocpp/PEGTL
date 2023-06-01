// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ISTRING_HPP
#define TAO_PEGTL_INTERNAL_ISTRING_HPP

#include <type_traits>

#include "bump_help.hpp"
#include "enable_control.hpp"
#include "one.hpp"
#include "result_on_found.hpp"
#include "success.hpp"

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char C >
   inline constexpr bool is_alpha = ( ( 'a' <= C ) && ( C <= 'z' ) ) || ( ( 'A' <= C ) && ( C <= 'Z' ) );

   template< char C >
   [[nodiscard]] constexpr bool ichar_equal( const char c ) noexcept
   {
      if constexpr( is_alpha< C > ) {
         return ( C | 0x20 ) == ( c | 0x20 );
      }
      else {
         return c == C;
      }
   }

   template< char... Cs >
   [[nodiscard]] constexpr bool istring_equal( const char* r ) noexcept
   {
      return ( ichar_equal< Cs >( *r++ ) && ... );
   }

   template< char... Cs >
   struct istring;

   template<>
   struct istring<>
      : success
   {};

   // template< char C >
   // struct istring< C >
   //    : std::conditional_t< is_alpha< C >, one< result_on_found::success, peek_char, C | 0x20, C & ~0x20 >, one< result_on_found::success, peek_char, C > >
   // {};

   template< char... Cs >
   struct istring
   {
      using rule_t = istring;
      using subs_t = empty_list;

      [[nodiscard]] static constexpr bool test_one( const char c ) noexcept
      {
         static_assert( sizeof...( Cs ) == 1 );
         return one< result_on_found::success, peek_char, Cs... >::test_one( c );
      }

      [[nodiscard]] static constexpr bool test_any( const char c ) noexcept
      {
         return one< result_on_found::success, peek_char, Cs... >::test_one( c );
      }

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 0 ) ) )
      {
         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            if( istring_equal< Cs... >( in.current() ) ) {
               bump_help< istring >( in, sizeof...( Cs ) );
               return true;
            }
         }
         return false;
      }
   };

   template< char... Cs >
   inline constexpr bool enable_control< istring< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
