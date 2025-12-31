// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <cerrno>
#include <fstream>
#include <iterator>
#include <string>

#include "test.hpp"

#include <tao/pegtl/stream.hpp>

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
      const char* filename = "src/test/data/test_data.txt";
      const std::istreambuf_iterator< char > end;
      {
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         array_iterator_input< decltype( begin ) > in( begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         alloc_iterator_input< decltype( begin ) > in( 100, 90, begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::array< char, 100 > buffer;
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         other_iterator_input< decltype( begin ) > in( buffer.data(), 100, 90, begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         array_iterator_input< decltype( begin ), std::string, std::string > in( "filename", begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         alloc_iterator_input< decltype( begin ), const char*, const char* > in( "filename", 100, 90, begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
      {
         std::array< char, 100 > buffer;
         std::ifstream stream( filename );
         std::istreambuf_iterator< char > begin( stream );
         other_iterator_input< decltype( begin ), const char*, const char* > in( "filename", buffer.data(), 100, 90, begin, end );
         TAO_PEGTL_TEST_ASSERT( parse< file_grammar >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
