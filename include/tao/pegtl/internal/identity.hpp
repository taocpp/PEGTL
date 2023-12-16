// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_IDENTITY_HPP
#define TAO_PEGTL_INTERNAL_IDENTITY_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   [[nodiscard]] T identity( const T n ) noexcept
   {
      static_assert( std::is_integral_v< T > || std::is_enum_v< T > );
      // ...or will anybody ever parse sequences of floats or doubles?
      return n;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif