// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_STRING_HPP
#define TAO_PEGTL_INTERNAL_ASCII_STRING_HPP

#include "../config.hpp"

#include "ascii_multiple.hpp"
#include "enable_control.hpp"
#include "one.hpp"
#include "peek_ascii8.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian, char... Cs >
   struct ascii_string
      : ascii_multiple< ascii_string< Endian, Cs... > >
   {
      template< char C, typename D >
      [[nodiscard]] static constexpr bool test( const D d ) noexcept
      {
         return ascii_char_equal< C >( d );
      }
   };

   template< typename Endian, char C >
   struct ascii_string< Endian, C >
      : one< peek_ascii8_impl< Endian >, C >
   {};

   template< typename Endian >
   struct ascii_string< Endian >
      : success
   {};

   template< typename Endian, char... Cs >
   inline constexpr bool enable_control< ascii_string< Endian, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
