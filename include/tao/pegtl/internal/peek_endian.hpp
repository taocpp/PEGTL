// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_ENDIAN_HPP
#define TAO_PEGTL_INTERNAL_PEEK_ENDIAN_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Endian >
   struct peek_endian
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
            return pair_t( t, s );
         }
         return pair_t();
      }
   };

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

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
