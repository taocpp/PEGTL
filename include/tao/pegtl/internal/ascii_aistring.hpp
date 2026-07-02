// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_AISTRING_HPP
#define TAO_PEGTL_INTERNAL_ASCII_AISTRING_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "is_simple_type.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::size_t Prefix, char... Cs >
   struct ascii_aistring
   {
      static_assert( Prefix > 0 );
      static_assert( Prefix < sizeof...( Cs ) );

      using rule_t = ascii_aistring;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using raw_t = typename ParseInput::data_t;

         static_assert( is_simple_type_v< raw_t > );
         static_assert( sizeof( raw_t ) == 1 );

         const std::size_t matched = ascii_aistring_equal< Cs... >( in.current(), in.size( sizeof...( Cs ) ) );

         if( matched == sizeof...( Cs ) ) {
            in.template consume< ascii_aistring >( sizeof...( Cs ) );
            return true;
         }
         if( matched >= Prefix ) {
            in.template consume< ascii_aistring >( Prefix );
            return true;
         }
         return false;
      }
   };

   template< std::size_t Prefix, char... Cs >
   inline constexpr bool enable_control< ascii_aistring< Prefix, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
