// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/contrib/charconv.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Integral >
   void charconv_success( const std::string& input, const Integral output, const std::size_t remaining = 0 )
   {
      view_input in( input );
      static_assert( std::is_integral_v< Integral > );
      Integral state = output + 1;
      TAO_PEGTL_TEST_ASSERT( state == output + 1 );
      const bool result = parse< charconv >( in, state );
      TAO_PEGTL_TEST_ASSERT( result );
      TAO_PEGTL_TEST_ASSERT( state == output );
      TAO_PEGTL_TEST_ASSERT( in.size() == remaining );
   }

   template< typename Integral >
   void charconv_failure( const std::string& input, const Integral value )
   {
      view_input in( input );
      static_assert( std::is_integral_v< Integral > );
      Integral state = value + 1;
      TAO_PEGTL_TEST_ASSERT( state == value + 1 );
      const bool result = parse< charconv >( in, state );
      TAO_PEGTL_TEST_ASSERT( !result );
      TAO_PEGTL_TEST_ASSERT( state == value + 1 );
      TAO_PEGTL_TEST_ASSERT( in.size() == input.size() );
   }

   void unit_test()
   {
      verify_analyze< charconv >( __LINE__, __FILE__, true, false );

      charconv_success( "0", int( 0 ) );
      charconv_success( "0", unsigned( 0 ) );

      charconv_success( "0a", int( 0 ), 1 );
      charconv_success( "0b", unsigned( 0 ), 1 );

      charconv_success( "0x0", int( 0 ), 2 );
      charconv_success( "0x0", unsigned( 0 ), 2 );

      charconv_success( "-1", int( -1 ) );
      charconv_failure( "-1", unsigned( 42 ) );

      charconv_failure( "999999999999", int( 0 ) );
      charconv_failure( "999999999999", unsigned( 0 ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
