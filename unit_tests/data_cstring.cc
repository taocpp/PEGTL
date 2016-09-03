// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   struct test_grammar : seq< string< 'a', 'b' >, discard, string< 'c', 'd' >, discard, string< 'e', 'f' >, discard, eof > {};

   void unit_test()
   {
      const char * test_data = "abcdef";
      TEST_ASSERT( parse_cstring< test_grammar >( test_data, "test data", 2 ) );
      TEST_ASSERT( ! parse_cstring< test_grammar >( test_data, "test data", 1 ) );
   }

} // namespace pegtl

#include "main.hh"
