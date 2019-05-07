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
      const auto result = parse< seq< numeric_string< M >, eof > >( in );
      TAO_PEGTL_TEST_ASSERT( result == E );
   }

   void unit_test()
   {
      test< 0, true >( "0" );
      test< 0, false >( "1" );
      test< 0, false >( "9" );

      test< 1, true >( "0" );
      test< 1, true >( "1" );
      test< 1, false >( "2" );
      test< 1, false >( "9" );

      test< 12, true >( "0" );
      test< 12, true >( "1" );
      test< 12, true >( "9" );
      test< 12, true >( "10" );
      test< 12, true >( "11" );
      test< 12, true >( "12" );
      test< 12, false >( "13" );
      test< 12, false >( "120" );
      test< 12, false >( "111" );
      test< 12, false >( "00" );
      test< 12, false >( "01" );

      test< 100, true >( "0" );
      test< 100, true >( "1" );
      test< 100, true >( "10" );
      test< 100, true >( "99" );
      test< 100, true >( "100" );
      test< 100, false >( "101" );

      test< 18446744073709551614ULL, true >( "18446744073709551614" );
      test< 18446744073709551614ULL, false >( "18446744073709551615" );

      test< 18446744073709551615ULL, true >( "18446744073709551615" );
      test< 18446744073709551615ULL, false >( "18446744073709551616" );
      test< 18446744073709551615ULL, false >( "98446744073709551614" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
