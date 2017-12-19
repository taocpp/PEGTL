// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include "verify_seqs.hpp"

namespace tao
{
   namespace pegtl
   {
      void unit_test()
      {
         verify_seqs< seq >();
      }

   }  // namespace pegtl

}  // namespace tao

#include "main.hpp"
