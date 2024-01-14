// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF8_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF8_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "integer_adapt.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct peek_utf8
   {
      using data_t = char32_t;
      using pair_t = data_and_size< char32_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.empty() ) )
      {
         if( in.size( offset + 1 ) >= ( offset + 1 ) ) {
            const char32_t c0 = integer_adapt< std::uint8_t >( in.current( offset ) );
            if( ( c0 & 0x80 ) == 0 ) {
               return pair_t( c0, 1 );
            }
            return peek_impl( in, c0, offset );
         }
         return pair_t();
      }

   private:
      template< typename ParseInput >
      [[nodiscard]] static pair_t peek_impl( ParseInput& in, char32_t c0, const std::size_t offset ) noexcept( noexcept( in.size( 42 ) ) )
      {
         if( ( c0 & 0xE0 ) == 0xC0 ) {
            if( in.size( offset + 2 ) >= ( offset + 2 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( 1 + offset ) );
               if( ( c1 & 0xC0 ) == 0x80 ) {
                  c0 &= 0x1F;
                  c0 <<= 6;
                  c0 |= ( c1 & 0x3F );
                  if( c0 >= 0x80 ) {
                     return pair_t( c0, 2 );
                  }
               }
            }
         }
         else if( ( c0 & 0xF0 ) == 0xE0 ) {
            if( in.size( offset + 3 ) >= ( offset + 3 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( 1 + offset ) );
               const char32_t c2 = integer_adapt< std::uint8_t >( in.current( 2 + offset ) );
               if( ( ( c1 & 0xC0 ) == 0x80 ) && ( ( c2 & 0xC0 ) == 0x80 ) ) {
                  c0 &= 0x0F;
                  c0 <<= 6;
                  c0 |= ( c1 & 0x3F );
                  c0 <<= 6;
                  c0 |= ( c2 & 0x3F );
                  if( c0 >= 0x800 && !( ( c0 >= 0xD800 ) && ( c0 <= 0xDFFF ) ) ) {
                     return pair_t( c0, 3 );
                  }
               }
            }
         }
         else if( ( c0 & 0xF8 ) == 0xF0 ) {
            if( in.size( offset + 4 ) >= ( offset + 4 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( 1 + offset ) );
               const char32_t c2 = integer_adapt< std::uint8_t >( in.current( 2 + offset ) );
               const char32_t c3 = integer_adapt< std::uint8_t >( in.current( 3 + offset ) );
               if( ( ( c1 & 0xC0 ) == 0x80 ) && ( ( c2 & 0xC0 ) == 0x80 ) && ( ( c3 & 0xC0 ) == 0x80 ) ) {
                  c0 &= 0x07;
                  c0 <<= 6;
                  c0 |= ( c1 & 0x3F );
                  c0 <<= 6;
                  c0 |= ( c2 & 0x3F );
                  c0 <<= 6;
                  c0 |= ( c3 & 0x3F );
                  if( ( c0 >= 0x10000 ) && ( c0 <= 0x10FFFF ) ) {
                     return pair_t( c0, 4 );
                  }
               }
            }
         }
         return { 0, 0 };
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
