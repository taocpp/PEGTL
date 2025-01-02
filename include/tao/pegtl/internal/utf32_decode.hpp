// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_UTF32_DECODE_HPP
#define TAO_PEGTL_INTERNAL_UTF32_DECODE_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "integer_size.hpp"
#include "utf32_details.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian, typename ParseInput >
   [[nodiscard]] char32_and_size utf32_decode_any( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 2 ) ) )
   {
      constexpr std::size_t s = integer_input_size< char32_t, ParseInput >();

      if( in.size( s + offset ) >= ( s + offset ) ) {
         const char32_t t = Endian::template get< char32_t >( in.current( offset ) );

         if( is_utf32( t ) ) {
            return { t, s };
         }
      }
      return {};
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
