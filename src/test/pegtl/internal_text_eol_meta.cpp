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
   void meta_test( const std::string& str, const std::size_t line, const std::size_t column, const std::size_t count )
   {
      text_position pos;
      internal::text_eol_meta< Eol, Rule >( pos, str.data(), str.size() );
      TAO_PEGTL_TEST_ASSERT( test::equal( pos, line, column, count ) );
   }

   void unit_test()
   {
      meta_test< eols::meta::lf, eol >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, eol >( " ", 1, 2, 1 );
      meta_test< eols::meta::lf, eol >( "\n", 1, 2, 1 );

      meta_test< eols::meta::lf, eolf >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, eolf >( " ", 1, 2, 1 );
      meta_test< eols::meta::lf, eolf >( "\n", 1, 2, 1 );

      meta_test< eols::meta::lf, ascii::lf >( "", 2, 1, 0 );
      meta_test< eols::meta::lf, ascii::lf >( " ", 2, 1, 1 );
      meta_test< eols::meta::lf, ascii::lf >( "\n", 2, 1, 1 );

      meta_test< eols::meta::lf, ascii::space >( "", 2, 1, 0 );
      meta_test< eols::meta::lf, ascii::space >( " ", 2, 1, 1 );
      meta_test< eols::meta::lf, ascii::space >( "\n", 2, 1, 1 );

      meta_test< eols::meta::lf, ascii::any8 >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, ascii::any8 >( " ", 1, 2, 1 );
      meta_test< eols::meta::lf, ascii::any8 >( "\n", 1, 2, 1 );

      meta_test< eols::meta::lf, ascii::many8< 3 > >( "xyz", 1, 4, 3 );
      meta_test< eols::meta::lf, ascii::many8< 30 > >( "xyz", 1, 4, 3 );
      meta_test< eols::meta::lf, ascii::many8< 3 > >( "\n\n\n", 1, 4, 3 );

      meta_test< eols::meta::lf, eol_exclude_tag >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, eol_exclude_tag >( " ", 1, 2, 1 );
      meta_test< eols::meta::lf, eol_exclude_tag >( "\n", 1, 2, 1 );

      meta_test< eols::meta::lf, eol_unknown_tag >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, eol_unknown_tag >( " ", 1, 2, 1 );
      meta_test< eols::meta::lf, eol_unknown_tag >( "\n", 1, 2, 1 );

      meta_test< eols::meta::lf, eol_matched_tag >( "", 1, 1, 0 );
      meta_test< eols::meta::lf, eol_matched_tag >( " ", 1, 1, 0 );
      meta_test< eols::meta::lf, eol_matched_tag >( "\n", 1, 1, 0 );
      meta_test< eols::meta::lf, eol_matched_tag >( "\r\n", 1, 1, 0 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
