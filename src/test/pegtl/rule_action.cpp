// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   using test_action_rule = action< nothing, Rules... >;

   template< typename Rule >
   struct action1
      : nothing< Rule >
   {};

   template< typename Rule >
   struct action2
      : nothing< Rule >
   {};

   bool b1 = false;
   bool b2 = false;

   template<>
   struct action1< ascii::alpha >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in ) noexcept
      {
         TAO_PEGTL_TEST_ASSERT( !b1 );
         b1 = true;
         TAO_PEGTL_TEST_ASSERT( in.string_view() == "a" );
      }
   };

   template<>
   struct action2< ascii::alpha >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in ) noexcept
      {
         TAO_PEGTL_TEST_ASSERT( !b2 );
         b2 = true;
         TAO_PEGTL_TEST_ASSERT( in.string_view() == "b" );
      }
   };

   void unit_test()
   {
      verify_meta< action< nothing >, internal::success >();
      verify_meta< action< nothing, eof >, internal::action< nothing, eof >, eof >();
      verify_meta< action< nothing, eof, any >, internal::action< nothing, internal::seq< eof, any > >, internal::seq< eof, any > >();

      verify_seqs< test_action_rule >();
      {
         view_input< scan::lf > in( "ab1" );
         TAO_PEGTL_TEST_ASSERT( parse< seq< ascii::alpha, action< action2, ascii::alpha, ascii::digit > >, action1 >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( b1 && b2 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
