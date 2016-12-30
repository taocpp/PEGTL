// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <cstdio>
#include <clocale>

#include "test.hh"

namespace pegtl
{
   struct file_content : seq< pegtl_string_t( "dummy content" ), eol, discard > {};
   struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof > {};

   void unit_test()
   {
      const char * const filename = "unit_tests/file_data.txt";
      std::FILE * stream = std::fopen( filename, "rb" );
      TEST_ASSERT( stream != nullptr );
      TEST_ASSERT( parse_cstream< file_grammar >( stream, filename, 16 ) );
      std::fclose( stream );
   }

} // namespace pegtl

#include "main.hh"
