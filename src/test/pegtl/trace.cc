// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hh"

#include <tao/pegtl/trace.hh>

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      using GRAMMAR = tao::TAOCPP_PEGTL_NAMESPACE::sor< tao::TAOCPP_PEGTL_NAMESPACE::failure, tao::TAOCPP_PEGTL_NAMESPACE::one< 'a' > >;

      void unit_test()
      {
         if ( ! tao::TAOCPP_PEGTL_NAMESPACE::trace_string< GRAMMAR >( "ab", "trace test please ignore" ) ) {
            ++failed;
         }
      }

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#include "main.hh"
