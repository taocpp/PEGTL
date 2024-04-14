// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstring>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const char* data = "hallo";
      {
         internal::view_input< char > in( "" );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() != data );
         TAO_PEGTL_TEST_ASSERT( in.current() == in.end() );
         in.private_set_current( data );
         TAO_PEGTL_TEST_ASSERT( in.current() == data );
         TAO_PEGTL_TEST_ASSERT( in.end() != data );
         in.private_set_end( data );
         TAO_PEGTL_TEST_ASSERT( in.end() == data );
      } {
         const std::string s = "hallo";
         internal::view_input< char > in( s );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == s.data() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == s.data() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == s.data() + s.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == s.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), s.data(), s.size() ) == 0 );
      } {
         const std::string_view sv = "hallo";
         internal::view_input< char > in( sv );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == sv.data() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == sv.data() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == sv.data() + sv.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == sv.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), sv.data(), sv.size() ) == 0 );
      } {
         const std::vector< char > v = { 'h', 'a', 'l', 'l', 'o' };
         internal::view_input< char > in( v );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == v.data() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == v.data() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == v.data() + v.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == v.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), v.data(), v.size() ) == 0 );
      } {
         const std::array< char, 5 > a = { 'h', 'a', 'l', 'l', 'o' };
         internal::view_input< char > in( a );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == a.data() );
         TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == a.data() + 3 );
         TAO_PEGTL_TEST_ASSERT( in.end() == a.data() + a.size() );
         TAO_PEGTL_TEST_ASSERT( in.size() == a.size() );
         TAO_PEGTL_TEST_ASSERT( std::memcmp( in.current(), a.data(), a.size() ) == 0 );
      } {
         internal::view_input< char > in( data, std::strlen( data ) );
         in.consume< int >( 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == data + 2 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 3 );
         const auto r = in.rewind_position();
         in.consume< int >( 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == data + 4 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.previous( r ) == data + 2 );
         TAO_PEGTL_TEST_ASSERT( in.previous_position( r ) == r );
         in.rewind_to_position( r );
         TAO_PEGTL_TEST_ASSERT( in.previous_position( r ) == r );
         TAO_PEGTL_TEST_ASSERT( in.current_position() == r );
         TAO_PEGTL_TEST_ASSERT( in.current() == data + 2 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 3 );
         {
            const auto p = in.rewind_position();
            (void)p;
            TAO_PEGTL_TEST_ASSERT( in.current() == data + 2 );
            in.consume< std::string >( 1 );
            TAO_PEGTL_TEST_ASSERT( in.current() == data + 3 );
         }
         TAO_PEGTL_TEST_ASSERT( in.current() == data + 3 );
         {
            const auto p = in.rewind_position();
            TAO_PEGTL_TEST_ASSERT( in.current() == data + 3 );
            in.consume< std::string >( 1 );
            TAO_PEGTL_TEST_ASSERT( in.current() == data + 4 );
            in.rewind_to_position( p );
         }
         TAO_PEGTL_TEST_ASSERT( in.current() == data + 3 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
