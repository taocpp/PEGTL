// Copyright (c) 2018-2023 Dr. Colin Hirsch and Daniel Frey
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

      static constexpr bool allow_bulk = true;
      static constexpr std::size_t fixed_size = sizeof( data_t );

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( sizeof( data_t ) ) ) )
      {
         const std::size_t s = integer_size< data_t >( in.current() );

         if( in.size( s + offset ) >= ( s + offset ) ) {
            const data_t t = Endian::template get< data_t >( in.current( offset ) );
            return pair_t( Mask & t, s );
         }
         return pair_t();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
