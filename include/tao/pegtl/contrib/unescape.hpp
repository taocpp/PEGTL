// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_UNESCAPE_HPP
#define TAO_PEGTL_CONTRIB_UNESCAPE_HPP

#include <cassert>
#include <exception>
#include <initializer_list>
#include <string>

#include "../ascii.hpp"
#include "../config.hpp"

#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif

#include "../internal/one.hpp"
#include "../internal/peek_direct.hpp"
#include "../internal/unhex_utility.hpp"
#include "../internal/utf16_details.hpp"
#include "../internal/utf8_append.hpp"

namespace TAO_PEGTL_NAMESPACE::unescape
{
   struct append_all
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& s )
      {
         s.append( in.string_view() );
      }
   };

   // The unescape_c action MUST be called for a character matching One which MUST
   // be a rule with the same rule_t as TAO_PEGTL_NAMESPACE::ascii::one< ... >.

   template< typename One, char... Rs >
   struct unescape_c
   {
      using one_t = typename One::test_t;

      static_assert( sizeof...( Rs ) > 0 );

      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& s )
      {
         // assert( in.size() == 1 );
         s += apply_one( in.peek_char(), one_t() );
      }

   private:
      template< char... Qs >
      [[nodiscard]] static char apply_one( const char c, const internal::one< internal::peek_char, Qs... > /*unused*/ ) noexcept
      {
         static_assert( sizeof...( Qs ) == sizeof...( Rs ), "Size mismatch between escaped characters and their mappings!" );
         return apply_two( c, { Qs... }, { Rs... } );
      }

      [[nodiscard]] static char apply_two( const char c, const std::initializer_list< char >& q, const std::initializer_list< char >& r ) noexcept
      {
         for( std::size_t i = 0; i < q.size(); ++i ) {
            if( *( q.begin() + i ) == c ) {
               return *( r.begin() + i );
            }
         }
         std::terminate();  // LCOV_EXCL_LINE
      }
   };

   // See src/example/pegtl/unescape.cpp for why the following two actions skip the
   // first input character. They also MUST be called with non-empty matched inputs!

   struct unescape_u
   {
#if defined( __cpp_exceptions )
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& s )
      {
         // assert( !in.empty() );  // First character MUST be present, usually 'u' or 'U'.
         if( !internal::utf8_append_utf32( s, internal::unhex_string_impl< char32_t >( in.begin() + 1, in.end() ) ) ) {
            throw_parse_error( "invalid escaped unicode code point", in );
         }
      }
#else
      template< typename ActionInput >
      static bool apply( const ActionInput& in, std::string& s )
      {
         // assert( !in.empty() );  // First character MUST be present, usually 'u' or 'U'.
         return internal::utf8_append_utf32( s, internal::unhex_string_impl< char32_t >( in.begin() + 1, in.end() ) );
      }
#endif
   };

   struct unescape_x
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& s )
      {
         // assert( !in.empty() );  // First character MUST be present, usually 'x'.
         s += internal::unhex_string_impl< char >( in.begin() + 1, in.end() );
      }
   };

   // The unescape_j action is similar to unescape_u, however unlike
   // unescape_u it
   // (a) assumes exactly 4 hexdigits per escape sequence,
   // (b) accepts multiple consecutive escaped 16-bit values.
   // When applied to more than one escape sequence, unescape_j
   // translates UTF-16 surrogate pairs in the input into a single
   // UTF-8 sequence in s, as required for JSON by RFC 8259.

   struct unescape_j
   {
      template< typename ActionInput >
      static bool apply( const ActionInput& in, std::string& s )
      {
         // Expects multiple "\\u1234", starting with the first "u".
         // assert( ( ( in.size() + 1 ) % 6 ) == 0 );
         for( const char* b = in.begin() + 1; b < in.end(); b += 6 ) {
            const auto c = internal::unhex_string_impl< char16_t >( b, b + 4 );
            if( internal::is_utf16_high_surrogate( c ) && ( b + 6 < in.end() ) ) {
               const auto d = internal::unhex_string_impl< char16_t >( b + 6, b + 10 );
               if( internal::is_utf16_low_surrogate( d ) ) {
                  b += 6;
                  internal::utf8_append_utf16( s, c, d );
                  continue;
               }
            }
            if( !internal::utf8_append_utf16( s, c ) ) {
#if defined( __cpp_exceptions )
               throw_parse_error( "invalid escaped unicode code point", in );
#else
               return false;
#endif
            }
         }
         return true;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::unescape

#endif
