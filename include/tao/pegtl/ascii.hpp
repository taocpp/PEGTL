// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ASCII_HPP
#define TAO_PEGTL_ASCII_HPP

#include "config.hpp"

#include "internal/endian.hpp"
#include "internal/peek_ascii7.hpp"
#include "internal/peek_ascii8.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   inline namespace ascii
   {
      // clang-format off
      struct alnum : internal::ranges< internal::peek_ascii8, 'a', 'z', 'A', 'Z', '0', '9' > {};
      struct alpha : internal::ranges< internal::peek_ascii8, 'a', 'z', 'A', 'Z' > {};
      struct any7 : internal::any< internal::peek_ascii7 > {};
      struct any8 : internal::any< internal::peek_ascii8 > {};
      struct blank : internal::one< internal::peek_ascii8, ' ', '\t' > {};
      struct cntrl : internal::ranges< internal::peek_ascii8, static_cast< char >( 0 ), static_cast< char >( 31 ), static_cast< char >( 127 ) > {};
      struct digit : internal::range< internal::peek_ascii8, '0', '9' > {};
      struct esc : internal::one< internal::peek_ascii8, static_cast< char >( 27 ) > {};
      struct ellipsis : internal::ascii_string< internal::identity_endian, '.', '.', '.' > {};
      struct ff : internal::one< internal::peek_ascii8, '\f' > {};
      struct graph : internal::range< internal::peek_ascii8, static_cast< char >( 33 ), static_cast< char >( 126 ) > {};
      struct ht : internal::one< internal::peek_ascii8, '\t' > {};
      struct identifier_first : internal::identifier_first< internal::peek_ascii8 > {};
      struct identifier_other : internal::identifier_other< internal::peek_ascii8 > {};
      struct identifier : internal::identifier< internal::peek_ascii8 > {};
      template< char... Cs > struct ione : internal::ione< internal::peek_ascii8, Cs... > {};
      template< char... Cs > struct istring : internal::ascii_istring< internal::identity_endian, Cs... > {};
      template< char... Cs > struct keyword : internal::seq< internal::ascii_string< internal::identity_endian, Cs... >, internal::not_at< internal::identifier_other< internal::peek_ascii8 > > > { static_assert( sizeof...( Cs ) > 0 ); };
      struct lower : internal::range< internal::peek_ascii8, 'a', 'z' > {};
      template< unsigned Count > struct many7 : internal::many< Count, internal::peek_ascii7 > {};
      template< unsigned Count > struct many8 : internal::many< Count, internal::peek_ascii8 > {};
      template< char... Cs > struct not_ione7 : internal::not_ione< internal::peek_ascii7, Cs... > {};
      template< char... Cs > struct not_ione8 : internal::not_ione< internal::peek_ascii8, Cs... > {};
      template< char... Cs > struct not_one7 : internal::not_one< internal::peek_ascii7, Cs... > {};
      template< char... Cs > struct not_one8 : internal::not_one< internal::peek_ascii8, Cs... > {};
      template< char Lo, char Hi > struct not_range7 : internal::not_range< internal::peek_ascii7, Lo, Hi > {};
      template< char Lo, char Hi > struct not_range8 : internal::not_range< internal::peek_ascii8, Lo, Hi > {};
      struct nul : internal::one< internal::peek_ascii8, static_cast< char >( 0 ) > {};
      struct odigit : internal::range< internal::peek_ascii8, '0', '7' > {};
      template< char... Cs > struct one : internal::one< internal::peek_ascii8, Cs... > {};
      struct print : internal::range< internal::peek_ascii8, static_cast< char >( 32 ), static_cast< char >( 126 ) > {};
      template< char Lo, char Hi > struct range : internal::range< internal::peek_ascii8, Lo, Hi > {};
      template< char... Cs > struct ranges : internal::ranges< internal::peek_ascii8, Cs... > {};
      struct shebang : internal::seq< internal::ascii_string< internal::identity_endian, '#', '!' >, internal::until< internal::eolf< void > > > {};
      struct sp : internal::one< internal::peek_ascii8, ' ' > {};
      struct space : internal::one< internal::peek_ascii8, ' ', '\n', '\r', '\t', '\v', '\f' > {};
      template< char... Cs > struct string : internal::ascii_string< internal::identity_endian, Cs... > {};
      template< char C > struct three : internal::ascii_string< internal::identity_endian, C, C, C > {};
      template< char C > struct two : internal::ascii_string< internal::identity_endian, C, C > {};
      struct upper : internal::range< internal::peek_ascii8, 'A', 'Z' > {};
      struct vt : internal::one< internal::peek_ascii8, '\v' > {};
      struct xdigit : internal::ranges< internal::peek_ascii8, '0', '9', 'a', 'f', 'A', 'F' > {};
      // clang-format on

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#endif
