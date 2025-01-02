// Copyright (c) 2019-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "test.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, template< typename... > class Action >
   bool parse_cstring( const char* string, const std::size_t size, const std::size_t chunk )
   {
      dynamic_cstring_input< void > in( size, chunk, string );
      return parse< Rule, Action >( in );
   }

   // clang-format off
   struct n : one< 'n' > {};
   struct a : one< 'a' > {};
   struct f : one< 'f' > {};
   struct s : one< 's' > {};

   template< typename Rule > struct my_action {};
   template<> struct my_action< a > : discard_input {};
   template<> struct my_action< f > : discard_input_on_failure {};
   template<> struct my_action< s > : discard_input_on_success {};
   // clang-format on

   void unit_test()
   {
#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( parse_cstring< rep< 4, sor< n, n > >, my_action >( "nnnn", 2, 1 ) );
#endif
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< a, n > >, my_action >( "nnnn", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< f, n > >, my_action >( "nnnn", 2, 1 ) );
#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( parse_cstring< rep< 4, sor< s, n > >, my_action >( "nnnn", 2, 1 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< n, a > >, my_action >( "aaaa", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< a, a > >, my_action >( "aaaa", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< f, a > >, my_action >( "aaaa", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< s, a > >, my_action >( "aaaa", 2, 1 ) );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( parse_cstring< rep< 4, sor< n, f > >, my_action >( "ffff", 2, 1 ) );
#endif
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< a, f > >, my_action >( "ffff", 2, 1 ) );
#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( parse_cstring< rep< 4, sor< f, f > >, my_action >( "ffff", 2, 1 ) );
      TAO_PEGTL_TEST_THROWS( parse_cstring< rep< 4, sor< s, f > >, my_action >( "ffff", 2, 1 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< n, s > >, my_action >( "ssss", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< a, s > >, my_action >( "ssss", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< f, s > >, my_action >( "ssss", 2, 1 ) );
      TAO_PEGTL_TEST_ASSERT( parse_cstring< rep< 4, sor< s, s > >, my_action >( "ssss", 2, 1 ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
