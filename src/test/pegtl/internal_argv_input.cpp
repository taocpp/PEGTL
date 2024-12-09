// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstring>

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   char s1[ 4 ];
   char s2[ 11 ];
   char* argv[ 3 ] = { s1, s2, nullptr };

   void unit_test()
   {
      std::memcpy( s1, "app", 3 );
      std::memcpy( s2, "frobnicate", 10 );
      {
         internal::argv_input in( argv, 1 );
         in.consume< any >( 5 );
         const auto p = in.current_position();
         const char* b = in.begin_of_line( p );
         const char* e = in.end_of_line_or_file( p );
         TAO_PEGTL_TEST_ASSERT( b + 10 == e );
         TAO_PEGTL_TEST_ASSERT( b == argv[ 1 ] );
      }
      {
         internal::argv_input_with_source< void > in( argv, 1 );
         in.consume< any >( 5 );
         const auto p = in.current_position();
         const char* b = in.begin_of_line( p );
         const char* e = in.end_of_line_or_file( p );
         TAO_PEGTL_TEST_ASSERT( b + 10 == e );
         TAO_PEGTL_TEST_ASSERT( b == argv[ 1 ] );
      }
      {
         internal::argv_input_with_source< std::string > in( argv, 1 );
         in.consume< any >( 5 );
         const auto p = in.current_position();
         const char* b = in.begin_of_line( p );
         const char* e = in.end_of_line_or_file( p );
         TAO_PEGTL_TEST_ASSERT( b + 10 == e );
         TAO_PEGTL_TEST_ASSERT( b == argv[ 1 ] );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
