// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#else

#include <vector>

#include "test.hpp"
#include "test_ascii.hpp"
#include "test_utility.hpp"
#include "verify_seqs.hpp"

#include <tao/pegtl/contrib/nested_exceptions.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   using test_try_catch_rule = try_catch_raise_nested< must< Rules... > >;

   template< typename... Rules >
   using test_try_catch_any_rule = try_catch_any_raise_nested< must< Rules... > >;

   template< template< typename... > class Rule >
   void verify_nested()
   {
      try {
         text_view_input< eols::scan::lf > in( "0" );
         parse< Rule< alpha > >( in );
         TAO_PEGTL_TEST_UNREACHABLE;
      }
      catch( ... ) {
         const std::vector< parse_error_base > e = nested::flatten();
         TAO_PEGTL_TEST_ASSERT( e.size() == 2 );
      }
   }

   void unit_test()
   {
      verify_seqs< try_catch_raise_nested >();
      verify_seqs< try_catch_any_raise_nested >();

      verify_nested< test_try_catch_rule >();
      verify_nested< test_try_catch_any_rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
