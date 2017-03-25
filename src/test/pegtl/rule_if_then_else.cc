// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include "verify_ifmt.hh"

namespace tao
{
   namespace pegtl
   {
      void unit_test()
      {
         verify_ifmt< if_then_else >();
      }

   } // namespace pegtl

} // namespace tao

#include "main.hh"
