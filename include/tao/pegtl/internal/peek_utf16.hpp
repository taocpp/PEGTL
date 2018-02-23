// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP

#include <type_traits>

#include "../config.hpp"

#include "endian.hpp"
#include "input_pair.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename R >
         struct peek_utf16_impl
         {
            using data_t = char32_t;
            using pair_t = input_pair< char32_t >;

            using short_t = std::make_unsigned< char16_t >::type;

            static_assert( sizeof( short_t ) == 2, "expected size 2 for 16bit value" );
            static_assert( sizeof( char16_t ) == 2, "expected size 2 for 16bit value" );

            template< typename Input >
            static pair_t peek( Input& in ) noexcept( noexcept( in.size( 4 ) ) )
            {
               const std::size_t s = in.size( 4 );
               if( s >= 2 ) {
                  const char32_t t = R::read( in.current() );
                  if( ( t < 0xd800 ) || ( t > 0xdbff ) || ( s < 4 ) ) {
                     return { t, 2 };
                  }
                  const char32_t u = R::read( in.current() + 2 );
                  if( ( u < 0xdc00 ) || ( u > 0xdfff ) ) {
                     return { t, 2 };
                  }
                  return { ( ( ( t & 0x03ff ) << 10 ) | ( u & 0x03ff ) ) + 0x10000, 4 };
               }
               return { 0, 0 };
            }
         };

         struct read_utf16_be
         {
            static std::uint16_t read( const void* d ) noexcept
            {
               return be_to_h< std::uint16_t >( d );
            }
         };

         struct read_utf16_le
         {
            static std::uint16_t read( const void* d ) noexcept
            {
               return le_to_h< std::uint16_t >( d );
            }
         };

         using peek_utf16_be = peek_utf16_impl< read_utf16_be >;
         using peek_utf16_le = peek_utf16_impl< read_utf16_le >;

         using peek_utf16 = peek_utf16_le;

      }  // namespace internal

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#endif
