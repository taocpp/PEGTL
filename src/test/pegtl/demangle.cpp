// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         const std::string s = "something that can't be demangled";
         const std::string a = internal::demangle( s.c_str() );
         TAOCPP_PEGTL_TEST_ASSERT( a == s );
         const std::string b = internal::demangle< std::string >();
         (void)b;  // Not standardised.
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
