// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF32_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF32_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_size.hpp"
#include "unicode_utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_utf32_impl
   {
      using data_t = char32_t;
      using pair_t = data_and_size< char32_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return integer_input_size< char32_t, ParseInput >();
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 4 ) ) )
      {
         constexpr std::size_t s = integer_input_size< char32_t, ParseInput >();

         if( in.size( s + offset ) >= ( s + offset ) ) {
            const char32_t t = Endian::template get< char32_t >( in.current( offset ) );
            if( is_utf32_codepoint( t ) ) {
               return pair_t( t, s );
            }
         }
         return pair_t();
      }
   };

   using peek_utf32_be = peek_utf32_impl< big_endian >;
   using peek_utf32_le = peek_utf32_impl< little_endian >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
