// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_fail.hpp"

#include <tao/pegtl/contrib/unescape.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct escaped_c : one< '"', '\\', 't' > {};
   struct escaped_u : seq< one< 'u' >, rep< 4, must< xdigit > > > {};
   struct escaped_U : seq< one< 'U' >, rep< 8, must< xdigit > > > {};
   struct escaped_j : list< seq< one< 'j' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
   struct escaped_x : seq< one< 'x' >, rep< 2, must< xdigit > > > {};
   struct escaped : sor< escaped_c, escaped_u, escaped_U, escaped_j, escaped_x > {};
   struct character : if_then_else< one< '\\' >, must< escaped >, utf8::any > {};
   struct unstring : until< eof, character > {};

   template< typename Rule > struct unaction {};

   template<> struct unaction< escaped_c > : unescape::unescape_c< escaped_c, '"', '\\', '\t' > {};
   template<> struct unaction< escaped_u > : unescape::unescape_u {};
   template<> struct unaction< escaped_U > : unescape::unescape_u {};
   template<> struct unaction< escaped_j > : unescape::unescape_j {};
   template<> struct unaction< escaped_x > : unescape::unescape_x {};
   template<> struct unaction< utf8::any > : unescape::append_all {};
   // clang-format on

   template< unsigned M, unsigned N >
   void verify_data( const char ( &m )[ M ], const char ( &n )[ N ] )
   {
      std::string s;
      memory_input in( m, M - 1, __FUNCTION__ );
      parse< unstring, unaction >( in, s );
      if( s != std::string( n, N - 1 ) ) {
         throw std::runtime_error( "test failed!" );
      }
   }

   void unit_test()
   {
      verify_data( "\\t", "\t" );
      verify_data( "\\\\", "\\" );
      verify_data( "abc", "abc" );
      verify_data( "\\\"foo\\\"", "\"foo\"" );
      verify_data( "\\x20", " " );
      verify_data( "\\x30", "0" );
      verify_data( "\\x2000", " 00" );
      verify_data( "\\u0020", " " );
      verify_data( "\\u0020\\u0020", "  " );
      verify_data( "\\u00e4", "\xc3\xa4" );
      verify_data( "\\u00E4", "\xC3\xA4" );
      verify_data( "\\u20ac", "\xe2\x82\xac" );

      TAO_PEGTL_TEST_THROWS( verify_data( "\\ud800", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\ud800X", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\ud800\\u0020", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\ud800\\udc00", "" ) );  // unescape_u does not support surrogate pairs.
      TAO_PEGTL_TEST_THROWS( verify_data( "\\udc00\\ud800", "" ) );

      verify_data( "\\j0020", " " );
      verify_data( "\\j0020\\j0020", "  " );
      verify_data( "\\j20ac", "\xe2\x82\xac" );

      verify_data( "\\jd800\\jdc00", "\xf0\x90\x80\x80" );  // unescape_j does support proper surrogate pairs.

      TAO_PEGTL_TEST_THROWS( verify_data( "\\jd800", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\jd800X", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\jd800\\j0020", "" ) );
      TAO_PEGTL_TEST_THROWS( verify_data( "\\jdc00\\jd800", "" ) );

      verify_data( "\\j0000\\u0000\x00", "\x00\x00\x00" );

      std::string s;
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\\\\\", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\x", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\xx", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\xa", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\x1", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\x1h", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\x", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\xx", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\xa", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\x1", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "a\\x1h", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\a", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\_", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\z", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\1", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\a00", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\_1111", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\z22222222", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\13333333333333333", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\u", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\uu", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\uuuu", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\u123", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\u999", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\u444h", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\j", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\ju", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\juuu", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\j123", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\j999", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\j444h", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\U00110000", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\U80000000", s );
      verify_fail< unstring, unaction >( __LINE__, __FILE__, "\\Uffffffff", s );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
