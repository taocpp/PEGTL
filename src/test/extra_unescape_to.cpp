// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <string>
#include <utility>

#include "test.hpp"

#include <tao/pegtl/extra/builders.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      std::string value;
      char prefix = 0;
      std::string nested;

      void set_nested( const char in_prefix, std::string in_nested )
      {
         prefix = in_prefix;
         nested = std::move( in_nested );
      }
   };

   struct other_state
   {};

   struct escaped
      : sor< c_escaped_char, short_escaped_unicode >
   {};

   struct character
      : if_then_else< one< '\\' >, escaped, utf8::range< 0x20, 0x10FFFF > >
   {};

   struct grammar
      : until< eof, character >
   {};

   struct bang
      : one< '!' >
   {};

   struct nested_grammar
      : seq< bang, grammar >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< grammar >
      : unescape_to< &test_target::value >
   {};

   template<>
   struct test_action< nested_grammar >
      : multi_to< &test_target::set_nested,
                  bang,
                  unescape_for< grammar > >
   {};

   void unit_test()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "a\\n\\u00e4" );

      TAO_PEGTL_TEST_ASSERT( parse< grammar, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.value == "a\n\xc3\xa4" );
      TAO_PEGTL_TEST_ASSERT( in.empty() );

      text_view_input< scan::lf > nested_input( "!b\\t\\u00f6" );

      TAO_PEGTL_TEST_ASSERT( parse< nested_grammar, test_action >( nested_input, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.prefix == '!' );
      TAO_PEGTL_TEST_ASSERT( target.nested == "b\t\xc3\xb6" );
      TAO_PEGTL_TEST_ASSERT( nested_input.empty() );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
