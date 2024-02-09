// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF16_DECODE_HPP
#define TAO_PEGTL_INTERNAL_UTF16_DECODE_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "integer_size.hpp"
#include "utf16_details.hpp"
#include "unicode_constants.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian, typename ParseInput >
   [[nodiscard]] char32_and_size utf16_decode_any( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 2 ) ) )
   {
      constexpr std::size_t s = integer_input_size< char16_t, ParseInput >();
      const std::size_t size = in.size( s + s + offset );

      if( size >= ( offset + s ) ) {
         const char16_t t = Endian::template get< char16_t >( in.current( offset ) );
         if( !is_utf16_surrogate( t ) ) {
            return char32_and_size( t, s );
         }
         if( is_utf16_high_surrogate( t ) ) {
            if( size >= ( offset + s + s ) ) {
               const char16_t u = Endian::template get< char16_t >( in.current( offset + s ) );
               if( is_utf16_low_surrogate( u ) ) {
                  return char32_and_size( utf16_surrogate_compose( t, u ), s + s );
               }
            }
         }
      }
      return char32_and_size();
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
