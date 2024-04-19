// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"

#include <tao/pegtl/internal/unwind_guard.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test1()
   {
      internal::unwind_guard ug( []() { throw 42; } );
      ug.unwind.reset();
   }

   void test2()
   {
      int i = 1;
      internal::unwind_guard( [ & ]() { i = 2; } );  // Anonymous object.
      TAO_PEGTL_TEST_ASSERT( i == 2 );
   }

   void test3()
   {
      int i = 1;
      try {
         internal::unwind_guard ug( [ & ]() { i = 2; } );
         TAO_PEGTL_TEST_ASSERT( i == 1 );
         throw 42;
      }
      catch( ... ) {
         TAO_PEGTL_TEST_ASSERT( i == 2 );
      }
   }

   void unit_test()
   {
      test1();
      test2();
      test3();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
