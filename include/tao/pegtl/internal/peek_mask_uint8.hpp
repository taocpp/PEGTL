// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_PEEK_MASK_UINT8_HPP
#define TAO_PEGTL_INTERNAL_PEEK_MASK_UINT8_HPP

#include <cstddef>
#include <cstdint>

#include "../config.hpp"

#include "input_pair.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< std::uint8_t M >
   struct peek_mask_uint8
   {
      using data_t = std::uint8_t;
      using pair_t = input_pair< std::uint8_t >;

      template< typename Input >
      [[nodiscard]] static pair_t peek( Input& in, const std::size_t o = 0 ) noexcept( noexcept( in.peek_uint8( 0 ) ) )
      {
         return { std::uint8_t( in.peek_uint8( o ) & M ), 1 };
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
