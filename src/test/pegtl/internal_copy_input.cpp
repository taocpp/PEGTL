// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstring>
#include <vector>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const char* data = "hallo";
      {
         internal::copy_input< std::string > in( "" );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.end() );
         in.private_set_current( data );
         TAO_PEGTL_TEST_ASSERT( in.current() == data );
      } {
         const std::string s = "hallo";
         internal::copy_input< std::string > in( s );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == in.start() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == in.start() + s.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == s.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), s.data(), s.size() ) == 0 );
      } {
         const std::string_view sv = "hallo";
         internal::copy_input< std::string > in( sv.data(), sv.size() );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == in.start() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == in.start() + sv.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == sv.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), sv.data(), sv.size() ) == 0 );
      } {
         const std::array< char, 5 > a = { 'h', 'a', 'l', 'l', 'o' };
         internal::copy_input< std::string > in( a );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == in.start() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == in.start() + a.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == a.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), a.data(), a.size() ) == 0 );
      } {
         internal::copy_input< std::string > in( data, std::strlen( data ) );
         in.consume< int >( 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 2 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 3 );
         const auto r = in.rewind_position();
         in.consume< int >( 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 4 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.previous( r ) == in.start() + 2 );
         TAO_PEGTL_TEST_ASSERT( in.start() + in.previous_position( r ).count == r.data );
         in.rewind_to_position( r );
         TAO_PEGTL_TEST_ASSERT( in.start() + in.previous_position( r ).count == r.data );
         TAO_PEGTL_TEST_ASSERT( in.start() + in.current_position().count == r.data );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 2 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 3 );
         {
            const auto m = in.make_rewind_guard< rewind_mode::optional >();
            TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 2 );
            in.consume< std::string >( 1 );
            TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 3 );
            (void)m( false );
         }
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 3 );
         {
            auto m = in.make_rewind_guard< rewind_mode::required >();
            TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 3 );
            in.consume< std::string >( 1 );
            TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 4 );
            (void)m( false );
         }
         TAO_PEGTL_TEST_ASSERT( in.current() == in.start() + 3 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
