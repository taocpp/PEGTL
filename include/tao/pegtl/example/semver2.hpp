// Copyright (c) 2020-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_SEMVER2_HPP
#define TAO_PEGTL_EXAMPLE_SEMVER2_HPP

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

namespace TAO_PEGTL_NAMESPACE::semver2
{
   // Based on "Backus-Naur Form for Valid SemVer Versions"
   // on https://semver.org for Semantic Versioning 2.0.0.
   // The language of all valid semantic versions is regular.

   // clang-format off
   struct dot : one< '.' > {};
   struct plus_ : one< '+' > {};
   struct minus : one< '-' > {};
   struct zero : one< '0' > {};

   struct digits : plus< digit >{};

   struct positive_digit : range< '1', '9' > {};
   struct non_digit : ranges< 'a', 'z', 'A', 'Z', '-' > {};

   struct identifier_character : ranges< 'a', 'z', 'A', 'Z', '0', '9', '-' > {};
   struct identifier_characters : plus< identifier_character > {};

   struct numeric_identifier : sor< zero, seq< positive_digit, star< digit > > > {};
   struct alphanumeric_identifier : seq< star< digit >, non_digit, star< identifier_character > > {};

   struct build_identifier : identifier_characters {};

   struct pre_release_identifier : sor< alphanumeric_identifier, numeric_identifier > {};

   struct build : list< build_identifier, dot > {};
   struct pre_release : list< pre_release_identifier, dot > {};

   struct major : numeric_identifier {};
   struct minor : numeric_identifier {};
   struct patch : numeric_identifier {};

   struct version_core : seq< major, dot, minor, dot, patch > {};

   struct valid_semver : seq< version_core, opt< minus, pre_release >, opt< plus_, build > > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::semver2

#endif
