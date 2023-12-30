// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_MULTIPLE_HPP
#define TAO_PEGTL_INTERNAL_ASCII_MULTIPLE_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "one.hpp"
#include "peek_ascii8.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename >
   struct ascii_multiple;

   template< template< typename Endian, char... Cs > class String, typename Endian, char... Cs >
   struct ascii_multiple< String< Endian, Cs... > >
   {
      using string_t = String< Endian, Cs... >;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using data_t = std::decay_t< decltype( *in.current() ) >;
         static_assert( std::is_integral_v< data_t > || std::is_enum_v< data_t > );
         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            std::size_t index = 0;
            if constexpr( sizeof( data_t ) == 1 ) {
               if( ( string_t::template test< Cs >( static_cast< char >( *in.current( index++ ) ) ) && ... ) ) {
                  in.template consume< string_t >( sizeof...( Cs ) );
                  return true;
               }
            }
            else {
               using temp_t = typename integer_traits< sizeof( *in.current() ) >::unsigned_t;
               if( ( string_t::template test< Cs >( Endian::template get< temp_t >( in.current( index++ ) ) ) && ... ) ) {
                  in.template consume< string_t >( sizeof...( Cs ) );
                  return true;
               }
            }
         }
         return false;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
