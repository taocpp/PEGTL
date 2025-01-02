// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_ISTRING_HPP
#define TAO_PEGTL_INTERNAL_ASCII_ISTRING_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "ione.hpp"
#include "peek_direct.hpp"
#include "success.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char... Cs >
   struct ascii_istring
   {
      using rule_t = ascii_istring;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 0 ) ) )
      {
         using raw_t = typename ParseInput::data_t;

         static_assert( is_simple_type_v< raw_t > );
         static_assert( sizeof( raw_t ) == 1 );

         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            if( ascii_istring_equal< Cs... >( in.current() ) ) {
               in.template consume< ascii_istring >( sizeof...( Cs ) );
               return true;
            }
         }
         return false;
      }
   };

   template< char C >
   struct ascii_istring< C >
      : ione< peek_char, C >
   {};

   template<>
   struct ascii_istring<>
      : success
   {};

   template< char... Cs >
   inline constexpr bool enable_control< ascii_istring< Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
