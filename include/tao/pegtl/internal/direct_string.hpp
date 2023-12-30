// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DIRECT_STRING_HPP
#define TAO_PEGTL_INTERNAL_DIRECT_STRING_HPP

#include <cstring>
#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "one.hpp"
#include "peek_direct.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   [[nodiscard]] inline bool direct_string_equal( const void* s, const std::initializer_list< char >& l ) noexcept
   {
      return std::memcmp( s, l.begin(), l.size() ) == 0;
   }

   template< char... Cs >
   struct direct_string
   {
      using rule_t = direct_string;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using data_t = std::decay_t< decltype( *in.current() ) >;
         static_assert( sizeof( data_t ) == 1 );
         static_assert( std::is_integral_v< data_t > || std::is_enum_v< data_t > );
         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            if( direct_string_equal( in.current(), { Cs... } ) ) {
               in.template consume< direct_string >( sizeof...( Cs ) );
               return true;
            }
         }
         return false;
      }
   };

   template< char C >
   struct direct_string< C >
      : one< peek_char, C >
   {};

   template<>
   struct direct_string<>
      : success
   {};

   template< char... Cs >
   inline constexpr bool enable_control< direct_string< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
