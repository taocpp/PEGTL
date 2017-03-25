// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_EXAMPLES_DOUBLE_HH
#define TAOCPP_PEGTL_INCLUDE_EXAMPLES_DOUBLE_HH

#include <tao/pegtl.hh>

namespace double_
{
   // A grammar for doubles suitable for std::stod without locale support.
   // See also: http://en.cppreference.com/w/cpp/string/basic_string/stof

   struct plus_minus : tao::TAOCPP_PEGTL_NAMESPACE::opt< tao::TAOCPP_PEGTL_NAMESPACE::one< '+', '-' > > {};
   struct dot : tao::TAOCPP_PEGTL_NAMESPACE::one< '.' > {};

   struct inf : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::istring< 'i', 'n', 'f' >,
                                 tao::TAOCPP_PEGTL_NAMESPACE::opt< tao::TAOCPP_PEGTL_NAMESPACE::istring< 'i', 'n', 'i', 't', 'y' > > > {};

   struct nan : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::istring< 'n', 'a', 'n' >,
                                 tao::TAOCPP_PEGTL_NAMESPACE::opt< tao::TAOCPP_PEGTL_NAMESPACE::one< '(' >,
                                                  tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::alnum >,
                                                  tao::TAOCPP_PEGTL_NAMESPACE::one< ')' > > > {};

   template< typename D >
   struct number : tao::TAOCPP_PEGTL_NAMESPACE::if_then_else< dot,
                                             tao::TAOCPP_PEGTL_NAMESPACE::plus< D >,
                                             tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::plus< D >, tao::TAOCPP_PEGTL_NAMESPACE::opt< dot, tao::TAOCPP_PEGTL_NAMESPACE::star< D > > > > {};

   struct e : tao::TAOCPP_PEGTL_NAMESPACE::one< 'e', 'E' > {};
   struct p : tao::TAOCPP_PEGTL_NAMESPACE::one< 'p', 'P' > {};
   struct exponent : tao::TAOCPP_PEGTL_NAMESPACE::seq< plus_minus, tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::digit > > {};

   struct decimal : tao::TAOCPP_PEGTL_NAMESPACE::seq< number< tao::TAOCPP_PEGTL_NAMESPACE::digit >, tao::TAOCPP_PEGTL_NAMESPACE::opt< e, exponent > > {};
   struct binary : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< '0' >, tao::TAOCPP_PEGTL_NAMESPACE::one< 'x', 'X' >, number< tao::TAOCPP_PEGTL_NAMESPACE::xdigit >, tao::TAOCPP_PEGTL_NAMESPACE::opt< p, exponent > > {};

   struct grammar : tao::TAOCPP_PEGTL_NAMESPACE::seq< plus_minus, tao::TAOCPP_PEGTL_NAMESPACE::sor< decimal, binary, inf, nan > > {};

} // double_

#endif
