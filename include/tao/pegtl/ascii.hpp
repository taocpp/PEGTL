// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ASCII_HPP
#define TAO_PEGTL_ASCII_HPP

#include "config.hpp"

#include "internal/result_on_found.hpp"
#include "internal/rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   inline namespace ascii
   {
      // clang-format off
      struct alnum : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > {};
      struct alpha : internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > {};
      struct any : internal::any< internal::peek_char > {};
      struct blank : internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\t' > {};
      struct cntrl : internal::ranges< internal::peek_char, static_cast< char >( 0 ), static_cast< char >( 31 ), static_cast< char >( 127 ) > {};
      struct cr : internal::one< internal::result_on_found::success, internal::peek_char, '\r' > {};
      struct digit : internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > {};
      struct esc : internal::one< internal::result_on_found::success, internal::peek_char, static_cast< char >( 27 ) > {};
      struct ellipsis : internal::string< '.', '.', '.' > {};
      struct ff : internal::one< internal::result_on_found::success, internal::peek_char, '\f' > {};
      template< char... Cs > struct forty_two : internal::rep< 42, internal::one< internal::result_on_found::success, internal::peek_char, Cs... > > {};
      struct graph : internal::range< internal::result_on_found::success, internal::peek_char, static_cast< char >( 33 ), static_cast< char >( 126 ) > {};
      struct ht : internal::one< internal::result_on_found::success, internal::peek_char, '\t' > {};
      struct identifier_first : internal::identifier_first {};
      struct identifier_other : internal::identifier_other {};
      struct identifier : internal::identifier {};
      template< char... Cs > struct istring : internal::istring< Cs... > {};
      template< char... Cs > struct keyword : internal::seq< internal::string< Cs... >, internal::not_at< internal::identifier_other > > { static_assert( sizeof...( Cs ) > 0 ); };
      struct lf : internal::one< internal::result_on_found::success, internal::peek_char, '\n' > {};
      struct lower : internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > {};
      template< char... Cs > struct not_one : internal::one< internal::result_on_found::failure, internal::peek_char, Cs... > {};
      template< char Lo, char Hi > struct not_range : internal::range< internal::result_on_found::failure, internal::peek_char, Lo, Hi > {};
      struct nul : internal::one< internal::result_on_found::success, internal::peek_char, static_cast< char >( 0 ) > {};
      struct odigit : internal::range< internal::result_on_found::success, internal::peek_char, '0', '7' > {};
      template< char... Cs > struct one : internal::one< internal::result_on_found::success, internal::peek_char, Cs... > {};
      struct print : internal::range< internal::result_on_found::success, internal::peek_char, static_cast< char >( 32 ), static_cast< char >( 126 ) > {};
      template< char Lo, char Hi > struct range : internal::range< internal::result_on_found::success, internal::peek_char, Lo, Hi > {};
      template< char... Cs > struct ranges : internal::ranges< internal::peek_char, Cs... > {};
      struct seven : internal::range< internal::result_on_found::success, internal::peek_char, static_cast< char >( 0 ), static_cast< char >( 127 ) > {};
      struct shebang : internal::seq< internal::string< '#', '!' >, internal::until< internal::eolf > > {};
      struct sp : internal::one< internal::result_on_found::success, internal::peek_char, ' ' > {};
      struct space : internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\n', '\r', '\t', '\v', '\f' > {};
      template< char... Cs > struct string : internal::string< Cs... > {};
      template< char C > struct three : internal::string< C, C, C > {};
      template< char C > struct two : internal::string< C, C > {};
      struct upper : internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > {};
      struct vt : internal::one< internal::result_on_found::success, internal::peek_char, '\v' > {};
      struct xdigit : internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > {};
      // clang-format on

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#include "internal/pegtl_string.hpp"

#endif
