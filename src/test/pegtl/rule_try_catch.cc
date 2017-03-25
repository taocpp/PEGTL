// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include "verify_seqs.hh"

namespace tao
{
   namespace pegtl
   {
      template< typename ... Rules >
      using test_try_catch_rule = try_catch< must< Rules ... > >;

      void unit_test()
      {
         verify_seqs< try_catch >();
         verify_seqs< test_try_catch_rule >();
      }

   } // namespace pegtl

} // namespace tao

#include "main.hh"
