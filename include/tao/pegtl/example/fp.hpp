// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_FP_HPP
#define TAO_PEGTL_EXAMPLE_FP_HPP

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

namespace TAO_PEGTL_NAMESPACE::fp
{
   // A grammar for floating point numbers, suitable for std::stod without locale support.
   // See also: http://en.cppreference.com/w/cpp/string/basic_string/stof

   // clang-format off
   struct plus_minus : opt< one< '+', '-' > > {};
   struct dot : one< '.' > {};

   struct inf : seq< istring< 'i', 'n', 'f' >,
                     opt< istring< 'i', 'n', 'i', 't', 'y' > > > {};

   struct nan : seq< istring< 'n', 'a', 'n' >,
                     opt< one< '(' >,
                          plus< alnum >,
                          one< ')' > > > {};

   template< typename D >
   struct numeric : if_then_else< dot,
                                 plus< D >,
                                 seq< plus< D >, opt< dot, star< D > > > > {};

   struct e : one< 'e', 'E' > {};
   struct p : one< 'p', 'P' > {};
   struct exponent : seq< plus_minus, plus< digit > > {};

   struct decimal : seq< numeric< digit >, opt< e, exponent > > {};
   struct hexadecimal : seq< one< '0' >, one< 'x', 'X' >, numeric< xdigit >, opt< p, exponent > > {};

   struct value : seq< plus_minus, sor< hexadecimal, decimal, inf, nan > > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::fp

#endif
