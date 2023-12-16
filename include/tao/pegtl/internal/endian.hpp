// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ENDIAN_HPP
#define TAO_PEGTL_INTERNAL_ENDIAN_HPP

#include "../config.hpp"

#include "byteswap.hpp"
#include "endian_system.hpp"
#include "identity.hpp"
#include "integer_adapt.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   struct identity_endian
   {
      template< typename T >
      [[nodiscard]] static T get( const T data ) noexcept
      {
         return identity( data );
      }

      template< typename T, typename Data >
      [[nodiscard]] static T get( const Data* pointer ) noexcept
      {
         return identity( integer_adapt< T >( pointer ) );
      }
   };

   struct byteswap_endian
   {
      template< typename T >
      [[nodiscard]] static T get( const T data ) noexcept
      {
         return byteswap( data );
      }

      template< typename T, typename Data >
      [[nodiscard]] static T get( const Data* pointer ) noexcept
      {
         return byteswap( integer_adapt< T >( pointer ) );
      }
   };

   using big_endian = TAO_PEGTL_BIG_ENDIAN_TYPE;
   using little_endian = TAO_PEGTL_LITTLE_ENDIAN_TYPE;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
