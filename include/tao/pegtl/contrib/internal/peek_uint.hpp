// Copyright (c) 2018-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_PEEK_UINT_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_PEEK_UINT_HPP

#include <cstddef>
#include <cstdint>

#include "../../internal/data_and_size.hpp"

#include "read_uint.hpp"

namespace tao::pegtl::internal
{
   template< typename R >
   struct peek_uint_impl
   {
      using data_t = typename R::type;
      using pair_t = data_and_size< data_t >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in ) noexcept( noexcept( in.size( sizeof( data_t ) ) ) )
      {
         if( in.size( sizeof( data_t ) ) < sizeof( data_t ) ) {
            return { 0, 0 };
         }
         const data_t data = R::read( in.current() );
         return { data, sizeof( data_t ) };
      }
   };

   using peek_uint16_be = peek_uint_impl< read_uint16_be >;
   using peek_uint16_le = peek_uint_impl< read_uint16_le >;

   using peek_uint32_be = peek_uint_impl< read_uint32_be >;
   using peek_uint32_le = peek_uint_impl< read_uint32_le >;

   using peek_uint64_be = peek_uint_impl< read_uint64_be >;
   using peek_uint64_le = peek_uint_impl< read_uint64_le >;

}  // namespace tao::pegtl::internal

#endif
