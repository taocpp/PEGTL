// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

#include "verify_seqs.hh"

namespace pegtl
{
   struct test_state_state
   {
      template< typename Input >
      test_state_state( const Input & )
      { }

      template< typename Input >
      void success( const Input & ) const
      { }
   };

   template< typename ... Rules > using test_state_rule = state< test_state_state, Rules ... >;

   void unit_test()
   {
      verify_seqs< test_state_rule >();
   }

} // pegtl

#include "main.hh"
