// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
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
#include "test_utility.hpp"
#include "verify_meta.hpp"

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< raise< int >, internal::raise< int > >();
      verify_meta< raise< any >, internal::raise< any > >();

      verify_analyze< raise< int > >( __LINE__, __FILE__, true, false );
      verify_analyze< raise< any > >( __LINE__, __FILE__, true, false );

      text_view_input< scan::lf > in( "foo" );

      TAO_PEGTL_TEST_THROWS( parse< raise< int > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.size( 4 ) == 3 );
      TAO_PEGTL_TEST_THROWS( parse< raise< any > >( in ) );
      TAO_PEGTL_TEST_ASSERT( in.size( 4 ) == 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#if defined( _MSC_VER )
#pragma warning( pop )
#endif

#include "main.hpp"

#endif
