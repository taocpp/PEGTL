// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_DIRECT_HPP
#define TAO_PEGTL_INTERNAL_PEEK_DIRECT_HPP

#include <cstddef>
#include <cstdint>

#include "data_and_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data >
   struct peek_direct
   {
      using data_t = Data;
      using pair_t = data_and_size< Data, std::uint8_t >;

      static constexpr bool allow_bulk = true;
      static constexpr std::size_t fixed_size = sizeof( Data );

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 42 ) ) )
      {
         using peek_t = typename ParseInput::data_t;

         static_assert( sizeof( peek_t ) == sizeof( data_t ) );

         if( in.size( offset + 1 ) > offset ) {
            return pair_t( Data( *in.current( offset ) ), 1 );
         }
         return pair_t();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif