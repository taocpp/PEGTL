// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/action/add_guard.hpp>
#include <tao/pegtl/contrib/alphabet_rules.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   bool ctor = false;
   bool dtor = false;

   struct guard1
   {
      guard1()
      {
         TAO_PEGTL_TEST_ASSERT( !ctor );
         TAO_PEGTL_TEST_ASSERT( !dtor );
         ctor = true;
      }

      ~guard1()
      {
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( !dtor );
         dtor = true;
      }

      template< typename ParseInput >
      void success( const ParseInput& in, std::size_t& st )
      {
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( !dtor );
         st = in.current_position().count;
      }

      guard1( guard1&& ) = delete;
      guard1( const guard1& ) = delete;

      void operator=( guard1&& ) = delete;
      void operator=( const guard1&& ) = delete;
   };

   struct guard2
   {
      template< typename ParseInput >
      guard2( const ParseInput& in, const int )
      {
         TAO_PEGTL_TEST_ASSERT( !ctor );
         TAO_PEGTL_TEST_ASSERT( !dtor );
         TAO_PEGTL_TEST_ASSERT( in.current_position().count == 1 );
         ctor = true;
      }

      ~guard2()
      {
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( !dtor );
         dtor = true;
      }

      guard2( guard2&& ) = delete;
      guard2( const guard2& ) = delete;

      void operator=( guard2&& ) = delete;
      void operator=( const guard2&& ) = delete;
   };

   using namespace alphabet::rules;

   struct grammar
      : seq< a, sor< b, c >, d, eof >
   {};

   template< typename Rule >
   struct action1
      : nothing< Rule >
   {};

   template<>
   struct action1< sor< b, c > >
      : add_guard< guard1 >
   {};

   template<>
   struct action1< b >
   {
      static void apply0( std::size_t& /*unused*/ ) noexcept
      {}
   };

   template< typename Rule >
   struct action2
      : nothing< Rule >
   {};

   template<>
   struct action2< sor< b, c > >
      : add_guard< guard2 >
   {};

   template<>
   struct action2< c >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, const int& /*unused*/ ) noexcept
      {}
   };

   void unit_test()
   {
      // guard1 parse success
      {
         ctor = false;
         dtor = false;
         std::size_t st = 0;
         view_input<> in( "abd" );
         const bool b = parse< grammar, action1 >( in, st );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( dtor );
         TAO_PEGTL_TEST_ASSERT( st == 2 );
      }
      // guard1 parse failure
      {
         ctor = false;
         dtor = false;
         std::size_t st = 0;
         view_input<> in( "ad" );
         const bool b = parse< grammar, action1 >( in, st );
         TAO_PEGTL_TEST_ASSERT( !b );
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( dtor );
         TAO_PEGTL_TEST_ASSERT( st == 0 );
      }
      // guard2 parse success
      {
         ctor = false;
         dtor = false;
         const int i = 42;
         view_input<> in( "abd" );
         const bool b = parse< grammar, action2 >( in, i );
         TAO_PEGTL_TEST_ASSERT( b );
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( dtor );
         TAO_PEGTL_TEST_ASSERT( i == 42 );
      }
      // guard2 parse failure
      {
         ctor = false;
         dtor = false;
         const int i = 42;
         view_input<> in( "ad" );
         const bool b = parse< grammar, action2 >( in, i );
         TAO_PEGTL_TEST_ASSERT( !b );
         TAO_PEGTL_TEST_ASSERT( ctor );
         TAO_PEGTL_TEST_ASSERT( dtor );
         TAO_PEGTL_TEST_ASSERT( i == 42 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
