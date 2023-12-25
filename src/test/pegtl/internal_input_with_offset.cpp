// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/internal/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   const std::string data = "foo\nbar";

   using rule1 = seq< string< 'f', 'o', 'o' >, eol >;
   using rule2 = seq< string< 'b', 'a', 'r' >, eof >;

   void test_count()
   {
      internal::input_with_offset< false, copy_input< ascii::lf > > in( data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 4 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 7 );
   }

   void test_count_offset()
   {
      internal::input_with_offset< true, copy_input< ascii::lf > > in( count_position( 42 ), data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 42 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 46 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 46 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 49 );
   }

   void test_count_offset_source()
   {
      internal::input_with_offset< true, copy_input< ascii::lf, std::string, const char*, std::string > > in( count_position( 42 ), __FUNCTION__, data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 42 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 46 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 46 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 49 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
   }

   void test_text()
   {
      internal::input_with_offset< false, text_copy_input< ascii::lf > > in( data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 4 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 7 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
   }

   void test_text_offset()
   {
      internal::input_with_offset< true, text_copy_input< ascii::lf > > in( text_position( 100, 200, 300 ), data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 100 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 200 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 300 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 107 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
   }

   void test_text_offset_source()
   {
      internal::input_with_offset< true, text_copy_input< ascii::lf, std::string, const char*, std::string > > in( text_position( 100, 200, 300 ), __FUNCTION__, data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 100 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 200 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 300 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 107 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
   }

   void test_lazy()
   {
      internal::input_with_offset< false, lazy_copy_input< ascii::lf > > in( data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 0 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 4 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 7 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 2 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
   }

   void test_lazy_offset()
   {
      internal::input_with_offset< true, lazy_copy_input< ascii::lf > > in( text_position( 100, 200, 300 ), data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 100 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 200 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 300 );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 107 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
   }

   void test_lazy_offset_source()
   {
      internal::input_with_offset< true, lazy_copy_input< ascii::lf, std::string, const char*, std::string > > in( text_position( 100, 200, 300 ), __FUNCTION__, data );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 100 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 200 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 300 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position() == in.previous_position( in.rewind_position() ) );
      const auto a = parse< rule1 >( in );
      TAO_PEGTL_TEST_ASSERT( a );
      const auto r = in.rewind_position();
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
      const auto b = parse< rule2 >( in );
      TAO_PEGTL_TEST_ASSERT( b );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).count == 104 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).column == 1 );
      TAO_PEGTL_TEST_ASSERT( in.previous_position( r ).source == __FUNCTION__ );
      TAO_PEGTL_TEST_ASSERT( in.current_position().count == 107 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().line == 201 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().column == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current_position().source == __FUNCTION__ );
   }

   void unit_test()
   {
      test_count();
      test_count_offset();
      test_count_offset_source();
      test_text();
      test_text_offset();
      test_text_offset_source();
      test_lazy();
      test_lazy_offset();
      test_lazy_offset_source();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
