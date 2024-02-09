// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP
#define TAO_PEGTL_INTERNAL_PEEK_UTF16_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "utf16_decode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_utf16_template
   {
      using data_t = char32_t;
      using pair_t = char32_and_size;

      using endian_t = Endian;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 0;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 2 ) ) )
      {
         return utf16_decode_any< Endian >( in, offset );
      }
   };

   using peek_utf16_be = peek_utf16_template< big_endian >;
   using peek_utf16_le = peek_utf16_template< little_endian >;

   using peek_utf16 = TAO_PEGTL_ENDIAN_SUFFIXED( peek_utf16_ );

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
