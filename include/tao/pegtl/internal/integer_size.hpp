// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INTEGER_SIZE_HPP
#define TAO_PEGTL_INTERNAL_INTEGER_SIZE_HPP

#include <cstddef>
#include <type_traits>

#include "../config.hpp"

#include "dependent_false.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T, typename Data >
   [[nodiscard]] std::size_t integer_size( const Data* /*unused*/ ) noexcept
   {
      static_assert( std::is_integral_v< T > || std::is_enum_v< T > );
      static_assert( std::is_integral_v< Data > || std::is_enum_v< Data > );

      if constexpr( sizeof( T ) == sizeof( Data ) ) {
         return 1;
      }
      else if constexpr( sizeof( Data ) == 1 ) {
         return sizeof( T );
      }
      else {
         static_assert( dependent_false< Data > );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
