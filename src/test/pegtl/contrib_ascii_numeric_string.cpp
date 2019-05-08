// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

#include <tao/pegtl/contrib/ascii_numeric_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< auto M, bool E >
   void test( const char* v )
   {
      memory_input in( v, "" );
      const auto result = parse< seq< one< 'a' >, numeric_string< M >, one< 'b' >, eof > >( in );
      TAO_PEGTL_TEST_ASSERT( result == E );
   }

   void unit_test()
   {
      test< 0, true >( "a0b" );
      test< 0, false >( "ab" );
      test< 0, false >( "a1b" );
      test< 0, false >( "a9b" );

      test< 1, true >( "a0b" );
      test< 1, true >( "a1b" );
      test< 1, false >( "ab" );
      test< 1, false >( "a2b" );
      test< 1, false >( "a9b" );

      test< 12, true >( "a0b" );
      test< 12, true >( "a1b" );
      test< 12, true >( "a9b" );
      test< 12, true >( "a10b" );
      test< 12, true >( "a11b" );
      test< 12, true >( "a12b" );
      test< 12, false >( "a13b" );
      test< 12, false >( "a120b" );
      test< 12, false >( "a111b" );
      test< 12, false >( "a00b" );
      test< 12, false >( "a01b" );

      test< 100, true >( "a0b" );
      test< 100, true >( "a1b" );
      test< 100, true >( "a10b" );
      test< 100, true >( "a99b" );
      test< 100, true >( "a100b" );
      test< 100, false >( "a101b" );

      test< 18446744073709551614ULL, true >( "a18446744073709551614b" );
      test< 18446744073709551614ULL, false >( "a18446744073709551615b" );

      test< 18446744073709551615ULL, true >( "a18446744073709551615b" );
      test< 18446744073709551615ULL, false >( "a18446744073709551616b" );
      test< 18446744073709551615ULL, false >( "a98446744073709551614b" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
