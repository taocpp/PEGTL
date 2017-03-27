// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <fstream>

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct file_content : seq< TAOCPP_PEGTL_STRING( "dummy content" ), eol, discard >
      {
      };
      struct file_grammar : seq< rep_min_max< 11, 11, file_content >, eof >
      {
      };

      void unit_test()
      {
         try {
            const std::string filename = "src/test/pegtl/no_such_file.txt";
            std::ifstream stream( filename );
            parse_istream< file_grammar >( stream, filename, 16 );
            TAOCPP_PEGTL_TEST_ASSERT( false );
         }
         catch( const input_error& e ) {
            TAOCPP_PEGTL_TEST_ASSERT( std::string( e.what() ).find( "error in istream.read()" ) != std::string::npos );
         }
         const std::string filename = "src/test/pegtl/file_data.txt";
         std::ifstream stream( filename );
         TAOCPP_PEGTL_TEST_ASSERT( parse_istream< file_grammar >( stream, filename, 16 ) );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
