// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/text_eol_scan.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol, typename Rule >
   void scan_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::text_eol_scan< Eol, Rule >( pos, str.data(), str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      scan_test< eols::scan::lf, eol_unknown_tag >( "", 1, 1, 0 );
      scan_test< eols::scan::lf, eol_unknown_tag >( " ", 1, 2, 1 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "     ", 1, 6, 5 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "\n", 2, 1, 1 );
      scan_test< eols::scan::lf, eol_unknown_tag >( " \n", 2, 1, 2 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "   \n", 2, 1, 4 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "\n ", 2, 2, 2 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "   \n ", 2, 2, 5 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "   \n   ", 2, 4, 7 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "\n\n", 3, 1, 2 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "\n \n", 3, 1, 3 );
      scan_test< eols::scan::lf, eol_unknown_tag >( " \n \n", 3, 1, 4 );
      scan_test< eols::scan::lf, eol_unknown_tag >( " \n \n ", 3, 2, 5 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      scan_test< eols::scan::lf, eol_unknown_tag >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "", 1, 1, 0 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( " ", 1, 2, 1 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "     ", 1, 6, 5 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n", 2, 1, 1 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( " \n", 2, 1, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "   \n", 2, 1, 4 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n ", 2, 2, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "   \n ", 2, 2, 5 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "   \n   ", 2, 4, 7 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n\n", 3, 1, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n \n", 3, 1, 3 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( " \n \n", 3, 1, 4 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( " \n \n ", 3, 2, 5 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "", 1, 1, 0 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( " ", 1, 2, 1 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "  ", 1, 3, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\r", 1, 2, 1 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n", 2, 1, 1 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\n\r", 2, 2, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "\r\n", 2, 1, 2 );
      scan_test< eols::scan::lf_crlf, eol_unknown_tag >( "  \r\n  ", 2, 3, 6 );

      // TODO...
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
