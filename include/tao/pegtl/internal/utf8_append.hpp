// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_APPEND_HPP
#define TAO_PEGTL_INTERNAL_UTF8_APPEND_HPP

#include <cstddef>
#include <string>

#include "../config.hpp"

#include "utf16_details.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   inline void utf8_append_1_impl( std::string& s, const char32_t u ) noexcept
   {
      s += char( u );
   }

   inline void utf8_append_2_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         char( ( ( u >> 6 ) & 0x1f ) | 0xc0 ),
         char( ( u & 0x3f ) | 0x80 )
      };
      s.append( t, sizeof( t ) );
   }

   inline void utf8_append_3_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         char( ( ( u >> 12 ) & 0x0f ) | 0xe0 ),
         char( ( ( u >> 6 ) & 0x3f ) | 0x80 ),
         char( ( u & 0x3f ) | 0x80 )
      };
      s.append( t, sizeof( t ) );
   }

   inline void utf8_append_4_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         char( ( ( u >> 18 ) & 0x07 ) | 0xf0 ),
         char( ( ( u >> 12 ) & 0x3f ) | 0x80 ),
         char( ( ( u >> 6 ) & 0x3f ) | 0x80 ),
         char( ( u & 0x3f ) | 0x80 )
      };
      s.append( t, sizeof( t ) );
   }

   [[nodiscard]] inline bool utf8_append_utf16( std::string& s, const char16_t u ) noexcept
   {
      if( u <= 0x7f ) {
         utf8_append_1_impl( s, u );
         return true;
      }
      if( u <= 0x7ff ) {
         utf8_append_2_impl( s, u );
         return true;
      }
      if( !is_utf16_surrogate( u ) ) {
         utf8_append_3_impl( s, u );
         return true;
      }
      return false;
   }

   inline void utf8_append_utf16( std::string& s, const char16_t v, const char16_t w ) noexcept
   {
      // assert( is_utf16_high_surrogate( v ) );
      // assert( is_utf16_low_surrogate( w ) );
      const char32_t u = utf16_surrogate_compose( v, w );
      utf8_append_4_impl( s, u );
   }

   [[nodiscard]] inline bool utf8_append_utf32( std::string& s, const char32_t u )
   {
      // assert( is_utf32_codepoint( u ) );

      if( u <= 0x7f ) {
         utf8_append_1_impl( s, u );
         return true;
      }
      if( u <= 0x7ff ) {
         utf8_append_2_impl( s, u );
         return true;
      }
      if( u <= 0xffff ) {
         if( !is_utf16_surrogate( u ) ) {
            utf8_append_3_impl( s, u );
            return true;
         }
         return false;
      }
      if( u <= 0x10ffff ) {
         utf8_append_4_impl( s, u );
         return true;
      }
      return false;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
