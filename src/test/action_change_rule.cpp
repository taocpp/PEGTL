// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/action/change_rule.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct my_action
      : normal< Rule >
   {};

   template<>
   struct my_action< one< 'a' > >
      : change_rule< one< 'b' > >
   {};

   template<>
   struct my_action< one< 'c' > >
      : change_rule< one< 'c' > >
   {};

   struct grammara
      : seq< one< 'a' >, eolf >
   {};

   struct grammarc
      : seq< one< 'c' >, eolf >
   {};

   void unit_test()
   {
      {
         view_input in( "a" );
         const bool b = parse< grammara, my_action >( in );
         TAO_PEGTL_TEST_ASSERT( !b );
      }
      {
         view_input in( "b" );
         const bool b = parse< grammara, my_action >( in );
         TAO_PEGTL_TEST_ASSERT( b );
      }
      {
         view_input in( "c" );
         const bool b = parse< grammarc, my_action >( in );
         TAO_PEGTL_TEST_ASSERT( b );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
