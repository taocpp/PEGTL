// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
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
#include "verify_meta.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename ParseInput >
   void unit_test()
   {
      const std::string rulename{ demangle< digit >() };

      ParseInput in( "test_source", "foo\nbar bla blubb\nbaz" );

      try {
         parse< seq< identifier, eol, identifier, one< ' ' >, must< digit > > >( in );
      }
      catch( const parse_error< text_position_with_source >& e ) {
         TAO_PEGTL_TEST_ASSERT( e.what() == "test_source@2:5(8): parse error matching " + rulename );

         TAO_PEGTL_TEST_ASSERT( e.message() == "parse error matching " + rulename );

         const auto& p = e.position_object();

         TAO_PEGTL_TEST_ASSERT( p.count == 8 );
         TAO_PEGTL_TEST_ASSERT( p.line == 2 );
         TAO_PEGTL_TEST_ASSERT( p.column == 5 );
         TAO_PEGTL_TEST_ASSERT( p.source == "test_source" );

         if constexpr( internal::input_traits< ParseInput >::is_text ) {
            TAO_PEGTL_TEST_ASSERT( line_view_at( in, p ) == "bar bla blubb" );
         }
         return;
      }
      TAO_PEGTL_TEST_UNREACHABLE;  // LCOV_EXCL_LINE
   }

   void unit_test()
   {
      unit_test< lazy_view_input< ascii::lf, char, std::string > >();
      unit_test< text_view_input< ascii::lf, char, std::string > >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
