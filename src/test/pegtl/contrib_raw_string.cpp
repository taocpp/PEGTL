// Copyright (c) 2016-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_ascii.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/contrib/raw_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   std::string content;

   using rstring = raw_string< '[', '=', ']' >;
   using qstring = raw_string< '[', '=', ']', alpha, digit >;

   template< typename Rule >
   struct raction
   {};

   template<>
   struct raction< rstring::content >
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& in, const States&... /*unused*/ )
      {
         content.assign( in.begin(), in.end() );
      }
   };

   template< typename Rule >
   struct qaction
   {};

   template<>
   struct qaction< qstring::content >
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& in, const States&... /*unused*/ )
      {
         content.assign( in.begin(), in.end() );
      }
   };

   struct rgrammar
      : seq< rstring, eof >
   {};

   struct qgrammar
      : seq< qstring, eof >
   {};

   template< typename Rule, template< typename > class Action, unsigned M, unsigned N >
   void verify_data( const std::size_t line, const char* file, const char ( &m )[ M ], const char ( &n )[ N ], const std::size_t count, const std::size_t dline, const std::size_t column )
   {
      content.clear();
      text_view_input< ascii::lf_crlf > in( m, m + M - 1 );
      const auto r = parse< Rule, Action >( in );
      if( ( !r ) || ( content != std::string_view( n, N - 1 ) ) ) {
         TAO_PEGTL_TEST_FAILED( "input data [ '" << m << "' ] expected success with [ '" << n << "' ] but got [ '" << content << "' ] result [ " << r << " ]" );  // LCOV_EXCL_LINE
      }
      TAO_PEGTL_TEST_ASSERT( test::equal( in.direct_position(), count, dline, column ) );
      content.clear();
      lazy_view_input< ascii::lf_crlf > in2( m, m + M - 1 );
      const auto r2 = parse< Rule, Action >( in2 );
      if( ( !r2 ) || ( content != std::string_view( n, N - 1 ) ) ) {
         TAO_PEGTL_TEST_FAILED( "input data [ '" << m << "' ] with tracking_mode::lazy expected success with [ '" << n << "' ] but got [ '" << content << "' ] result [ " << r2 << " ]" );  // LCOV_EXCL_LINE
      }
      TAO_PEGTL_TEST_ASSERT( test::equal( in.current_position(), count, dline, column ) );
   }

   template< typename Rule >
   void verify_fail( const std::size_t line, const char* file, const std::string& s )
   {
      text_view_input< ascii::lf_crlf > in( s );
      if( parse< Rule >( in ) ) {
         TAO_PEGTL_TEST_FAILED( "expected exception" );  // LCOV_EXCL_LINE
      }
   }

   void unit_test()
   {
      verify_analyze< rstring >( __LINE__, __FILE__, true, false );
      verify_analyze< qstring >( __LINE__, __FILE__, true, false );

      verify_analyze< raw_string< 'a', 'b', 'c', star< star< any > > > >( __LINE__, __FILE__, true, true );

      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[[]]", "", 4, 1, 5 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[[foo]]", "foo", 7, 1, 8 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[===[foo]===]", "foo", 13, 1, 14 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[===[\nfoo]===]", "foo", 14, 2, 9 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[===[\r\nfoo]===]", "foo", 15, 2, 9 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[===[\0\0\0]===]", "\0\0\0", 13, 1, 14 );
      verify_data< rgrammar, raction >( __LINE__, __FILE__, "[=[\na\nb\nc\n]=]", "a\nb\nc\n", 13, 5, 4 );

      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[[]]", "", 4, 1, 5 );
      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[[a1]]", "a1", 6, 1, 7 );
      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[===[a1]===]", "a1", 12, 1, 13 );
      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[===[\na1]===]", "a1", 13, 2, 8 );
      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[===[\r\na1]===]", "a1", 14, 2, 8 );
      verify_data< qgrammar, qaction >( __LINE__, __FILE__, "[===[a0a1a2a3]===]", "a0a1a2a3", 18, 1, 19 );

      verify_fail< rgrammar >( __LINE__, __FILE__, "" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[=" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[=[" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[=[]=" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[=[]]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[[]] " );
      verify_fail< rgrammar >( __LINE__, __FILE__, " [[]]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[=[]-]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[-[]=]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[-[]-]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[===[]====]" );
      verify_fail< rgrammar >( __LINE__, __FILE__, "[====[]===]" );

      verify_fail< qgrammar >( __LINE__, __FILE__, "" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[]=" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[]]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[[]] " );
      verify_fail< qgrammar >( __LINE__, __FILE__, " [[]]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[]-]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[-[]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[-[]-]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[===[]====]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[====[]===]" );

      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[-]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[1]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[a]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[a+]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[aa]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[11]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[a1a]=]" );
      verify_fail< qgrammar >( __LINE__, __FILE__, "[=[a1aa]=]" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
