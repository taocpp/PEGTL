// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/inputs.hpp>
#include <tao/pegtl/internal/peek_member.hpp>
#include <tao/pegtl/parse.hpp>
#include <tao/pegtl/rules.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static int the_integer = -2;

   struct token
   {
      explicit token( std::string&& s ) noexcept
         : string( std::move( s ) )
      {}

      std::string string;

      int local1 = 12;
      const int local2 = 23;

      int* global1 = &the_integer;
      const int* global2 = &the_integer;
      int* const global3 = &the_integer;
      const int* const global4 = &the_integer;

      [[nodiscard]] int method1() const noexcept
      {
         return 503;
      }

      const int method2_int = 102;

      [[nodiscard]] const int& method2() const noexcept
      {
         return method2_int;
      }

      int other = 900;
   };

   [[nodiscard]] int get_other1( const token& t ) noexcept
   {
      return t.other;
   }

   [[nodiscard]] const int& get_other2( const token& t ) noexcept
   {
      return t.other;
   }

   using tokens = std::vector< token >;

   using test_input = internal::input_with_fakes< internal::copy_input< tokens > >;

   static int test_integer = -1;

   [[nodiscard]] bool func1( const int i ) noexcept
   {
      test_integer = i;
      return true;
   }

   void unit_test()
   {
      tokens ts;
      ts.emplace_back( "0" );
      ts.emplace_back( "token" );
      ts.emplace_back( "something" );
      {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::local1 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 12 );
      } {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::local2 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 23 );
      } {
         the_integer = 42;
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::global1 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 42 );
      } {
         the_integer = -120;
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::global2 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == -120 );
      } {
         the_integer = 0;
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::global3 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 0 );
      } {
         the_integer = 65536;
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::global4 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 65536 );
      } {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::method1 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 503 );
      } {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &token::method2 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 102 );
      } {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &get_other1 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 900 );
      } {
         test_integer = -1;
         test_input in( ts );
         const auto b = parse< function< func1, internal::peek_member< &get_other2 > > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 900 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
