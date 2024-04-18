// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_DECODE_HPP
#define TAO_PEGTL_INTERNAL_UTF8_DECODE_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "integer_adapt.hpp"
#include "utf16_details.hpp"
#include "utf8_details.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename ParseInput >
   [[nodiscard]] char32_and_size utf8_decode_any( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
   {
      const std::size_t size = in.size( offset + 4 );

      if( size >= ( offset + 1 ) ) {
         const char32_t c0 = integer_adapt< std::uint8_t >( in.current( offset ) );
         if( is_utf8_byte_1_of_1( c0 ) ) {
            return { c0, 1 };
         }
         if( is_utf8_byte_1_of_2( c0 ) ) {
            if( size >= ( offset + 2 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( offset + 1 ) );
               if( is_utf8_continuation( c1 ) ) {
                  const char32_t c = utf8_compose( c0, c1 );
                  if( c >= 0x80 ) {
                     return { c, 2 };
                  }
               }
            }
         }
         else if( is_utf8_byte_1_of_3( c0 ) ) {
            if( size >= ( offset + 3 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( offset + 1 ) );
               const char32_t c2 = integer_adapt< std::uint8_t >( in.current( offset + 2 ) );
               if( is_utf8_continuation( c1 ) && is_utf8_continuation( c2 ) ) {
                  const char32_t c = utf8_compose( c0, c1, c2 );
                  if( ( c >= 0x800 ) && ( !is_utf16_surrogate( c ) ) ) {
                     return { c, 3 };
                  }
               }
            }
         }
         else if( is_utf8_byte_1_of_4( c0 ) ) {
            if( size >= ( offset + 3 ) ) {
               const char32_t c1 = integer_adapt< std::uint8_t >( in.current( offset + 1 ) );
               const char32_t c2 = integer_adapt< std::uint8_t >( in.current( offset + 2 ) );
               const char32_t c3 = integer_adapt< std::uint8_t >( in.current( offset + 3 ) );
               if( is_utf8_continuation( c1 ) && is_utf8_continuation( c2 ) && is_utf8_continuation( c3 ) ) {
                  const char32_t c = utf8_compose( c0, c1, c2, c3 );
                  if( ( c >= 0x10000 ) && ( c <= 0x10ffff ) ) {
                     return { c, 4 };
                  }
               }
            }
         }
      }
      return {};
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
