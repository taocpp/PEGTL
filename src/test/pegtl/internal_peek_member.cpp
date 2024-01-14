// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>

#include "test.hpp"

#include <tao/pegtl/internal/peek_member.hpp>

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

      const int method3_int = 3333;

      [[nodiscard]] const int* method3() const noexcept
      {
         return &method3_int;
      }

      int other1 = 901;
      int other2 = 902;
      int other3 = 903;
   };

   [[nodiscard]] int get_other1( const token& t ) noexcept
   {
      return t.other1;
   }

   [[nodiscard]] const int& get_other2( const token& t ) noexcept
   {
      return t.other2;
   }

   [[nodiscard]] const int* get_other3( const token& t ) noexcept
   {
      return &t.other3;
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
         using peek_type = internal::peek_member< &token::local1 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 12 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::local2 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 23 );
      } {
         the_integer = 42;
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::global1 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 42 );
      } {
         the_integer = -120;
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::global2 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == -120 );
      } {
         the_integer = 0;
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::global3 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 0 );
      } {
         the_integer = 65536;
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::global4 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 65536 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::method1 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 503 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::method2 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 102 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &token::method3 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 3333 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &get_other1 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 901 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &get_other2 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 902 );
      } {
         test_integer = -1;
         test_input in( ts );
         using peek_type = internal::peek_member< &get_other3 >;
         static_assert( peek_type::bulk< test_input >() );
         static_assert( peek_type::size< test_input >() == 1 );
         const auto b = parse< function< func1, peek_type > >( in );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( test_integer == 903 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
