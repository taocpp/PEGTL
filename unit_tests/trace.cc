// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

#include <pegtl/trace.hh>

namespace pegtl
{
   using GRAMMAR = pegtl::sor< pegtl::failure, pegtl::one< 'a' > >;

   void unit_test()
   {
      if ( ! pegtl::trace_string< GRAMMAR >( "ab", "trace test please ignore" ) ) {
         ++failed;
      }
   }

} // namespace pegtl

#include "main.hh"
