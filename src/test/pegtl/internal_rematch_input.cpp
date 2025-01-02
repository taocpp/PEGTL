// Copyright (c) 2024-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_file.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      internal::read_input in( "src/test/pegtl/data/duseltronik.txt" );
      in.consume< any >( 1 );
      internal::rewind_guard< internal::read_input > rg( in );
      in.consume< any >( 4 );
      internal::rematch_input ri( rg, in );
      TAO_PEGTL_TEST_ASSERT( !ri.empty() );
      TAO_PEGTL_TEST_ASSERT( ri.size() == 4 );
      TAO_PEGTL_TEST_ASSERT( ri.start() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.start() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.current() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.current() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.end() == in.current() );
      const auto pr = ri.rewind_position();
      ri.consume< any >( 1 );
      TAO_PEGTL_TEST_ASSERT( !ri.empty() );
      TAO_PEGTL_TEST_ASSERT( ri.size() == 3 );
      TAO_PEGTL_TEST_ASSERT( ri.start() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.start() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.current() == in.start() + 2 );
      TAO_PEGTL_TEST_ASSERT( ri.current() + 3 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.previous( pr ) == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.previous( pr ) + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.end() == in.current() );
      ri.restart();
      TAO_PEGTL_TEST_ASSERT( !ri.empty() );
      TAO_PEGTL_TEST_ASSERT( ri.size() == 4 );
      TAO_PEGTL_TEST_ASSERT( ri.start() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.start() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.current() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.current() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.end() == in.current() );
      ri.private_set_current( ri.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( !ri.empty() );
      TAO_PEGTL_TEST_ASSERT( ri.size() == 3 );
      TAO_PEGTL_TEST_ASSERT( ri.start() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.start() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.current() == in.start() + 2 );
      TAO_PEGTL_TEST_ASSERT( ri.current() + 3 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.previous( pr ) == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.previous( pr ) + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.end() == in.current() );
      ri.rewind_to_position( pr );
      TAO_PEGTL_TEST_ASSERT( !ri.empty() );
      TAO_PEGTL_TEST_ASSERT( ri.size() == 4 );
      TAO_PEGTL_TEST_ASSERT( ri.start() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.start() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.current() == in.start() + 1 );
      TAO_PEGTL_TEST_ASSERT( ri.current() + 4 == in.current() );
      TAO_PEGTL_TEST_ASSERT( ri.end() == in.current() );
      ri.restart();
      ri.consume< any >( 1 );
      const auto p1 = ri.current_position();
      TAO_PEGTL_TEST_ASSERT( p1.count == 2 );
      const auto p2 = ri.previous_position( pr );
      TAO_PEGTL_TEST_ASSERT( p2.count == 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
