// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UNICODE_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UNICODE_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "dependent_false.hpp"
#include "endian.hpp"
#include "peek_utf16.hpp"
#include "peek_utf32.hpp"
#include "peek_utf8.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_unicode_template
   {
      using data_t = char32_t;
      using pair_t = char32_and_size;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 4 ) ) )
      {
         static constexpr std::size_t size = sizeof( typename ParseInput::data_t );

         if constexpr( size == 1 ) {
            return peek_utf8::peek( in, offset );
         }
         else if constexpr( size == 2 ) {
            return peek_utf16_template< Endian >::peek( in, offset );
         }
         else if constexpr( size == 4 ) {
            return peek_utf32_template< Endian >::peek( in, offset );
         }
         else {
            static_assert( dependent_false< ParseInput > );
         }
      }
   };

   using peek_unicode_be = peek_unicode_template< big_endian >;
   using peek_unicode_le = peek_unicode_template< little_endian >;

   using peek_unicode = TAO_PEGTL_ENDIAN_SUFFIXED( peek_unicode_ );

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
