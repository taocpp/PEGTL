// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_EXAMPLES_DOUBLE_HH
#define TAOCPP_PEGTL_INCLUDE_EXAMPLES_DOUBLE_HH

#include <tao/pegtl.hh>

namespace double_
{
   // A grammar for doubles suitable for std::stod without locale support.
   // See also: http://en.cppreference.com/w/cpp/string/basic_string/stof

   struct plus_minus : tao::pegtl::opt< tao::pegtl::one< '+', '-' > > {};
   struct dot : tao::pegtl::one< '.' > {};

   struct inf : tao::pegtl::seq< tao::pegtl::istring< 'i', 'n', 'f' >,
                            tao::pegtl::opt< tao::pegtl::istring< 'i', 'n', 'i', 't', 'y' > > > {};

   struct nan : tao::pegtl::seq< tao::pegtl::istring< 'n', 'a', 'n' >,
                            tao::pegtl::opt< tao::pegtl::one< '(' >,
                                        tao::pegtl::plus< tao::pegtl::alnum >,
                                        tao::pegtl::one< ')' > > > {};

   template< typename D >
   struct number : tao::pegtl::if_then_else< dot,
                                        tao::pegtl::plus< D >,
                                        tao::pegtl::seq< tao::pegtl::plus< D >, tao::pegtl::opt< dot, tao::pegtl::star< D > > > > {};

   struct e : tao::pegtl::one< 'e', 'E' > {};
   struct p : tao::pegtl::one< 'p', 'P' > {};
   struct exponent : tao::pegtl::seq< plus_minus, tao::pegtl::plus< tao::pegtl::digit > > {};

   struct decimal : tao::pegtl::seq< number< tao::pegtl::digit >, tao::pegtl::opt< e, exponent > > {};
   struct binary : tao::pegtl::seq< tao::pegtl::one< '0' >, tao::pegtl::one< 'x', 'X' >, number< tao::pegtl::xdigit >, tao::pegtl::opt< p, exponent > > {};

   struct grammar : tao::pegtl::seq< plus_minus, tao::pegtl::sor< decimal, binary, inf, nan > > {};

} // double_

#endif
