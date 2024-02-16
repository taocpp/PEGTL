// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF8_APPEND_HPP
#define TAO_PEGTL_INTERNAL_UTF8_APPEND_HPP

#include <cstddef>
#include <string>

#include "../config.hpp"

#include "utf16_details.hpp"
#include "utf8_constants.hpp"
#include "utf8_details.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   inline void utf8_append_1_impl( std::string& s, const char32_t u ) noexcept
   {
      s += char( u );
   }

   inline void utf8_append_2_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         utf8_char_1_of_2( u ),
         utf8_char_2_of_2( u )
      };
      s.append( t, sizeof( t ) );
   }

   inline void utf8_append_3_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         utf8_char_1_of_3( u ),
         utf8_char_2_of_3( u ),
         utf8_char_3_of_3( u )
      };
      s.append( t, sizeof( t ) );
   }

   inline void utf8_append_4_impl( std::string& s, const char32_t u ) noexcept
   {
      const char t[] = {
         utf8_char_1_of_4( u ),
         utf8_char_2_of_4( u ),
         utf8_char_3_of_4( u ),
         utf8_char_4_of_4( u )
      };
      s.append( t, sizeof( t ) );
   }

   [[nodiscard]] inline bool utf8_append_utf16( std::string& s, const char16_t u ) noexcept
   {
      if( u <= utf8_max_length_1 ) {
         utf8_append_1_impl( s, u );
         return true;
      }
      if( u <= utf8_max_length_2 ) {
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
      const char32_t u = utf16_compose( v, w );
      utf8_append_4_impl( s, u );
   }

   [[nodiscard]] inline bool utf8_append_utf32( std::string& s, const char32_t u )
   {
      // assert( is_utf32( u ) );

      if( u <= utf8_max_length_1 ) {
         utf8_append_1_impl( s, u );
         return true;
      }
      if( u <= utf8_max_length_2 ) {
         utf8_append_2_impl( s, u );
         return true;
      }
      if( u <= utf8_max_length_3 ) {
         if( !is_utf16_surrogate( u ) ) {
            utf8_append_3_impl( s, u );
            return true;
         }
         return false;
      }
      if( u <= utf8_max_length_4 ) {
         utf8_append_4_impl( s, u );
         return true;
      }
      return false;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
