// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/text_position.hpp>
#include <tao/pegtl/internal/text_rule_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol, typename Rule >
   void rule_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::lazy_scan_input< char > in( str.data(), str.size() );
      internal::text_rule_traits< Eol, Rule >::scan( pos, in );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      rule_test< ascii::lf, eol >( "", 1, 1, 0 );
      rule_test< ascii::lf, eol >( " ", 1, 2, 1 );
      rule_test< ascii::lf, eol >( "\n", 1, 2, 1 );

      rule_test< ascii::lf, eolf >( "", 1, 1, 0 );
      rule_test< ascii::lf, eolf >( " ", 1, 2, 1 );
      rule_test< ascii::lf, eolf >( "\n", 1, 2, 1 );

      rule_test< ascii::lf, ascii::any >( "", 1, 1, 0 );
      rule_test< ascii::lf, ascii::any >( " ", 1, 2, 1 );
      rule_test< ascii::lf, ascii::any >( "\n", 1, 2, 1 );

      rule_test< ascii::lf, ascii::many< 3 > >( "xyz", 1, 4, 3 );
      rule_test< ascii::lf, ascii::many< 30 > >( "xyz", 1, 4, 3 );
      rule_test< ascii::lf, ascii::many< 3 > >( "\n\n\n", 1, 4, 3 );

      rule_test< ascii::lf, eol_exclude_tag >( "", 1, 1, 0 );
      rule_test< ascii::lf, eol_exclude_tag >( " ", 1, 2, 1 );
      rule_test< ascii::lf, eol_exclude_tag >( "\n", 1, 2, 1 );

      rule_test< ascii::lf, eol_unknown_tag >( "", 1, 1, 0 );
      rule_test< ascii::lf, eol_unknown_tag >( " ", 1, 2, 1 );
      rule_test< ascii::lf, eol_unknown_tag >( "\n", 1, 2, 1 );

      rule_test< ascii::lf, eol_matched_tag >( "", 2, 1, 0 );
      rule_test< ascii::lf, eol_matched_tag >( " ", 2, 1, 1 );
      rule_test< ascii::lf, eol_matched_tag >( "\n", 2, 1, 1 );
      rule_test< ascii::lf, eol_matched_tag >( "\r\n", 2, 1, 2 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
