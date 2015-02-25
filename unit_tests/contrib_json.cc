// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <cassert>

#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/json.hh>

using GRAMMAR = pegtl::must< pegtl::json::text, pegtl::eof >;

void fail( const std::string& in )
{
   try {
      pegtl::parse< GRAMMAR >( in, "expect_exception" );
      std::cerr << "FAILED: " << in << std::endl;
      assert( false );
   }
   catch( ... ) {
   }
}

int main( int, char ** )
{
   // ensure the grammar does not contain any obvious problems
   pegtl::analyze< GRAMMAR >();

   // some unit tests that should work
   pegtl::parse< GRAMMAR >( "[]", "test" );
   pegtl::parse< GRAMMAR >( "{}", "test" );
   pegtl::parse< GRAMMAR >( " [ ] ", "test" );
   pegtl::parse< GRAMMAR >( " { } ", "test" );
   pegtl::parse< GRAMMAR >( "   [   ]   ", "test" );
   pegtl::parse< GRAMMAR >( "   {   }   ", "test" );
   pegtl::parse< GRAMMAR >( "[[{}],[],{}]", "test" );
   pegtl::parse< GRAMMAR >( "[ null, true, false, 0, 1, 2, 123, 1.23, 0.12, -1, -0, -1.23, \"\", \"abc\" ]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\b\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\f\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\n\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\r\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\t\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\/\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\\\\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\\"\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\u002C\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\u002c\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"ab\\u002Ccd\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"ab\\u002ccd\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\uD834\\uDD1E\"]", "test" );
   pegtl::parse< GRAMMAR >( "[\"\\uD834\"]", "test" ); // unfortunately, this is valid for the grammar...
   pegtl::parse< GRAMMAR >( "[\"\\uDD1E\"]", "test" ); // ...although both inputs are illegal in unicode.
   pegtl::parse< GRAMMAR >( "[\"\xC3\x84\"]", "test" ); // German a-umlaut
   pegtl::parse< GRAMMAR >( "[\"\xF4\x8F\xBF\xBF\"]", "test" ); // largest allowed codepoint U+10FFFF
   pegtl::parse< GRAMMAR >( "[\"\U0010FFFF\"]", "test" ); // largest allowed codepoint U+10FFFF

   // some unit tests that should fail
   fail( "" );
   fail( " " );
   fail( "   " );
   fail( "[" );
   fail( "]" );
   fail( " [" );
   fail( " ]" );
   fail( "[ " );
   fail( "] " );
   fail( " [ " );
   fail( " ] " );
   fail( "[\"\\a\"]" );
   fail( "[\"\\c\"]" );
   fail( "[\"\\d\"]" );
   fail( "[\"\\e\"]" );
   fail( "[\"\\v\"]" );
   fail( "[\"\\'\"]" );
   fail( "[\"\b\"]" );
   fail( "[\"\f\"]" );
   fail( "[\"\n\"]" );
   fail( "[\"\r\"]" );
   fail( "[\"\t\"]" );
   fail( "[\"\\\"]" );
   fail( "[\"\\\\\\\"]" );
   fail( "[\"\\u12\"]" );
   fail( "[\"\xFF\"]" );
   fail( "[\"\xF4\x90\x80\x80\"]" );
   fail( "[\"\xF7\xBF\xBF\xBF\"]" );
   return 0;
}
