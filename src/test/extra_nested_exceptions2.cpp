// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <tao/pegtl/system.hpp>

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"

#include <string>
#include <utility>
#include <vector>

#include <tao/pegtl/extra/nested_exceptions.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   const std::string s1 = "foo";
   const std::string s2 = "bar";

   void test33()
   {
      try {
         try {
            throw std::string( s1 );
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( const std::exception& /*unused*/ ) {
      }
      catch( const std::string& /*unused*/ ) {
      }
   }

   void unit_test()
   {
      test33();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
