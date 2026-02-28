// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if defined( __cpp_exceptions ) && defined( __cpp_rtti )

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

#include <tao/pegtl/config.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace test
   {
      std::size_t failed = 0;

   }  // namespace test

   struct foo
      : std::exception
   {};

   struct bar
      : std::exception
   {};

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

   void simple()
   {
      try {
         std::cerr << __FUNCTION__ << " throw foo" << std::endl;
         throw foo();
      }
      catch( const foo& /*unused*/ ) {
         std::cerr << __FUNCTION__ << " catch foo" << std::endl;
         std::cerr << __FUNCTION__ << " return success" << std::endl;
         return;
      }
      std::cerr << __FUNCTION__ << " failed catch foo" << std::endl;
      ++test::failed;
      std::cerr << __FUNCTION__ << " return failure" << std::endl;
      return;
   }

   void nested1()
   {
      try {
         try {
            std::cerr << __FUNCTION__ << " throw foo" << std::endl;
            throw foo();
         }
         catch( const foo& /*unused*/ ) {
            std::cerr << __FUNCTION__ << " catch foo" << std::endl;
            std::cerr << __FUNCTION__ << " throw bar with nested" << std::endl;
            std::throw_with_nested( bar() );
         }
         std::cerr << __FUNCTION__ << " failed catch foo" << std::endl;
         ++test::failed;
         std::cerr << __FUNCTION__ << " return failure" << std::endl;
         return;
      }
      catch( const bar& /*unused*/ ) {
         std::cerr << __FUNCTION__ << " catch foo" << std::endl;
         std::cerr << __FUNCTION__ << " return success" << std::endl;
         return;
      }
      std::cerr << __FUNCTION__ << " failed catch bar" << std::endl;
      ++test::failed;
      std::cerr << __FUNCTION__ << " return failure" << std::endl;
      return;
   }

   void nested2()
   {
      try {
         try {
            std::cerr << __FUNCTION__ << " throw foo" << std::endl;
            throw foo();
         }
         catch( const foo& /*unused*/ ) {
            std::cerr << __FUNCTION__ << " catch foo" << std::endl;
            std::cerr << __FUNCTION__ << " throw bar with nested" << std::endl;
            std::throw_with_nested( bar() );
         }
         std::cerr << __FUNCTION__ << " failed catch foo" << std::endl;
         ++test::failed;
         std::cerr << __FUNCTION__ << " return failure" << std::endl;
         return;
      }
      catch( const bar& e ) {
         std::cerr << __FUNCTION__ << " catch foo" << std::endl;
         try {
            std::cerr << __FUNCTION__ << " rethrow if nested" << std::endl;
            std::rethrow_if_nested( e );
         }
         catch( const foo& /*unused*/ ) {
            std::cerr << __FUNCTION__ << " catch foo nested" << std::endl;
            std::cerr << __FUNCTION__ << " return success" << std::endl;
            return;
         }
         std::cerr << __FUNCTION__ << " failed catch foo nested" << std::endl;
         ++test::failed;
         std::cerr << __FUNCTION__ << " return failure" << std::endl;
         return;
      }
      std::cerr << __FUNCTION__ << " failed catch bar" << std::endl;
      ++test::failed;
      std::cerr << __FUNCTION__ << " return failure" << std::endl;
      return;
   }

#if defined( _MSC_VER )
#pragma warning( pop )
#endif

   void unit_test()
   {
      simple();
      nested1();
      nested2();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#else

int main()
{
   return 0;
}

#endif
