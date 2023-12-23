// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_APPLY_OFFSET_HPP
#define TAO_PEGTL_INTERNAL_APPLY_OFFSET_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   struct type_identity
   {
      using type = T;
   };

   template< typename T >
   using type_identity_t = typename type_identity< T >::type;  // TODO: Use C++20 std::type_identity_t.

   template< typename T >
   void apply_offset( type_identity_t< T >& position, const T& offset ) noexcept
   {
      position = offset + position;  // This operator+ is NOT always commutative!
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
