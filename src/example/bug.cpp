// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// Reproduces with GCC 16 when the local -Wmaybe-uninitialized suppression in
// action/internal/builder_utility.hpp is disabled:
// g++-16 -std=c++20 -Iinclude -pedantic -Wall -Wextra -Wshadow -Werror -O3 src/example/bug.cpp

#include <cstdlib>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using value_t = std::variant< int, std::string, bool >;

   struct target
   {
      value_t value;
      std::vector< value_t > values;
   };

   struct number
      : plus< digit >
   {};

   struct word
      : plus< alpha >
   {};

   struct yes
      : string< 'y', 'e', 's' >
   {};

   struct no
      : string< 'n', 'o' >
   {};

   struct value
      : sor< yes, no, number, word >
   {};

   struct values
      : list< value, one< ',' > >
   {};

   template< typename Rule >
   struct bug_action
      : nothing< Rule >
   {};

   template<>
   struct bug_action< value >
      : variant_to< &target::value,
                    number,
                    word,
                    cases< case_< yes, true >,
                           case_< no, false > > >
   {};

   template<>
   struct bug_action< values >
      : repeat_to< &target::values,
                   variant_for< value,
                                number,
                                word,
                                cases< case_< yes, true >,
                                       case_< no, false > > > >
   {};
}  // namespace TAO_PEGTL_NAMESPACE

template< typename Rule >
void parse_into( const std::string_view text, tao::pegtl::target& result )
{
   tao::pegtl::text_view_input< tao::pegtl::scan::lf > in( text );
   if( !tao::pegtl::parse< Rule, tao::pegtl::bug_action >( in, result ) || !in.empty() ) {
      std::abort();
   }
}

int main()
{
   tao::pegtl::target word;
   parse_into< tao::pegtl::value >( "alpha", word );

   tao::pegtl::target repeated;
   parse_into< tao::pegtl::values >( "yes,42,beta,no", repeated );

   return word.value == tao::pegtl::value_t( std::string( "alpha" ) ) && repeated.values.size() == 4 ? 0 : 1;
}
