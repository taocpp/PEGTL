// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ISTRING_HPP
#define TAO_PEGTL_INTERNAL_ISTRING_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "ione.hpp"
#include "peek_direct.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char... Cs >
   struct istring
   {
      using rule_t = istring;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using data_t = std::decay_t< decltype( *in.current() ) >;
         static_assert( sizeof( data_t ) == 1 );
         static_assert( std::is_integral_v< data_t > || std::is_enum_v< data_t > );
         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            if( ascii_istring_equal< Cs... >( in.current() ) ) {
               in.template consume< istring >( sizeof...( Cs ) );
               return true;
            }
         }
         return false;
      }
   };

   template< char C >
   struct istring< C >
      : ione< peek_char, C >
   {};

   template<>
   struct istring<>
      : success
   {};

   template< char... Cs >
   inline constexpr bool enable_control< istring< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
