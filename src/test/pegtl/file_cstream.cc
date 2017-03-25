// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cstdio>
#include <clocale>

#include "test.hh"

namespace tao
{
   namespace pegtl
   {
      struct file_content : seq< TAOCPP_PEGTL_STRING( "dummy content" ), eol, discard > {};
      struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof > {};

      void unit_test()
      {
         const char * const filename = "src/test/pegtl/file_data.txt";
#if defined( _WIN32 )
         std::FILE * stream;
         ::fopen_s( & stream, filename, "rb" );
#else
         std::FILE * stream = std::fopen( filename, "rb" );
#endif
         TEST_ASSERT( stream != nullptr );
         TEST_ASSERT( parse_cstream< file_grammar >( stream, filename, 16 ) );
         std::fclose( stream );
      }

   } // namespace pegtl

} // namespace tao

#include "main.hh"
