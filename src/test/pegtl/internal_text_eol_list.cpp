// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/text_eol_meta.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol, typename Rule >
   void list_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::text_eol_list< Eol, Rule >( pos, str.data(), str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      list_test< eols::list::lf_crlf, ascii::ff >( "", 1, 1, 0 );
      list_test< eols::list::lf_crlf, ascii::ff >( " ", 1, 2, 1 );
      list_test< eols::list::lf_crlf, ascii::ff >( "\n", 1, 2, 1 );

      list_test< eols::list::lf_crlf, ascii::lf >( "", 2, 1, 0 );
      list_test< eols::list::lf_crlf, ascii::lf >( " ", 2, 1, 1 );
      list_test< eols::list::lf_crlf, ascii::lf >( "\n", 2, 1, 1 );

      list_test< eols::list::lf_crlf, ascii::crlf >( "", 2, 1, 0 );
      list_test< eols::list::lf_crlf, ascii::crlf >( " ", 2, 1, 1 );
      list_test< eols::list::lf_crlf, ascii::crlf >( "\n", 2, 1, 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
