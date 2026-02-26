// Copyright (c) 2019-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_INTEGER_HPP
#define TAO_PEGTL_EXAMPLE_INTEGER_HPP

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct unsigned_rule_old
      : plus< digit >
   {
      // Pre-3.0 version of this rule.
   };

   using integer_digits_one = sor< one< '0' >, plus< digit > >;  // Don't consume after first zero.
   using integer_digits_two = if_then_else< one< '0' >, not_at< digit >, plus< digit > >;  // Don't match when superfluous leading zero(s) are present.

   struct unsigned_rule_new
      : integer_digits_two
   {
      // New version that does not allow leading zeros.
   };

   struct signed_rule_old
      : seq< opt< one< '-', '+' > >, plus< digit > >
   {
      // Pre-3.0 version of this rule.
   };

   struct signed_rule_new
      : seq< opt< one< '-', '+' > >, integer_digits_two >
   {
      // New version that does not allow leading zeros.
   };

   struct signed_rule_bis
      : seq< opt< one< '-' > >, integer_digits_two >
   {
      // Only allow minus as sign like std::from_chars.
   };

   struct signed_rule_ter
      : seq< one< '-', '+' >, integer_digits_two >
   {
      // Require sign, for languages where 42 is unsigned and +42 is signed.
   };

   // There are many more possibilities, for example "allow a sign only for non-zero integers".

}  // namespace TAO_PEGTL_NAMESPACE

#endif
