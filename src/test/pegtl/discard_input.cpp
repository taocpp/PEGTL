// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <string>

#include "test.hpp"

#include <tao/pegtl/internal/cstring_reader.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, template< typename... > class Action >
   bool parse_cstring( const char* string, const char* source, const std::size_t maximum )
   {
      buffer_input< internal::cstring_reader, eol::lf_crlf, const char*, 1 > in( source, maximum, string );
      return parse< Rule, Action >( in );
   }

   // clang-format off
   struct a : one< 'a' > {};
   struct b : one< 'b' > {};
   struct c : one< 'c' > {};
   struct d : one< 'd' > {};
   struct x : one< 'x' > {};
   struct dd : two< '.' > {};
   struct ab : seq< a, b > {};
   struct cx : seq< c, x > {};
   struct cd : seq< c, d > {};
   struct my_grammar : seq< ab, dd, discard, sor< ab, dd >, discard,
                            sor< cx, cd >, dd, discard,
                            sor< cd, cx > > {};

   template< typename Rule > struct my_action {};
   template<> struct my_action< ab > : discard_input {};
   template<> struct my_action< c > : discard_input_on_failure {};
   template<> struct my_action< d > : discard_input_on_success {};
   // clang-format on

   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( parse_cstring< my_grammar, my_action >( "ab....cd..cx", TAO_TEST_LINE, 1 ) );
      // We need one extra byte in the buffer so that eof calling in.empty() calling in.require( 1 ) does not throw a "require beyond end of buffer" exception.
      TAO_PEGTL_TEST_THROWS( parse_cstring< seq< my_grammar, eof >, my_action >( "ab....cd..cx", TAO_TEST_LINE, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< seq< my_grammar, eof >, my_action >( "ab....cd..cx", TAO_TEST_LINE, 2 ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
