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

#include <sstream>
#include <string>
#include <type_traits>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Input >
   void test_count_input( Input& in )
   {
      static_assert( std::is_same_v< typename Input::error_position_t, position_with_source< std::string, count_position > > );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == "source" );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );

      TAO_PEGTL_TEST_ASSERT( parse< one< 'a' > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == "source" );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 1 );
   }

   template< typename Input >
   void test_text_input( Input& in )
   {
      static_assert( std::is_same_v< typename Input::error_position_t, position_with_source< std::string, text_position > > );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == "source" );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );

      TAO_PEGTL_TEST_ASSERT( parse< seq< one< 'a' >, eol > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == "source" );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 2 );
   }

   template< typename Input >
   void test_error_position( const std::string& expected )
   {
      try {
         Input in( "source", "abc" );
         parse< must< one< 'z' > > >( in );
         TAO_PEGTL_TEST_UNREACHABLE;
      }
      catch( const parse_error_base& e ) {
         TAO_PEGTL_TEST_ASSERT( e.position_string() == expected );
      }
   }

   void test_positions()
   {
      const position_with_source< std::string, count_position > p1( "input", count_position( 3 ) );
      const position_with_source< std::string, count_position > p2( "input", count_position( 3 ) );
      const position_with_source< std::string, count_position > p3( "other", count_position( 3 ) );

      TAO_PEGTL_TEST_ASSERT( p1 == p2 );
      TAO_PEGTL_TEST_ASSERT( p1 != p3 );
      TAO_PEGTL_TEST_ASSERT( p1.base() == count_position( 3 ) );

      std::ostringstream os;
      os << p1;
      TAO_PEGTL_TEST_ASSERT( os.str() == "input@3" );
   }

   void unit_test()
   {
      {
         view_input< scan::lf, char, std::string > in( "source", "abc" );
         test_count_input( in );
      }
      {
         copy_input< scan::lf, std::string, std::string > in( "source", "abc" );
         test_count_input( in );
      }
      {
         text_view_input< scan::lf, char, std::string > in( "source", "a\n" );
         test_text_input( in );
      }
      {
         text_copy_input< scan::lf, std::string, std::string > in( "source", "a\n" );
         test_text_input( in );
      }

      test_error_position< view_input< scan::lf, char, std::string > >( "source@0" );
      test_error_position< copy_input< scan::lf, std::string, std::string > >( "source@0" );
      test_error_position< text_view_input< scan::lf, char, std::string > >( "source@1:1(0)" );
      test_error_position< text_copy_input< scan::lf, std::string, std::string > >( "source@1:1(0)" );

      test_positions();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
