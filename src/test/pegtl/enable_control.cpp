// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct r
      : seq< internal::seq< any > >
   {};

   static_assert( !internal::enable_control< internal::seq< any > > );
   static_assert( internal::enable_control< seq< any > > );
   static_assert( !internal::enable_control< internal::seq< internal::seq< any > > > );
   static_assert( internal::enable_control< seq< internal::seq< any > > > );
   static_assert( internal::enable_control< r > );

   static_assert( !normal< internal::seq< any > >::enable );
   static_assert( normal< seq< any > >::enable );
   static_assert( !normal< internal::seq< internal::seq< any > > >::enable );
   static_assert( normal< seq< internal::seq< any > > >::enable );
   static_assert( normal< r >::enable );

   template< typename R >
   struct a
      : nothing< R >
   {};

   unsigned flags = 0;

   template<>
   struct a< r >
   {
      static void apply0()
      {
         flags |= 0x01;
      }
   };

   template<>
   struct a< any >
   {
      static void apply0()
      {
         flags |= 0x02;
      }
   };

   template<>
   struct a< internal::seq< any > >
   {
      static void apply0()
      {
         flags |= 0x10;
      }
   };

   void unit_test()
   {
      text_view_input< scan::lf > in( "a" );
      const bool b = parse< r, a >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( flags == 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
