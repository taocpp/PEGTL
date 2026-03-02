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

   struct visitor
   {
      void operator()( const std::string& s, const std::size_t l )
      {
         v.emplace_back( std::string( "string%" ) + s, l );
      }

      void operator()( const std::exception& e, const std::size_t l )
      {
         v.emplace_back( std::string( "exception%" ) + e.what(), l );
      }

      void operator()( const std::runtime_error& e, const std::size_t l )
      {
         v.emplace_back( std::string( "runtime%" ) + e.what(), l );
      }

      std::vector< std::pair< std::string, std::size_t > > v;
   };

   void test4()
   {
      try {
         try {
            throw std::logic_error( s1 );
         }
         catch( ... ) {
            std::throw_with_nested( std::runtime_error( s2 ) );
         }
      }
      catch( ... ) {
         visitor v;
         inspect< std::exception, std::runtime_error >( v );
         TAO_PEGTL_TEST_ASSERT( v.v.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].first == "exception%foo" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 0 ].second == 1 );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].first == "runtime%bar" );
         TAO_PEGTL_TEST_ASSERT( v.v[ 1 ].second == 0 );
      }
   }

   void unit_test()
   {
      test4();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
