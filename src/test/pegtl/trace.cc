// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include <tao/pegtl/trace.hh>

namespace tao
{
   namespace pegtl
   {
      using GRAMMAR = tao::pegtl::sor< tao::pegtl::failure, tao::pegtl::one< 'a' > >;

      void unit_test()
      {
         if ( ! tao::pegtl::trace_string< GRAMMAR >( "ab", "trace test please ignore" ) ) {
            ++failed;
         }
      }

   } // namespace pegtl

} // namespace tao

#include "main.hh"
