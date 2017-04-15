// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/trace.hpp>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      using GRAMMAR = sor< failure, one< 'a' > >;

      void unit_test()
      {
         memory_input<> in( "ab", "trace test please ignore" );
         if( !parse< GRAMMAR, nothing, tracer >( in ) ) {
            ++failed;
         }
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
