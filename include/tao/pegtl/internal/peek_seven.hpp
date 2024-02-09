// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_SEVEN_HPP
#define TAO_PEGTL_INTERNAL_PEEK_SEVEN_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "ascii_utility.hpp"
#include "data_and_size.hpp"
#include "integer_traits.hpp"
#include "type_traits.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct peek_seven
   {
      using data_t = char;
      using pair_t = char_and_size;

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
         using raw_t = typename ParseInput::data_t;

         static_assert( sizeof( data_t ) == sizeof( raw_t ) );
         static_assert( is_simple_type_v< raw_t > );

         if( in.size( 1 + offset ) >= ( 1 + offset ) ) {
            const data_t c = static_cast< data_t >( *in.current( offset ) );
            if( is_seven_value( c ) ) {
               return pair_t( c, 1 );
            }
         }
         return pair_t();
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
