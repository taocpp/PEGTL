// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_ALPHABET_RULES_HPP
#define TAO_PEGTL_CONTRIB_ALPHABET_RULES_HPP

#include "../ascii.hpp"
#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::alphabet::rules
{
   struct a : ascii::one< 'a' > {};
   struct b : ascii::one< 'b' > {};
   struct c : ascii::one< 'c' > {};
   struct d : ascii::one< 'd' > {};
   struct e : ascii::one< 'e' > {};
   struct f : ascii::one< 'f' > {};
   struct g : ascii::one< 'g' > {};
   struct h : ascii::one< 'h' > {};
   struct i : ascii::one< 'i' > {};
   struct j : ascii::one< 'j' > {};
   struct k : ascii::one< 'k' > {};
   struct l : ascii::one< 'l' > {};
   struct m : ascii::one< 'm' > {};
   struct n : ascii::one< 'n' > {};
   struct o : ascii::one< 'o' > {};
   struct p : ascii::one< 'p' > {};
   struct q : ascii::one< 'q' > {};
   struct r : ascii::one< 'r' > {};
   struct s : ascii::one< 's' > {};
   struct t : ascii::one< 't' > {};
   struct u : ascii::one< 'u' > {};
   struct v : ascii::one< 'v' > {};
   struct w : ascii::one< 'w' > {};
   struct x : ascii::one< 'x' > {};
   struct y : ascii::one< 'y' > {};
   struct z : ascii::one< 'z' > {};

   struct A : ascii::one< 'A' > {};  // NOLINT(readability-identifier-naming)
   struct B : ascii::one< 'B' > {};  // NOLINT(readability-identifier-naming)
   struct C : ascii::one< 'C' > {};  // NOLINT(readability-identifier-naming)
   struct D : ascii::one< 'D' > {};  // NOLINT(readability-identifier-naming)
   struct E : ascii::one< 'E' > {};  // NOLINT(readability-identifier-naming)
   struct F : ascii::one< 'F' > {};  // NOLINT(readability-identifier-naming)
   struct G : ascii::one< 'G' > {};  // NOLINT(readability-identifier-naming)
   struct H : ascii::one< 'H' > {};  // NOLINT(readability-identifier-naming)
   struct I : ascii::one< 'I' > {};  // NOLINT(readability-identifier-naming,misc-confusable-identifiers)
   struct J : ascii::one< 'J' > {};  // NOLINT(readability-identifier-naming)
   struct K : ascii::one< 'K' > {};  // NOLINT(readability-identifier-naming)
   struct L : ascii::one< 'L' > {};  // NOLINT(readability-identifier-naming)
   struct M : ascii::one< 'M' > {};  // NOLINT(readability-identifier-naming)
   struct N : ascii::one< 'N' > {};  // NOLINT(readability-identifier-naming)
   struct O : ascii::one< 'O' > {};  // NOLINT(readability-identifier-naming)
   struct P : ascii::one< 'P' > {};  // NOLINT(readability-identifier-naming)
   struct Q : ascii::one< 'Q' > {};  // NOLINT(readability-identifier-naming)
   struct R : ascii::one< 'R' > {};  // NOLINT(readability-identifier-naming)
   struct S : ascii::one< 'S' > {};  // NOLINT(readability-identifier-naming)
   struct T : ascii::one< 'T' > {};  // NOLINT(readability-identifier-naming)
   struct U : ascii::one< 'U' > {};  // NOLINT(readability-identifier-naming)
   struct V : ascii::one< 'V' > {};  // NOLINT(readability-identifier-naming)
   struct W : ascii::one< 'W' > {};  // NOLINT(readability-identifier-naming)
   struct X : ascii::one< 'X' > {};  // NOLINT(readability-identifier-naming)
   struct Y : ascii::one< 'Y' > {};  // NOLINT(readability-identifier-naming)
   struct Z : ascii::one< 'Z' > {};  // NOLINT(readability-identifier-naming)

}  // namespace TAO_PEGTL_NAMESPACE::alphabet::rules

#endif  // FCOV_EXCL_FILE
