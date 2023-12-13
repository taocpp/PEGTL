// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP

#include <type_traits>

#include "../config.hpp"

#include "endian.hpp"
#include "data_and_size.hpp"
#include "peek_endian.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_utf16_impl
   {
      using data_t = char32_t;
      using pair_t = data_and_size< char32_t >;

      static constexpr std::size_t fixed_size = 0;

      static_assert( sizeof( char16_t ) == 2 );
      static_assert( sizeof( char32_t ) == 4 );

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         if( const auto r = peek_endian< char16_t, Endian >::peek( in, offset ) ) {
            if( ( r.data() < 0xd800 ) || ( r.data() > 0xdfff ) ) {
               return pair_t( r.data(), r.size() );
            }
            if( r.data() < 0xdc00 ) {
               if( const auto s = peek_endian< char16_t, Endian >::peek( in, r.size() + offset ) ) {
                  if( ( s.data() >= 0xdc00 ) && ( s.data() <= 0xdfff ) ) {
                     return pair_t( ( ( char32_t( r.data() & 0x03ff ) << 10 ) | char32_t( s.data() & 0x03ff ) ) + 0x10000, std::uint8_t( r.size() + s.size() ) );
                  }
               }
            }
         }
         return pair_t();
      }
   };

   using peek_utf16_be = peek_utf16_impl< big_endian >;
   using peek_utf16_le = peek_utf16_impl< little_endian >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
