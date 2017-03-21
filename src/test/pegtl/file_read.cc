// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include "verify_file.hh"

namespace pegtl
{
   void unit_test()
   {
      verify_file< read_parser >();
   }

} // namespace pegtl

#include "main.hh"
