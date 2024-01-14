// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BYTESWAP_WIN32_HPP
#define TAO_PEGTL_INTERNAL_BYTESWAP_WIN32_HPP

#include <cstdint>
#include <type_traits>

#include "../config.hpp"

#include "dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   [[nodiscard]] T byteswap( const T n ) noexcept
   {
      static_assert( std::is_integral_v< T > || std::is_enum_v< T > );

      if constexpr( sizeof( T ) == 1 ) {
         return n;
      }
      else if constexpr( sizeof( T ) == 2 ) {
         return static_cast< T >( _byteswap_ushort( static_cast< std::uint16_t >( n ) ) );
      }
      else if constexpr( sizeof( T ) == 4 ) {
         return static_cast< T >( _byteswap_ulong( static_cast< std::uint32_t >( n ) ) );
      }
      else if constexpr( sizeof( T ) == 8 ) {
         return static_cast< T >( _byteswap_uint64( static_cast< std::uint64_t >( n ) ) );
      }
      else {
         static_assert( dependent_false< T > );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
