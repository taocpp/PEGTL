// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_UNESCAPE_HPP
#define TAO_PEGTL_EXTRA_UNESCAPE_HPP

#include <cassert>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <string>

#include "../nothing.hpp"

#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif

#include "../example/escaped.hpp"

#include "../internal/peek_direct.hpp"
#include "../internal/terminal_aliases.hpp"

#include "../unicode/utf8.hpp"

#include "../unicode/internal/utf16_details.hpp"
#include "../unicode/internal/utf8_append.hpp"

#include "internal/unhex_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct unescape
      : nothing< Rule >
   {};

   struct unescape_append
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& out )
      {
         out.append( in.string_view() );
      }
   };

   template<>
   struct unescape< any >
      : unescape_append
   {};

   template<>
   struct unescape< any7 >
      : unescape_append
   {};

   template<>
   struct unescape< utf8::any >
      : unescape_append
   {};

   template<>
   struct unescape< utf8::range< 0x20, 0x10FFFF > >
      : unescape_append
   {};

   template< char... Rs >
   struct unescape_char
   {
      static_assert( sizeof...( Rs ) > 0 );

      template< typename One, typename ActionInput >
      static void apply( const ActionInput& in, std::string& out )
      {
         // assert( in.size() == 1 );
         out += apply_impl( in.peek_char(), One() );
      }

   protected:
      template< char... Qs >
      [[nodiscard]] static char apply_impl( const char c, const internal::one< internal::peek_char, Qs... > /*unused*/ ) noexcept
      {
         static_assert( sizeof...( Qs ) == sizeof...( Rs ), "Size mismatch between escaped characters and their mapping!" );
         return apply_impl( c, { Qs... }, { Rs... } );
      }

      [[nodiscard]] static char apply_impl( const char c, const std::initializer_list< char >& q, const std::initializer_list< char >& r ) noexcept
      {
         for( std::size_t i = 0; i < q.size(); ++i ) {
            if( *( q.begin() + i ) == c ) {
               return *( r.begin() + i );
            }
         }
         std::terminate();  // LCOV_EXCL_LINE
      }
   };

   // Yes, I know, \0 is one of many possible octal escape sequences in C and C++,
   // but in 2026 nobody is using octal numbers outside of filesystem permissions?

   struct c_unescape_char
      : unescape_char< '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\0' >
   {};

   template<>
   struct unescape< c_escaped_char >
      : c_unescape_char
   {};

   struct json_unescape_char
      : unescape_char< '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' >
   {};

   template<>
   struct unescape< json_escaped_char >
      : json_unescape_char
   {};

   struct unescape_hex
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& out )
      {
         // assert( in contains one or more hex digits );
         out += internal::unhex_string< 2, char >( in.begin() );
      }
   };

   template<>
   struct unescape< hex_char_xdigits >
      : unescape_hex
   {};

   template< std::size_t N >
   struct unescape_unicode
   {
#if defined( __cpp_exceptions )
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& out )
      {
         // assert( in contains one or more hex digits );
         if( !internal::utf8_append_utf32( out, internal::unhex_string< N, char32_t >( in.begin() ) ) ) {
            throw_parse_error( "invalid escaped unicode code point", in );
         }
      }
#else
      template< typename ActionInput >
      [[nodiscard]] static bool apply( const ActionInput& in, std::string& out )
      {
         // assert( in contains one or more hex digits );
         return internal::utf8_append_utf32( out, internal::unhex_string< N, char32_t >( in.begin() ) );
      }
#endif
   };

   template< std::size_t N >
   struct unescape< rep_unicode_xdigits< N > >
      : unescape_unicode< N >
   {};

   // Unicode unescape for JSON which merges consecutive
   // escaped UTF-16 surrogates as required by RFC 8259.
   // It does not double-check the two bytes 'backslash'
   // and 'u' between individual escaped 16-bit values.

   struct json_unescape_unicode
   {
      template< typename ActionInput >
      [[nodiscard]] static auto apply( const ActionInput& in, std::string& out )
      {
         // assert( ( ( in.size() + 1 ) % 6 ) == 0 );
         for( const char* b = in.begin() + 1; b < in.end(); b += 6 ) {
            const auto c = internal::unhex_string< 4, char16_t >( b );
            if( internal::is_utf16_high_surrogate( c ) && ( b + 6 < in.end() ) ) {
               const auto d = internal::unhex_string< 4, char16_t >( b + 6 );
               if( internal::is_utf16_low_surrogate( d ) ) {
                  b += 6;
                  internal::utf8_append_utf16( out, c, d );
                  continue;
               }
            }
            if( !internal::utf8_append_utf16( out, c ) ) {
#if defined( __cpp_exceptions )
               throw_parse_error( "invalid escaped unicode code point", in );
#else
               return false;
#endif
            }
         }
#if defined( __cpp_exceptions )
         return;  // void
#else
         return true;
#endif
      }
   };

   template<>
   struct unescape< json_escaped_unicode >
      : json_unescape_unicode
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
