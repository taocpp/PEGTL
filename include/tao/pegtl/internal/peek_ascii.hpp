// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_ASCII_HPP
#define TAO_PEGTL_INTERNAL_PEEK_ASCII_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "data_and_size.hpp"
#include "endian.hpp"
#include "integer_traits.hpp"
#include "utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian >
   struct peek_ascii_impl
   {
      using data_t = char;
      using pair_t = data_and_size< data_t >;

      static constexpr bool allow_bulk = false;

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

   using peek_ascii = peek_ascii_impl< identity_endian >;
   using peek_ascii_be = peek_ascii_impl< big_endian >;
   using peek_ascii_le = peek_ascii_impl< little_endian >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
