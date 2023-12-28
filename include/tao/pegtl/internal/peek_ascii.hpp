// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_ASCII_HPP
#define TAO_PEGTL_INTERNAL_PEEK_ASCII_HPP

#include <cstddef>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_traits.hpp"
#include "utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_ascii7_impl
   {
      using data_t = char;
      using pair_t = data_and_size< data_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return false;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using temp_t = typename integer_traits< sizeof( *in.current() ) >::unsigned_t;

         if( in.size( 1 + offset ) >= ( 1 + offset ) ) {
            const temp_t t = Endian::template get< temp_t >( in.current( offset ) );
            if( is_ascii_value( t ) ) {
               return pair_t( char( t ), 1 );
            }
         }
         return pair_t();
      }
   };

   using peek_ascii7 = peek_ascii7_impl< identity_endian >;
   using peek_ascii7_be = peek_ascii7_impl< big_endian >;
   using peek_ascii7_le = peek_ascii7_impl< little_endian >;

   template< typename Endian >
   struct peek_ascii8_impl
   {
      using data_t = char;
      using pair_t = data_and_size< data_t >;

      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return sizeof( typename ParseInput::data_t ) == 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 1 ) ) )
      {
         using temp_t = typename integer_traits< sizeof( *in.current() ) >::unsigned_t;

         if( in.size( 1 + offset ) >= ( 1 + offset ) ) {
            const temp_t t = Endian::template get< temp_t >( in.current( offset ) );
            if constexpr( sizeof( temp_t ) > 1 ) {
               if( t > 255 ) {
                  return pair_t();
               }
            }
            return pair_t( char( t ), 1 );
         }
         return pair_t();
      }
   };

   using peek_ascii8 = peek_ascii8_impl< identity_endian >;
   using peek_ascii8_be = peek_ascii8_impl< big_endian >;
   using peek_ascii8_le = peek_ascii8_impl< little_endian >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
