// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>

#include <tao/pegtl/internal/data_and_size.hpp>
#include <tao/pegtl/internal/dependent_false.hpp>
#include <tao/pegtl/internal/lazy_scan_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol >
   void lazy_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::scan_input< char > in( str.data(), str.size() );
      internal::lazy_scan_traits< typename Eol::rule_t, internal::peek_seven >::scan( pos, in );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      lazy_test< ascii::lf >( "", 1, 1, 0 );
      lazy_test< ascii::lf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf >( "\r", 1, 2, 1 );
      lazy_test< ascii::lf >( "     ", 1, 6, 5 );
      lazy_test< ascii::lf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf >( " \n", 2, 1, 2 );
      lazy_test< ascii::lf >( "   \n", 2, 1, 4 );
      lazy_test< ascii::lf >( "\n ", 2, 2, 2 );
      lazy_test< ascii::lf >( "   \n ", 2, 2, 5 );
      lazy_test< ascii::lf >( "   \n   ", 2, 4, 7 );
      lazy_test< ascii::lf >( "\n\n", 3, 1, 2 );
      lazy_test< ascii::lf >( "\n \n", 3, 1, 3 );
      lazy_test< ascii::lf >( " \n \n", 3, 1, 4 );
      lazy_test< ascii::lf >( " \n \n ", 3, 2, 5 );
      lazy_test< ascii::lf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< ascii::lf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< ascii::crlf >( "", 1, 1, 0 );
      lazy_test< ascii::crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::crlf >( "  ", 1, 3, 2 );
      lazy_test< ascii::crlf >( "\r", 1, 2, 1 );
      lazy_test< ascii::crlf >( "\n", 1, 2, 1 );
      lazy_test< ascii::crlf >( "\n\r", 1, 3, 2 );
      lazy_test< ascii::crlf >( "\r\n", 2, 1, 2 );
      lazy_test< ascii::crlf >( "  \r\n  ", 2, 3, 6 );

      lazy_test< ascii::lf_crlf >( "", 1, 1, 0 );
      lazy_test< ascii::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "     ", 1, 6, 5 );
      lazy_test< ascii::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf_crlf >( " \n", 2, 1, 2 );
      lazy_test< ascii::lf_crlf >( "   \n", 2, 1, 4 );
      lazy_test< ascii::lf_crlf >( "\n ", 2, 2, 2 );
      lazy_test< ascii::lf_crlf >( "   \n ", 2, 2, 5 );
      lazy_test< ascii::lf_crlf >( "   \n   ", 2, 4, 7 );
      lazy_test< ascii::lf_crlf >( "\n\n", 3, 1, 2 );
      lazy_test< ascii::lf_crlf >( "\n \n", 3, 1, 3 );
      lazy_test< ascii::lf_crlf >( " \n \n", 3, 1, 4 );
      lazy_test< ascii::lf_crlf >( " \n \n ", 3, 2, 5 );
      lazy_test< ascii::lf_crlf >( "   \n\n\n   \n\n\n", 7, 1, 12 );
      lazy_test< ascii::lf_crlf >( "   \n\n\n   \n\n\n   ", 7, 4, 15 );

      lazy_test< ascii::lf_crlf >( "", 1, 1, 0 );
      lazy_test< ascii::lf_crlf >( " ", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "  ", 1, 3, 2 );
      lazy_test< ascii::lf_crlf >( "\r", 1, 2, 1 );
      lazy_test< ascii::lf_crlf >( "\n", 2, 1, 1 );
      lazy_test< ascii::lf_crlf >( "\n\r", 2, 2, 2 );
      lazy_test< ascii::lf_crlf >( "\r\n", 2, 1, 2 );
      lazy_test< ascii::lf_crlf >( "  \r\n  ", 2, 3, 6 );

#if defined( __cpp_exceptions )
      const auto c = char( 0x80 );
      TAO_PEGTL_TEST_THROWS( lazy_test< ascii::lf_crlf >( std::string( 1, c ), 0, 0, 0 ) );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
