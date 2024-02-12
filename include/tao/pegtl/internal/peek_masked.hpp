// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_MASKED_HPP
#define TAO_PEGTL_INTERNAL_PEEK_MASKED_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, Data Mask, typename Endian = identity_endian >
   struct peek_masked
   {
      using data_t = Data;
      using pair_t = data_and_size< data_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return integer_input_size< data_t, ParseInput >();
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( sizeof( data_t ) ) ) )
      {
         constexpr std::size_t s = integer_input_size< data_t, ParseInput >();

         if( in.size( s + offset ) >= ( s + offset ) ) {
            const data_t t = Endian::template get< data_t >( in.current( offset ) );
            return pair_t( Mask & t, s );
         }
         return pair_t();
      }
   };

   template< std::int8_t Mask >
   using peek_mask_int8 = peek_masked< std::int8_t, Mask >;

   template< std::uint8_t Mask >
   using peek_mask_uint8 = peek_masked< std::uint8_t, Mask >;

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
