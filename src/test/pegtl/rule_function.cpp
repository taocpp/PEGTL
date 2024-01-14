// Copyright (c) 2020-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/internal/peek_ascii7.hpp>
#include <tao/pegtl/internal/peek_ascii8.hpp>
#include <tao/pegtl/internal/peek_current.hpp>
#include <tao/pegtl/internal/peek_direct.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   static bool called = false;

   template< auto F, typename P = void >
   void unit_test_1()
   {
      int i = 42;
      char c = 'a';
      double d = 42.0;
      called = false;
      text_view_input< eols::scan::lf > in( "f" );
      TAO_PEGTL_TEST_ASSERT( parse< seq< function< F, P >, eof > >( in, i, &c, d ) );
      TAO_PEGTL_TEST_ASSERT( called );
   }

   [[nodiscard]] bool func0( text_view_input< eols::scan::lf >& in )
   {
      called = true;
      in.consume< eol_exclude_tag >( 1 );
      return true;
   }

   [[nodiscard]] bool func0n( text_view_input< eols::scan::lf >& in ) noexcept
   {
      called = true;
      in.consume< eol_exclude_tag >( 1 );
      return true;
   }

   [[nodiscard]] bool func1( text_view_input< eols::scan::lf >& in, int /*unused*/, char*& /*unused*/, const double& /*unused*/ )
   {
      called = true;
      in.consume< eol_exclude_tag >( 1 );
      return true;
   }

   [[nodiscard]] bool func1n( text_view_input< eols::scan::lf >& in, int /*unused*/, char*& /*unused*/, const double& /*unused*/ ) noexcept
   {
      called = true;
      in.consume< eol_exclude_tag >( 1 );
      return true;
   }

   [[nodiscard]] bool func2( const char /*unused*/, int /*unused*/, char*& /*unused*/, const double& /*unused*/ )
   {
      called = true;
      return true;
   }

   [[nodiscard]] bool func2n( const char /*unused*/, int /*unused*/, char*& /*unused*/, const double& /*unused*/ ) noexcept
   {
      called = true;
      return true;
   }

   [[nodiscard]] bool func3( const char /*unused*/ )
   {
      called = true;
      return true;
   }

   [[nodiscard]] bool func3n( const char /*unused*/ ) noexcept
   {
      called = true;
      return true;
   }

   void unit_test()
   {
      unit_test_1< func0 >();
      unit_test_1< func1 >();
      unit_test_1< func2, internal::peek_char >();
      unit_test_1< func2, internal::peek_ascii7 >();
      unit_test_1< func2, internal::peek_ascii8 >();
      unit_test_1< func2, internal::peek_current >();
      unit_test_1< func3, internal::peek_char >();
      unit_test_1< func3, internal::peek_ascii7 >();
      unit_test_1< func3, internal::peek_ascii8 >();
      unit_test_1< func3, internal::peek_current >();
      unit_test_1< func0n >();
      unit_test_1< func1n >();
      unit_test_1< func2n, internal::peek_char >();
      unit_test_1< func2n, internal::peek_ascii7 >();
      unit_test_1< func2n, internal::peek_ascii8 >();
      unit_test_1< func2n, internal::peek_current >();
      unit_test_1< func3n, internal::peek_char >();
      unit_test_1< func3n, internal::peek_ascii7 >();
      unit_test_1< func3n, internal::peek_ascii8 >();
      unit_test_1< func3n, internal::peek_current >();
      // unit_test_1< decltype( TODO: An appropriate lambda as soon as we switch to C++20 ) ...
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
