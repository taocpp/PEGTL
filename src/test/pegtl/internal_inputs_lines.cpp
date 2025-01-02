// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "test.hpp"
#include <tao/pegtl/internal/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void test_01_lines()
   {
      using input_t = internal::input_with_lines< scan::lf, internal::input_with_fakes< internal::view_input< char > > >;
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( !parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
   }

   void test_02_lines()
   {
      using input_t = internal::input_with_lines< scan::lf_crlf, internal::input_with_fakes< internal::view_input< char > > >;
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( !parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\n\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( !parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( !parse< eof >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start );
      }
      {
         input_t in( "\r\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eol >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 0 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 2 );
      }
      {
         input_t in( "\r\n" );
         const auto* start = in.current();
         TAO_PEGTL_TEST_ASSERT( in.size() == 2 );
         TAO_PEGTL_TEST_ASSERT( parse< eolf >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 0 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 2 );
      }
   }

   void unit_test()
   {
      test_01_lines();
      test_02_lines();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
