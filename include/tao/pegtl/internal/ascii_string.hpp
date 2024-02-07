// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_STRING_HPP
#define TAO_PEGTL_INTERNAL_ASCII_STRING_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "one.hpp"
#include "peek_direct.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char... Cs >
   struct ascii_string
   {
      using rule_t = ascii_string;
      using subs_t = empty_list;

      static_assert( sizeof...( Cs ) > 0 );

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using raw_t = std::decay_t< decltype( *in.current() ) >;

         static_assert( sizeof( raw_t ) == 1 );
         static_assert( std::is_integral_v< raw_t > || std::is_enum_v< raw_t > );

         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            if( char_string_equal( in.current(), { Cs... } ) ) {
               in.template consume< ascii_string >( sizeof...( Cs ) );
               return true;
            }
         }
         return false;
      }
   };

   template< char C >
   struct ascii_string< C >
      : one< peek_char, C >
   {};

   template<>
   struct ascii_string<>
      : success
   {};

   template< char... Cs >
   inline constexpr bool enable_control< ascii_string< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
