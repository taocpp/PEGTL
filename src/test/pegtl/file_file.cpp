// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include "verify_file.hpp"

namespace tao
{
   namespace pegtl
   {
      void unit_test()
      {
         verify_file< file_input<> >();
      }

   }  // namespace pegtl

}  // namespace tao

#include "main.hpp"
