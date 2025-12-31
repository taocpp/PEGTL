// Copyright (c) 2018-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BINARY_INTERNAL_PEEK_ALIASES_HPP
#define TAO_PEGTL_BINARY_INTERNAL_PEEK_ALIASES_HPP

#include <cstdint>

#include "../../config.hpp"

#include "endian.hpp"
#include "peek_endian.hpp"
#include "peek_masked.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   using peek_int16_be = peek_endian< std::int16_t, big_endian >;
   using peek_int32_be = peek_endian< std::int32_t, big_endian >;
   using peek_int64_be = peek_endian< std::int64_t, big_endian >;

   using peek_int16_le = peek_endian< std::int16_t, little_endian >;
   using peek_int32_le = peek_endian< std::int32_t, little_endian >;
   using peek_int64_le = peek_endian< std::int64_t, little_endian >;

   using peek_uint16_be = peek_endian< std::uint16_t, big_endian >;
   using peek_uint32_be = peek_endian< std::uint32_t, big_endian >;
   using peek_uint64_be = peek_endian< std::uint64_t, big_endian >;

   using peek_uint16_le = peek_endian< std::uint16_t, little_endian >;
   using peek_uint32_le = peek_endian< std::uint32_t, little_endian >;
   using peek_uint64_le = peek_endian< std::uint64_t, little_endian >;

   template< std::int8_t Mask >
   using peek_mask_int8 = peek_masked< std::int8_t, Mask, identity_endian >;

   template< std::uint8_t Mask >
   using peek_mask_uint8 = peek_masked< std::uint8_t, Mask, identity_endian >;

   template< std::int16_t Mask >
   using peek_mask_int16_be = peek_masked< std::int16_t, Mask, big_endian >;
   template< std::int32_t Mask >
   using peek_mask_int32_be = peek_masked< std::int32_t, Mask, big_endian >;
   template< std::int64_t Mask >
   using peek_mask_int64_be = peek_masked< std::int64_t, Mask, big_endian >;

   template< std::int16_t Mask >
   using peek_mask_int16_le = peek_masked< std::int16_t, Mask, little_endian >;
   template< std::int32_t Mask >
   using peek_mask_int32_le = peek_masked< std::int32_t, Mask, little_endian >;
   template< std::int64_t Mask >
   using peek_mask_int64_le = peek_masked< std::int64_t, Mask, little_endian >;

   template< std::uint16_t Mask >
   using peek_mask_uint16_be = peek_masked< std::uint16_t, Mask, big_endian >;
   template< std::uint32_t Mask >
   using peek_mask_uint32_be = peek_masked< std::uint32_t, Mask, big_endian >;
   template< std::uint64_t Mask >
   using peek_mask_uint64_be = peek_masked< std::uint64_t, Mask, big_endian >;

   template< std::uint16_t Mask >
   using peek_mask_uint16_le = peek_masked< std::uint16_t, Mask, little_endian >;
   template< std::uint32_t Mask >
   using peek_mask_uint32_le = peek_masked< std::uint32_t, Mask, little_endian >;
   template< std::uint64_t Mask >
   using peek_mask_uint64_le = peek_masked< std::uint64_t, Mask, little_endian >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
