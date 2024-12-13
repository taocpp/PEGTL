// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename ParseInput >
   void main_test( ParseInput& in )
   {
      in.require( 10 );
      const auto m = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.previous( m ) == in.current() );
      in.consume< any >( 6 );
      TAO_PEGTL_TEST_ASSERT( in.previous( m ) + 6 == in.current() );
      in.rewind_to_position( m );
      TAO_PEGTL_TEST_ASSERT( in.previous( m ) == in.current() );
      TAO_PEGTL_TEST_ASSERT( &in.current_position() == &in.direct_position() );
      TAO_PEGTL_TEST_ASSERT( in.direct_count() == in.current_position().count );
      TAO_PEGTL_TEST_ASSERT( in.direct_line() == in.current_position().line );
      TAO_PEGTL_TEST_ASSERT( in.direct_column() == in.current_position().column );
      in.consume< alpha >( 5 );
      const auto n = in.rewind_position();
      in.consume< eol >( 1 );
      TAO_PEGTL_TEST_ASSERT( in.direct_count() == 6 );
      TAO_PEGTL_TEST_ASSERT( in.direct_line() == 2 );
      TAO_PEGTL_TEST_ASSERT( in.direct_column() == 1 );
      const auto p = in.previous_position( n );
      TAO_PEGTL_TEST_ASSERT( p.count == 5 );
      TAO_PEGTL_TEST_ASSERT( p.line == 1 );
      TAO_PEGTL_TEST_ASSERT( p.column == 6 );
   }

   void unit_test()
   {
      dynamic_text_cstring_input<> i1( 50, 10, "input\ndata" );
      main_test( i1 );
      dynamic_text_cstring_input< tao_buffer_eol, std::string > i2( "source", 50, 10, "input\ndata" );
      main_test( i2 );
      TAO_PEGTL_TEST_ASSERT( i2.direct_source() == "source" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
