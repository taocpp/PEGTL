// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/char_eol_scan.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void char_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::char_eol_scan< '\n' >( pos, str.data(), str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      char_test( "", 1, 1, 0 );
      char_test( " ", 1, 2, 1 );
      char_test( "\r", 1, 2, 1 );
      char_test( "     ", 1, 6, 5 );
      char_test( "\n", 2, 1, 1 );
      char_test( " \n", 2, 1, 2 );
      char_test( "   \n", 2, 1, 4 );
      char_test( "\n ", 2, 2, 2 );
      char_test( "   \n ", 2, 2, 5 );
      char_test( "   \n   ", 2, 4, 7 );
      char_test( "\n\n", 3, 1, 2 );
      char_test( "\n \n", 3, 1, 3 );
      char_test( " \n \n", 3, 1, 4 );
      char_test( " \n \n ", 3, 2, 5 );
      char_test( "   \n\n\n   \n\n\n", 7, 1, 12 );
      char_test( "   \n\n\n   \n\n\n   ", 7, 4, 15 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
