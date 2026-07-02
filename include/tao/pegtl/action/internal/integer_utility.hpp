// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_INTEGER_UTILITY_HPP
#define TAO_PEGTL_ACTION_INTERNAL_INTEGER_UTILITY_HPP

#include <cstddef>
#include <limits>
#include <string>

#include "../../config.hpp"
#include "../../type_list.hpp"

#include "throw_or_terminate.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   inline constexpr bool is_proper_signed_v = type_list_contains_v< T, type_list< signed char, signed short, signed int, signed long, signed long long > >;

   template< typename T >
   inline constexpr bool is_proper_unsigned_v = type_list_contains_v< T, type_list< unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long > >;

   template< typename T >
   inline constexpr bool is_proper_integer_v = is_proper_signed_v< T > || is_proper_unsigned_v< T >;

   template< typename Object, typename ActionInput >
   [[nodiscard]] Object convert_signed( const ActionInput& in )
   {
      std::size_t pos = 0;

      const signed long long result = std::stoll( in.string(), &pos );

      if( pos != in.size() ) {
         throw_or_terminate( "trailing garbage after signed integer", in );
      }

      if constexpr( std::numeric_limits< Object >::digits < std::numeric_limits< signed long long >::digits ) {
         if( ( result < static_cast< signed long long >( std::numeric_limits< Object >::min() ) ) ||
             ( result > static_cast< signed long long >( std::numeric_limits< Object >::max() ) ) ) {
            throw_or_terminate( "signed integer out of range", in );
         }
      }

      return static_cast< Object >( result );
   }

   template< typename Object, typename ActionInput >
   [[nodiscard]] Object convert_unsigned( const ActionInput& in )
   {
      std::size_t pos = 0;

      const unsigned long long result = std::stoull( in.string(), &pos );

      if( pos != in.size() ) {
         throw_or_terminate( "trailing garbage after unsigned integer", in );
      }

      if constexpr( std::numeric_limits< Object >::digits < std::numeric_limits< unsigned long long >::digits ) {
         if( result > static_cast< unsigned long long >( std::numeric_limits< Object >::max() ) ) {
            throw_or_terminate( "unsigned integer out of range", in );
         }
      }
      return static_cast< Object >( result );
   }

   template< typename Object, typename ActionInput >
   [[nodiscard]] Object convert_integer( const ActionInput& in )
   {
      if constexpr( is_proper_signed_v< Object > ) {
         return convert_signed< Object >( in );
      }
      else {
         return convert_unsigned< Object >( in );

      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
