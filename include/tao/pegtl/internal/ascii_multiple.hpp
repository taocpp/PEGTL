// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_MULTIPLE_HPP
#define TAO_PEGTL_INTERNAL_ASCII_MULTIPLE_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "enable_control.hpp"
#include "integer_traits.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename >
   struct ascii_multiple;

   template< template< typename Endian, char... Cs > class String, typename Endian, char... Cs >
   struct ascii_multiple< String< Endian, Cs... > >
   {
      using rule_t = ascii_multiple;
      using subs_t = empty_list;

      static_assert( sizeof...( Cs ) > 0 );

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using data_t = std::decay_t< decltype( *in.current() ) >;
         static_assert( is_simple_type_v< data_t > );
         if( in.size( sizeof...( Cs ) ) >= sizeof...( Cs ) ) {
            std::size_t index = 0;
            if constexpr( sizeof( data_t ) == 1 ) {
               if( ( String< Endian, Cs... >::template test< Cs >( static_cast< char >( *in.current( index++ ) ) ) && ... ) ) {
                  in.template consume< ascii_multiple >( sizeof...( Cs ) );
                  return true;
               }
            }
            else {
               using temp_t = typename integer_traits< sizeof( *in.current() ) >::unsigned_t;
               if( ( String< Endian, Cs... >::template test< Cs >( Endian::template get< temp_t >( in.current( index++ ) ) ) && ... ) ) {
                  in.template consume< ascii_multiple >( sizeof...( Cs ) );
                  return true;
               }
            }
         }
         return false;
      }
   };

   template< typename String >
   inline constexpr bool enable_control< ascii_multiple< String > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
