// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/debug/coverage.hpp>
#include <tao/pegtl/debug/print_coverage.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   [[nodiscard]] inline bool operator==( const coverage_info& l, const coverage_info& r ) noexcept
   {
      return ( l.start == r.start ) && ( l.success == r.success ) && ( l.failure == r.failure ) && ( l.unwind == r.unwind ) && ( l.raise == r.raise );
   }

   template< typename Rule >
   [[nodiscard]] bool equals( const coverage_result& result, const coverage_info& i )
   {
      const coverage_entry m = result.coverage.at( demangle< Rule >() );  // Slice
      return i == m;
   }

#if defined( __cpp_exceptions )
   using grammar = seq< sor< try_catch_return_false< must< one< 'a' > > >, seq< at< one< 'F' > >, one< 'F' > >, digit >, eof >;

   void unit_test()
   {
      const std::string data = "F";
      coverage_result result;
      text_view_input< scan::lf > in( data );
      const bool success = coverage< grammar >( in, result );
      std::cout << result;  // To manually see that printing does the right thing, too.
      TAO_PEGTL_TEST_ASSERT( success );
      TAO_PEGTL_TEST_ASSERT( result.coverage.size() == 10 );
      TAO_PEGTL_TEST_ASSERT( equals< grammar >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< one< 'a' > >( result, coverage_info{ 1, 0, 1, 0, 1 } ) );  // TODO: Should this really be counted as both failure and raise?
      TAO_PEGTL_TEST_ASSERT( equals< one< 'F' > >( result, coverage_info{ 2, 2, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< at< one< 'F' > > >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< eof >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< try_catch_return_false< must< one< 'a' > > > >( result, coverage_info{ 1, 0, 1, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< must< one< 'a' > > >( result, coverage_info{ 1, 0, 0, 1, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< digit >( result, coverage_info{ 0, 0, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< sor< try_catch_return_false< must< one< 'a' > > >, seq< at< one< 'F' > >, one< 'F' > >, digit > >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
   }
#else
   using grammar = seq< sor< one< 'a' >, one< 'F' > >, eof >;

   void unit_test()
   {
      const std::string data = "F";
      coverage_result result;
      text_view_input< scan::lf > in( data );
      const bool success = coverage< grammar >( in, result );
      std::cout << result;  // To manually see that printing does the right thing, too.
      TAO_PEGTL_TEST_ASSERT( success );
      TAO_PEGTL_TEST_ASSERT( result.coverage.size() == 5 );
      TAO_PEGTL_TEST_ASSERT( equals< grammar >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< one< 'a' > >( result, coverage_info{ 1, 0, 1, 0, 0 } ) );  // TODO: Should this really be counted as both failure and raise?
      TAO_PEGTL_TEST_ASSERT( equals< one< 'F' > >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< eof >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
      TAO_PEGTL_TEST_ASSERT( equals< sor< one< 'a' >, one< 'F' > > >( result, coverage_info{ 1, 1, 0, 0, 0 } ) );
   }
#endif
}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
