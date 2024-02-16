// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/action/add_guard.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   bool ctor = false;
   bool dtor = false;

   struct test_class1
   {
      template< typename ParseInput >
      test_class1( const ParseInput& /*unused*/ )
      {
         TAO_PEGTL_TEST_ASSERT( ctor == false );
         TAO_PEGTL_TEST_ASSERT( dtor == false );

         ctor = true;
      }

      test_class1( test_class1&& ) = delete;
      test_class1( const test_class1& ) = delete;

      ~test_class1()
      {
         TAO_PEGTL_TEST_ASSERT( ctor == true );
         TAO_PEGTL_TEST_ASSERT( dtor == false );

         dtor = true;
      }

      void operator=( test_class1&& ) = delete;
      void operator=( const test_class1& ) = delete;
   };

   using test_grammar = sor< alpha, digit >;

   template< typename Rule >
   struct test_action1
      : nothing< Rule >
   {};

   template<>
   struct test_action1< alpha >
   {
      static void apply0()
      {
         TAO_PEGTL_TEST_ASSERT( ctor == true );
         TAO_PEGTL_TEST_ASSERT( dtor == false );
      }
   };

   template<>
   struct test_action1< sor< alpha, digit > >
      : add_guard< test_class1 >
   {};

   void unit_test_one()
   {
      ctor = false;
      dtor = false;

      text_view_input< scan::lf > in( "a" );
      TAO_PEGTL_TEST_ASSERT( parse< test_grammar, test_action1 >( in ) );

      TAO_PEGTL_TEST_ASSERT( ctor == true );
      TAO_PEGTL_TEST_ASSERT( dtor == true );
   }

   struct test_class2
   {
      template< typename ParseInput >
      test_class2( const ParseInput& /*unused*/, std::string& c, std::string& /*unused*/ )
      {
         TAO_PEGTL_TEST_ASSERT( ctor == false );
         TAO_PEGTL_TEST_ASSERT( dtor == false );

         ctor = true;
         c = "c";
      }

      test_class2( test_class2&& ) = delete;
      test_class2( const test_class2& ) = delete;

      ~test_class2()
      {
         TAO_PEGTL_TEST_ASSERT( ctor == true );
         TAO_PEGTL_TEST_ASSERT( dtor == false );

         dtor = true;
      }

      void operator=( test_class2&& ) = delete;
      void operator=( const test_class2& ) = delete;

      template< typename ParseInput >
      void success( const ParseInput& /*unused*/, std::string& /*unused*/, std::string& o )
      {
         o = "o";
      }
   };

   template< typename Rule >
   struct test_action2
      : nothing< Rule >
   {};

   template<>
   struct test_action2< alpha >
   {
      static void apply0( std::string& /*unused*/, std::string& /*unused*/ )
      {
         TAO_PEGTL_TEST_ASSERT( ctor == true );
         TAO_PEGTL_TEST_ASSERT( dtor == false );
      }
   };

   template<>
   struct test_action2< sor< alpha, digit > >
      : add_guard< test_class2 >
   {};

   void unit_test_two()
   {
      ctor = false;
      dtor = false;

      std::string c;
      std::string o;

      text_view_input< scan::lf > in( "a" );
      TAO_PEGTL_TEST_ASSERT( parse< test_grammar, test_action2 >( in, c, o ) );

      TAO_PEGTL_TEST_ASSERT( ctor == true );
      TAO_PEGTL_TEST_ASSERT( dtor == true );

      TAO_PEGTL_TEST_ASSERT( c == "c" );
      TAO_PEGTL_TEST_ASSERT( o == "o" );
   }

   void unit_test()
   {
      unit_test_one();
      unit_test_two();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
