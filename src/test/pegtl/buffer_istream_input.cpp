// Copyright (c) 2016-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cerrno>
#include <fstream>
#include <string>

#include "test.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct file_content
      : seq< TAO_PEGTL_STRING( "dummy content" ), one< '\n' >, discard >
   {};

   struct file_grammar
      : seq< rep_min_max< 11, 11, file_content >, eof >
   {};

   void unit_test()
   {
#if defined( __cpp_exceptions )
      {
         const char* filename = "src/test/pegtl/no_such_file.txt";
         try {
            std::ifstream stream( filename );
            static_istream_input< void > in( stream );
            parse< file_grammar >( in );
            TAO_PEGTL_TEST_UNREACHABLE;  // LCOV_EXCL_LINE
         }
         catch( const std::system_error& e ) {
            TAO_PEGTL_TEST_ASSERT( e.code().category() == std::system_category() );
            TAO_PEGTL_TEST_ASSERT( e.code().value() == ENOENT );
         }
      }
#endif
      const char* filename = "src/test/pegtl/data/test_data.txt";
      {
         std::ifstream stream( filename );
         static_istream_input< void > in( stream );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         dynamic_istream_input< void > in( 100, 90, stream );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         static_istream_input< void, std::string, std::string > in( "filename", stream );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         dynamic_istream_input< void, const char*, const char* > in( "filename", 100, 90, stream );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
