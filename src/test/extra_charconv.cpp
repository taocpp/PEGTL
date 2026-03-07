// Copyright (c) 2025-2026 Dr. Colin Hirsch and Daniel Frey
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

#include <tao/pegtl/extra/charconv.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   template<>
   struct test_action< everything >
      : from_chars_throws< void >
   {};

   template< typename Integral >
   void from_chars_success( const std::string& input, const Integral output, const std::size_t remaining = 0 )
   {
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = output + 1;
         const bool result = parse< from_chars_nothrow< void > >( in, state );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( state == output );
         TAO_PEGTL_TEST_ASSERT( in.size() == remaining );
      }
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = output + 1;
         const bool result = parse< from_chars_nothrow< Integral > >( in, state );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( state == output );
         TAO_PEGTL_TEST_ASSERT( in.size() == remaining );
      }
      // Test action.
      {
         view_input in( input.data(), input.size() - remaining );
         Integral state = output + 1;
         const bool result = parse< everything, test_action >( in, state );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( state == output );
         TAO_PEGTL_TEST_ASSERT( in.size() == 0 );
      }
   }

   template< typename Integral >
   void from_chars_local_failure( const std::string& input, Integral value )
   {
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = value + 1;
         const bool result = parse< from_chars_throws< void > >( in, state );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( state == value + 1 );
         TAO_PEGTL_TEST_ASSERT( in.size() == input.size() );
      }
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = value + 1;
         const bool result = parse< from_chars_throws< Integral > >( in, state );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( state == value + 1 );
         TAO_PEGTL_TEST_ASSERT( in.size() == input.size() );
      }
      // Test action.
      {
         view_input in( input );
         Integral state = value + 1;
         const bool result = parse< everything, test_action >( in, state );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( state == value + 1 );
         TAO_PEGTL_TEST_ASSERT( in.size() == input.size() );
      }
   }

   template< typename Integral >
   void from_chars_global_failure( const std::string& input, Integral value )
   {
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = value + 1;
         TAO_PEGTL_TEST_THROWS( (void)parse< from_chars_throws< void > >( in, state ) );
      }
      // Test rule.
      {
         view_input in( input );
         static_assert( std::is_integral_v< Integral > );
         Integral state = value + 1;
         TAO_PEGTL_TEST_THROWS( (void)parse< from_chars_throws< Integral > >( in, state ) );
      }
      // Test action.
      {
         view_input in( input );
         Integral state = value + 1;
         TAO_PEGTL_TEST_THROWS( (void)parse< everything, test_action >( in, state ) );
      }
   }

   void unit_test()
   {
      verify_analyze< from_chars_throws< void > >( __LINE__, __FILE__, true, false );
      verify_analyze< from_chars_throws< unsigned > >( __LINE__, __FILE__, true, false );

      verify_analyze< from_chars_nothrow< void > >( __LINE__, __FILE__, true, false );
      verify_analyze< from_chars_nothrow< unsigned > >( __LINE__, __FILE__, true, false );

      from_chars_local_failure( "", int( 1 ) );
      from_chars_local_failure( "", unsigned( 1 ) );

      from_chars_success( "0", int( 0 ) );
      from_chars_success( "0", unsigned( 0 ) );

      from_chars_success( "0 ", int( 0 ), 1 );
      from_chars_success( "0 ", unsigned( 0 ), 1 );

      from_chars_local_failure( " 0", int( 1 ) );
      from_chars_local_failure( " 0", unsigned( 1 ) );

      from_chars_success( "00", int( 0 ) );
      from_chars_success( "00", unsigned( 0 ) );

      from_chars_success( "007", int( 7 ) );
      from_chars_success( "007", unsigned( 7 ) );

      from_chars_success( "0r", int( 0 ), 1 );
      from_chars_success( "0s", unsigned( 0 ), 1 );

      from_chars_success( "0x0", int( 0 ), 2 );
      from_chars_success( "0x0", unsigned( 0 ), 2 );

      from_chars_success( "-1", int( -1 ) );
      from_chars_local_failure( "-1", unsigned( 42 ) );

      from_chars_local_failure( "+1", int( 3 ) );
      from_chars_local_failure( "+1", unsigned( 3 ) );

      from_chars_local_failure( "rrr", int( 0 ) );
      from_chars_local_failure( "sss", unsigned( 0 ) );

      from_chars_global_failure( "999999999999", int( 0 ) );
      from_chars_global_failure( "999999999999", unsigned( 0 ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
