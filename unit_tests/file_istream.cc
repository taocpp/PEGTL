// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <fstream>

#include "test.hh"

namespace pegtl
{
   struct file_content : seq< pegtl_string_t( "dummy content" ), eol, discard > {};
   struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof > {};

   void unit_test()
   {
      std::ifstream stream( "unit_tests/file_data.txt" );
      TEST_ASSERT( parse_istream< file_grammar >( stream, 16 ) );
   }

} // pegtl

#include "main.hh"
