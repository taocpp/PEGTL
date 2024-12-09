// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/contrib/dispatch.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct action1
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, std::string& st )
      {
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         st += in.peek_char();
      }
   };

   template< typename Rule >
   struct action2
   {
      static void apply0( std::string& st )
      {
         st += '2';
      }
   };

   struct grammar
      : seq< one< 'a' >, one< 'b' >, one< 'c' >, one< 'd' >, eof >
   {};

   void unit_test()
   {
      std::string st;
      view_input<> in( "abcd" );
      using clause1 = clause1< action1, one< 'a' >, one< 'c' > >;
      using clause2 = clause2< action2, one< 'b' >, one< 'd' > >;
      const auto b = dispatch< clause1, clause2 >::parse< grammar >( in, st );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( st == "a2c2" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
