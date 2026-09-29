// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include <tao/pegtl.hpp>

#include <tao/pegtl/action/change_action_and_state.hpp>
#include <tao/pegtl/extra/charconv.hpp>
#include <tao/pegtl/extra/unescape.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace
{
   struct escaped : pegtl::seq< pegtl::one< '\\' >, pegtl::one< 'n' > > {};

   template< typename Rule >
   struct unescape_action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct unescape_action< escaped >
      : pegtl::change_action_and_state< pegtl::unescape, std::string >
   {};

}  // namespace

int main()
{
   int value = 0;  // NOLINT(misc-const-correctness) -- Mutated by from_chars_nothrow.
   pegtl::view_input number( "42" );
   const bool converted = pegtl::parse< pegtl::from_chars_nothrow< int > >( number, value );

   std::string result;  // NOLINT(misc-const-correctness) -- Mutated by the unescape action.
   pegtl::text_view_input<> text( "\\n" );
   const bool unescaped = pegtl::parse< escaped, unescape_action >( text, result );
   return ( converted && unescaped && ( value == 42 ) ) ? 0 : 1;
}
