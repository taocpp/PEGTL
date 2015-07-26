// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

#include <pegtl/analyze.hh>
#include <pegtl/contrib/json.hh>

namespace pegtl
{
   using GRAMMAR = must< json::text, eof >;

   void unit_test()
   {
      const auto p = analyze< GRAMMAR >();
      assert( p == 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "{}", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, " [ ] ", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, " { } ", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "   [   ]   ", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "   {   }   ", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[[{}],[],{}]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[ null, true, false, 0, 1, 2, 123, 1.23, 0.12, -1, -0, -1.23, \"\", \"abc\" ]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\b\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\f\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\n\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\r\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\t\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\/\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\\\\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\\"\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\u002C\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\u002c\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"ab\\u002Ccd\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"ab\\u002ccd\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\uD834\\uDD1E\"]", result_type::SUCCESS, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\uD834\"]", result_type::SUCCESS, 0 ); // unfortunately, this is valid for the grammar...
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\\uDD1E\"]", result_type::SUCCESS, 0 ); // ...although both inputs are illegal in unicode.
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\xC3\x84\"]", result_type::SUCCESS, 0 ); // German a-umlaut
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\xF4\x8F\xBF\xBF\"]", result_type::SUCCESS, 0 ); // largest allowed codepoint U+10FFFF
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "[\"\U0010FFFF\"]", result_type::SUCCESS, 0 ); // largest allowed codepoint U+10FFFF

      verify_fail< GRAMMAR >( __LINE__, __FILE__, "" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, " " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "   " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, " [" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, " ]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[ " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "] " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, " [ " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, " ] " );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\a\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\c\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\d\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\e\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\v\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\'\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\b\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\f\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\n\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\r\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\t\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\\\\\\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\\u12\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\xFF\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\xF4\x90\x80\x80\"]" );
      verify_fail< GRAMMAR >( __LINE__, __FILE__, "[\"\xF7\xBF\xBF\xBF\"]" );
   }

} // pegtl

#include "main.hh"
