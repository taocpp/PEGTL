// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_ASCII_ISTRING_HPP
#define TAO_PEGTL_INTERNAL_ASCII_ISTRING_HPP

#include <type_traits>

#include "../config.hpp"
#include "../type_list.hpp"

#include "ascii_multiple.hpp"
#include "ascii_utility.hpp"
#include "enable_control.hpp"
#include "ione.hpp"
#include "peek_ascii8.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Endian, char... Cs >
   struct ascii_istring
      : ascii_multiple< ascii_istring< Endian, Cs... > >
   {
      using rule_t = ascii_istring;
      using subs_t = empty_list;

      template< char C, typename D >
      [[nodiscard]] static constexpr bool test( const D d ) noexcept
      {
         return ascii_ichar_equal< C >( d );
      }
   };

   template< typename Endian, char C >
   struct ascii_istring< Endian, C >
      : ione< peek_ascii8_impl< Endian >, C >
   {};

   template< typename Endian >
   struct ascii_istring< Endian >
      : success
   {};

   template< typename Endian, char... Cs >
   inline constexpr bool enable_control< ascii_istring< Endian, Cs... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
