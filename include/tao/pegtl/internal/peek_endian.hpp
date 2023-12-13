// Copyright (c) 2018-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_ENDIAN_HPP
#define TAO_PEGTL_INTERNAL_PEEK_ENDIAN_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "dependent_false.hpp"
#include "endian.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Endian >
   struct peek_endian
   {
      using data_t = Data;
      using pair_t = data_and_size< Data >;

      static_assert( sizeof( Data ) > 1 );

      static constexpr bool allow_bulk = true;
      static constexpr std::size_t fixed_size = sizeof( Data );

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         using peek_t = typename ParseInput::data_t;

         if constexpr( sizeof( peek_t ) == 1 ) {
            if( in.size( sizeof( Data ) + offset ) < sizeof( Data ) + offset ) {
               return pair_t();
            }
            return pair_t( Endian::template from< Data >( in.current( offset ) ), sizeof( Data ) );
         }
         else if constexpr( sizeof( peek_t ) == sizeof( Data ) ) {
            if( in.size( 1 + offset ) < 1 + offset ) {
               return pair_t();
            }
            return pair_t( Endian::template from< Data >( *in.current( offset ) ), 1 );
         }
         else {
            static_assert( dependent_false< peek_endian > );
         }
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
