// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_size.hpp"
#include "unicode_utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_utf16_impl
   {
      using data_t = char32_t;
      using pair_t = data_and_size< char32_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 2 ) ) )
      {
         constexpr std::size_t s = integer_input_size< char16_t, ParseInput >();
         const std::size_t size = in.size( s + s + offset );

         if( size >= ( s + offset ) ) {
            const char16_t t = Endian::template get< char16_t >( in.current( offset ) );
            if( !is_utf16_surrogate( t ) ) {
               return pair_t( t, s );
            }
            if( is_utf16_high_surrogate( t ) ) {
               if( size >= ( s + s + offset ) ) {
                  const char16_t u = Endian::template get< char16_t >( in.current( s + offset ) );
                  if( is_utf16_low_surrogate( u ) ) {
                     return pair_t( utf16_surrogate_compose( t, u ), s + s );
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
