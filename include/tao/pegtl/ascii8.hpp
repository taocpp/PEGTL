// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ASCII8_HPP
#define TAO_PEGTL_ASCII8_HPP

#include "config.hpp"

#include "internal/peek_ascii.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace ascii8
   {
      // clang-format off
      struct alnum : internal::ranges< internal::peek_ascii8, 'a', 'z', 'A', 'Z', '0', '9' > {};
      struct alpha : internal::ranges< internal::peek_ascii8, 'a', 'z', 'A', 'Z' > {};
      struct any : internal::any< internal::peek_ascii8 > {};
      struct blank : internal::one< internal::peek_ascii8, ' ', '\t' > {};
      struct byte : internal::any< internal::peek_ascii8 > {};
      struct cntrl : internal::ranges< internal::peek_ascii8, static_cast< char >( 0 ), static_cast< char >( 31 ), static_cast< char >( 127 ) > {};
      struct digit : internal::range< internal::peek_ascii8, '0', '9' > {};
      struct esc : internal::one< internal::peek_ascii8, static_cast< char >( 27 ) > {};
      struct ellipsis : internal::seq_one< internal::peek_ascii8, '.', '.', '.' > {};
      struct ff : internal::one< internal::peek_ascii8, '\f' > {};
      template< char... Cs > struct forty_two : internal::rep< 42, internal::one< internal::peek_ascii8, Cs... > > {};
      struct graph : internal::range< internal::peek_ascii8, static_cast< char >( 33 ), static_cast< char >( 126 ) > {};
      struct ht : internal::one< internal::peek_ascii8, '\t' > {};
      struct identifier_first : internal::identifier_first< internal::peek_ascii8 > {};
      struct identifier_other : internal::identifier_other< internal::peek_ascii8 > {};
      struct identifier : internal::identifier< internal::peek_ascii8 > {};
      template< char... Cs > struct ione : internal::ione< internal::peek_ascii8, Cs... > {};
      template< char... Cs > struct istring : internal::seq_ione< internal::peek_ascii8, Cs... > {};
      template< char... Cs > struct keyword : internal::seq< internal::seq_one< internal::peek_ascii8, Cs... >, internal::not_at< internal::identifier_other< internal::peek_ascii8 > > > { static_assert( sizeof...( Cs ) > 0 ); };
      struct lower : internal::range< internal::peek_ascii8, 'a', 'z' > {};
      template< unsigned Count > struct many : internal::many< Count, internal::peek_ascii8 > {};
      template< char... Cs > struct not_one : internal::not_one< internal::peek_ascii8, Cs... > {};
      template< char Lo, char Hi > struct not_range : internal::not_range< internal::peek_ascii8, Lo, Hi > {};
      struct nul : internal::one< internal::peek_ascii8, static_cast< char >( 0 ) > {};
      struct odigit : internal::range< internal::peek_ascii8, '0', '7' > {};
      template< char... Cs > struct one : internal::one< internal::peek_ascii8, Cs... > {};
      struct print : internal::range< internal::peek_ascii8, static_cast< char >( 32 ), static_cast< char >( 126 ) > {};
      template< char Lo, char Hi > struct range : internal::range< internal::peek_ascii8, Lo, Hi > {};
      template< char... Cs > struct ranges : internal::ranges< internal::peek_ascii8, Cs... > {};
      struct shebang : internal::seq< internal::seq_one< internal::peek_ascii8, '#', '!' >, internal::until< internal::eolf< void > > > {};
      struct sp : internal::one< internal::peek_ascii8, ' ' > {};
      struct space : internal::one< internal::peek_ascii8, ' ', '\n', '\r', '\t', '\v', '\f' > {};
      template< char... Cs > struct string : internal::seq_one< internal::peek_ascii8, Cs... > {};
      template< char C > struct three : internal::seq_one< internal::peek_ascii8, C, C, C > {};
      template< char C > struct two : internal::seq_one< internal::peek_ascii8, C, C > {};
      struct upper : internal::range< internal::peek_ascii8, 'A', 'Z' > {};
      struct vt : internal::one< internal::peek_ascii8, '\v' > {};
      struct xdigit : internal::ranges< internal::peek_ascii8, '0', '9', 'a', 'f', 'A', 'F' > {};
      // clang-format on

   }  // namespace ascii8

}  // namespace TAO_PEGTL_NAMESPACE

#endif
