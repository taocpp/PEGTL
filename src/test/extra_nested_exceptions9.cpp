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
   [[nodiscard]] inline bool operator==( const parse_error_base& l, const parse_error_base& r ) noexcept
   {
      return ( l.message() == r.message() ) && ( l.position_string() == r.position_string() );
   }

   void test9()
   {
      try {
         try {
            try {
               throw_parse_error( "first", count_position( 1 ) );
            }
            catch( ... ) {
               throw_parse_error_with_nested( "second", count_position( 2 ) );
            }
         }
         catch( ... ) {
            throw_parse_error_with_nested( "third", count_position( 3 ) );
         }
      }
      catch( const parse_error_base& e ) {
         const auto v1 = flatten();
         const auto v2 = flatten( e );
         TAO_PEGTL_TEST_ASSERT( v1 == v2 );
         TAO_PEGTL_TEST_ASSERT( v1.size() == 3 );
         TAO_PEGTL_TEST_ASSERT( v1[ 0 ].message() == "first" );
         TAO_PEGTL_TEST_ASSERT( v1[ 1 ].message() == "second" );
         TAO_PEGTL_TEST_ASSERT( v1[ 2 ].message() == "third" );
      }
   }

   void unit_test()
   {
      test9();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
