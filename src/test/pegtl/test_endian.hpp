// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_ENDIAN_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_ENDIAN_HPP

#include <cstddef>
#include <cstdint>

#include <tao/pegtl/config.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   // This implementation doesn't need to be fast, it needs to be diverse from
   // the one in tao/pegtl/include/endian.hpp etc. for the tests to be useful...
   // TODO: When can we finally switch to the C++20 and C++23 endian facilities?

   [[nodiscard]] inline bool is_be() noexcept
   {
      union
      {
         std::uint8_t a[ 2 ];
         std::uint16_t i;
      } u;
      static_assert( sizeof( u ) == 2 );
      u.i = 0x1af3;
      return u.a[ 0 ] == 0x1a;
   }

   [[nodiscard]] inline std::uint16_t b_swap( const std::uint16_t value ) noexcept
   {
      return ( ( value & 0x00ff ) << 8 ) | ( ( value & 0xff00 ) >> 8 );
   }

   [[nodiscard]] inline std::uint32_t b_swap( const std::uint32_t value ) noexcept
   {
      return ( ( value & 0x000000ff ) << 24 ) | ( ( value & 0xff000000 ) >> 24 ) | ( ( value & 0x00ff0000 ) >> 8 ) | ( ( value & 0x0000ff00 ) << 8 );
   }

   [[nodiscard]] inline std::uint16_t h_to_be( const std::uint16_t value ) noexcept
   {
      if( is_be() ) {
         return value;
      }
      return b_swap( value );
   }

   [[nodiscard]] inline std::uint16_t h_to_le( const std::uint16_t value ) noexcept
   {
      if( is_be() ) {
         return b_swap( value );
      }
      return value;
   }

   [[nodiscard]] inline std::uint32_t h_to_be( const std::uint32_t value ) noexcept
   {
      if( is_be() ) {
         return value;
      }
      return b_swap( value );
   }

   [[nodiscard]] inline std::uint32_t h_to_le( const std::uint32_t value ) noexcept
   {
      if( is_be() ) {
         return b_swap( value );
      }
      return value;
   }

}  // namespace TAO_PEGTL_NAMESPACE::test

#endif
