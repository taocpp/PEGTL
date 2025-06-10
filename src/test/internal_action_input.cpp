// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      view_input< scan::lf > in( "hallo" );
      in.consume< eol_exclude_tag >( 1 );
      {
         const auto p = in.rewind_position();
         in.consume< eol_exclude_tag >( 3 );
         internal::action_input_impl< view_input< scan::lf > > ai( p, in );
         TAO_PEGTL_TEST_ASSERT( ai.begin() == ai.current() );
         TAO_PEGTL_TEST_ASSERT( ai.begin() + 2 == ai.current( 2 ) );
         TAO_PEGTL_TEST_ASSERT( ai.begin() + 3 == ai.end() );
         TAO_PEGTL_TEST_ASSERT( !ai.empty() );
         TAO_PEGTL_TEST_ASSERT( ai.size() == 3 );
         TAO_PEGTL_TEST_ASSERT( &ai.input() == &in );
         TAO_PEGTL_TEST_ASSERT( ai.current_position() == in.previous_position( p ) );
         TAO_PEGTL_TEST_ASSERT( ai.rewind_position() == p );
         TAO_PEGTL_TEST_ASSERT( ai.begin() + 3 == in.current() );
      }
      {
         const auto p = in.rewind_position();
         internal::action_input_impl< view_input< scan::lf > > ai( p, in );
         TAO_PEGTL_TEST_ASSERT( ai.empty() );
         TAO_PEGTL_TEST_ASSERT( ai.size() == 0 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
