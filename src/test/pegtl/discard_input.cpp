// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/internal/cstring_reader.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule,
             template< typename... > class Action = nothing,
             typename... States >
   bool parse_cstring( const char* string, const char* source, const std::size_t maximum, States&&... st )
   {
      buffer_input< internal::cstring_reader > in( source, maximum, string );
      return parse< Rule, Action >( in, st... );
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
   struct grammar : seq< ab, dd, sor< ab, dd >, discard,
                         sor< cx, cd >, dd, discard,
                         sor< cd, cx >, eof > {};

   template< typename Rule > struct my_action {};
   template<> struct my_action< ab > : discard_input {};
   template<> struct my_action< c > : discard_input_on_failure {};
   template<> struct my_action< d > : discard_input_on_success {};
   // clang-format on

   void unit_test()
   {
      const char* test_data = "ab....cd..cx";
      TAO_PEGTL_TEST_ASSERT( parse_cstring< grammar, my_action >( test_data, "test data", 2 ) );
      TAO_PEGTL_TEST_ASSERT( !parse_cstring< grammar, my_action >( test_data, "test data", 1 ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
