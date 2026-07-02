// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "test.hpp"

#include <tao/pegtl/extra/unescape_to.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_target
   {
      std::string value;
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

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< grammar >
      : unescape_to< &test_target::value >
   {};

   void unit_test()
   {
      other_state other;
      test_target target;
      text_view_input< scan::lf > in( "a\\n\\u00e4" );

      TAO_PEGTL_TEST_ASSERT( parse< grammar, test_action >( in, other, target ) );
      TAO_PEGTL_TEST_ASSERT( target.value == "a\n\xc3\xa4" );
      TAO_PEGTL_TEST_ASSERT( in.empty() );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
