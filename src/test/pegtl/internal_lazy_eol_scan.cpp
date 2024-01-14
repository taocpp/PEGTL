// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/lazy_eol_scan.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol >
   void lazy_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::lazy_eol_scan< Eol >( pos, str.data(), str.data() + str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      lazy_test< eols::rule::lf >( "", 1, 1, 0 );
      lazy_test< eols::rule::lf >( " ", 1, 2, 1 );
      lazy_test< eols::rule::lf >( "     ", 1, 6, 5 );
      lazy_test< eols::rule::lf >( "\n", 2, 1, 1 );
      lazy_test< eols::rule::lf >( " \n", 2, 1, 2 );
      lazy_test< eols::rule::lf >( "   \n", 2, 1, 4 );
      lazy_test< eols::rule::lf >( "\n ", 2, 2, 2 );
      lazy_test< eols::rule::lf >( "   \n ", 2, 2, 5 );
      lazy_test< eols::rule::lf >( "   \n   ", 2, 4, 7 );
      lazy_test< eols::rule::lf >( "\n\n", 3, 1, 2 );
      lazy_test< eols::rule::lf >( "\n \n", 3, 1, 3 );
      lazy_test< eols::rule::lf >( " \n \n", 3, 1, 4 );
      lazy_test< eols::rule::lf >( " \n \n ", 3, 2, 5 );
      lazy_test< eols::rule::lf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< eols::rule::lf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< eols::rule::crlf >( "", 1, 1, 0 );
      lazy_test< eols::rule::crlf >( " ", 1, 2, 1 );
      lazy_test< eols::rule::crlf >( "  ", 1, 3, 2 );
      lazy_test< eols::rule::crlf >( "\r", 1, 2, 1 );
      lazy_test< eols::rule::crlf >( "\n", 1, 2, 1 );
      lazy_test< eols::rule::crlf >( "\n\r", 1, 3, 2 );
      lazy_test< eols::rule::crlf >( "\r\n", 2, 1, 2 );
      lazy_test< eols::rule::crlf >( "  \r\n  ", 2, 3, 6 );

      lazy_test< eols::rule::lf_crlf >( "", 1, 1, 0 );
      lazy_test< eols::rule::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< eols::rule::lf_crlf >( "     ", 1, 6, 5 );
      lazy_test< eols::rule::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< eols::rule::lf_crlf >( " \n", 2, 1, 2 );
      lazy_test< eols::rule::lf_crlf >( "   \n", 2, 1, 4 );
      lazy_test< eols::rule::lf_crlf >( "\n ", 2, 2, 2 );
      lazy_test< eols::rule::lf_crlf >( "   \n ", 2, 2, 5 );
      lazy_test< eols::rule::lf_crlf >( "   \n   ", 2, 4, 7 );
      lazy_test< eols::rule::lf_crlf >( "\n\n", 3, 1, 2 );
      lazy_test< eols::rule::lf_crlf >( "\n \n", 3, 1, 3 );
      lazy_test< eols::rule::lf_crlf >( " \n \n", 3, 1, 4 );
      lazy_test< eols::rule::lf_crlf >( " \n \n ", 3, 2, 5 );
      lazy_test< eols::rule::lf_crlf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< eols::rule::lf_crlf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< eols::rule::lf_crlf >( "", 1, 1, 0 );
      lazy_test< eols::rule::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< eols::rule::lf_crlf >( "  ", 1, 3, 2 );
      lazy_test< eols::rule::lf_crlf >( "\r", 1, 2, 1 );
      lazy_test< eols::rule::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< eols::rule::lf_crlf >( "\n\r", 2, 2, 2 );
      lazy_test< eols::rule::lf_crlf >( "\r\n", 2, 1, 2 );
      lazy_test< eols::rule::lf_crlf >( "  \r\n  ", 2, 3, 6 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
