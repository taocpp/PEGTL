// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include "verify_seqs.hh"

namespace pegtl
{
   template< typename ... Rules > using test_action_rule = action< nothing, Rules ... >;

   void unit_test()
   {
      verify_seqs< test_action_rule >();
   }

} // namespace pegtl

#include "main.hh"
