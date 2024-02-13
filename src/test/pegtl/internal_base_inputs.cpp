// Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/internal/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   const std::string data_01 = "duseltronik";

   template< typename Input >
   void test_01( Input& in )
   {
      const auto* start = in.current();
      static_assert( std::is_same_v< typename Input::data_t, char > );
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 11 );
      TAO_PEGTL_TEST_ASSERT( std::string( in.current(), in.size() ) == "duseltronik" );
      TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == ( in.current() + 3 ) );
      TAO_PEGTL_TEST_ASSERT( in.end() == in.current() + 11 );
      TAO_PEGTL_TEST_ASSERT( *in.current() == 'd' );
      TAO_PEGTL_TEST_ASSERT( *in.current( 10 ) == 'k' );
      in.template consume< void >( 5 );  // "tronik"
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 6 );
      TAO_PEGTL_TEST_ASSERT( std::string( in.current(), in.size() ) == "tronik" );
      TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == ( in.current() + 3 ) );
      TAO_PEGTL_TEST_ASSERT( in.end() == in.current() + 6 );
      TAO_PEGTL_TEST_ASSERT( *in.current() == 't' );
      TAO_PEGTL_TEST_ASSERT( *in.current( 5 ) == 'k' );
      TAO_PEGTL_TEST_ASSERT( in.current() == start + 5 );
      TAO_PEGTL_TEST_ASSERT( in.previous( in.current_position() ) == in.current() );
      in.private_set_current( start );  // "duseltronik"
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 11 );
      TAO_PEGTL_TEST_ASSERT( std::string( in.current(), in.size() ) == "duseltronik" );
      TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == ( in.current() + 3 ) );
      TAO_PEGTL_TEST_ASSERT( in.end() == in.current() + 11 );
      TAO_PEGTL_TEST_ASSERT( *in.current() == 'd' );
      TAO_PEGTL_TEST_ASSERT( *in.current( 10 ) == 'k' );
      TAO_PEGTL_TEST_ASSERT( in.previous( in.current_position() ) == in.current() );
      {
         auto m = in.template make_rewind_guard< rewind_mode::required >();
         in.template consume< void >( 1 );
         TAO_PEGTL_TEST_ASSERT( in.size() == 10 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 1 );
         std::ignore = m( false );
      }
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 11 );
      TAO_PEGTL_TEST_ASSERT( std::string( in.current(), in.size() ) == "duseltronik" );
      TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == ( in.current() + 3 ) );
      TAO_PEGTL_TEST_ASSERT( in.end() == in.current() + 11 );
      TAO_PEGTL_TEST_ASSERT( *in.current() == 'd' );
      TAO_PEGTL_TEST_ASSERT( *in.current( 10 ) == 'k' );
      {
         auto m = in.template make_rewind_guard< rewind_mode::required >();
         in.template consume< void >( 5 );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.size() == 6 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 5 );
         std::ignore = m( true );
      }
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 6 );
      TAO_PEGTL_TEST_ASSERT( in.current() == start + 5 );
      {
         auto m = in.template make_rewind_guard< rewind_mode::optional >();
         in.template consume< void >( 1 );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.size() == 5 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 6 );
         std::ignore = m( false );
      }
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 5 );
      TAO_PEGTL_TEST_ASSERT( in.current() == start + 6 );
      {
         auto m = in.template make_rewind_guard< rewind_mode::optional >();
         in.template consume< void >( 1 );
         TAO_PEGTL_TEST_ASSERT( !in.empty() );
         TAO_PEGTL_TEST_ASSERT( in.size() == 4 );
         TAO_PEGTL_TEST_ASSERT( in.current() == start + 7 );
         std::ignore = m( true );
      }
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 4 );
      TAO_PEGTL_TEST_ASSERT( in.current() == start + 7 );
      in.private_set_current( start + 1 );  // "useltronik"
      {
         const auto p = in.rewind_position();
         const auto r = test::rewind_adapt( start, p );
         TAO_PEGTL_TEST_ASSERT( test::rewind_adapt( start, p ) == in.current() );
         in.template consume< void >( 4 );
         TAO_PEGTL_TEST_ASSERT( test::rewind_adapt( start, p ) == r );
         TAO_PEGTL_TEST_ASSERT( in.current() != r );
      }
   }

   template< typename Input >
   void test_01_view( Input& in )
   {
      in.private_set_end( in.current() + 5 );
      TAO_PEGTL_TEST_ASSERT( !in.empty() );
      TAO_PEGTL_TEST_ASSERT( in.size() == 5 );
      TAO_PEGTL_TEST_ASSERT( std::string( in.current(), in.size() ) == "dusel" );
      TAO_PEGTL_TEST_ASSERT( in.current( 3 ) == ( in.current() + 3 ) );
      TAO_PEGTL_TEST_ASSERT( in.end() == in.current() + 5 );
      TAO_PEGTL_TEST_ASSERT( *in.current() == 'd' );
      TAO_PEGTL_TEST_ASSERT( *in.current( 4 ) == 'l' );
   }

   void test_01_view()
   {
      {
         internal::view_input< char > in( data_01 );
         test_01( in );
      } {
         internal::view_input< char > in( data_01 );
         test_01_view( in );
      } {
         const std::string_view sv( data_01 );
         internal::view_input< char > in( sv );
         test_01( in );
      } {
         internal::view_input< char > in( data_01.data(), data_01.size() );
         test_01( in );
      } {
         internal::view_input< char > in( data_01.data(), data_01.data() + data_01.size() );
         test_01( in );
      } {
         internal::view_input< char > in( "duseltronik" );
         test_01( in );
      } {
         std::array< char, 11 > a = { 'd', 'u', 's', 'e', 'l', 't', 'r', 'o', 'n', 'i', 'k' };
         internal::view_input< char > in( a );
         test_01( in );
      }
   }

   template< typename Input >
   void test_01_copy( Input& in )
   {
      const auto* base = in.current();
      TAO_PEGTL_TEST_ASSERT( in.start() == in.current() );
      in.template consume< void >( 3 );
      TAO_PEGTL_TEST_ASSERT( in.start() + 3 == in.current() );
      in.restart();
      TAO_PEGTL_TEST_ASSERT( in.start() == in.current() );
      TAO_PEGTL_TEST_ASSERT( in.current() == base );
   }

   void test_01_copy()
   {
      {
         internal::copy_input< std::string > in( data_01 );
         test_01( in );
      } {
         internal::copy_input< std::string > in( data_01 );
         test_01_copy( in );
      } {
         std::string s( data_01 );
         internal::copy_input< std::string > in( std::move( s ) );
         test_01( in );
      } {
         internal::copy_input< std::string > in( data_01.data(), data_01.size() );
         test_01( in );
      } {
         internal::copy_input< std::string > in( data_01.data(), data_01.data() + data_01.size() );
         test_01( in );
      } {
         std::array< char, 11 > a = { 'd', 'u', 's', 'e', 'l', 't', 'r', 'o', 'n', 'i', 'k' };
         internal::copy_input< std::string > in( a );
         test_01( in );
      } {
         internal::copy_input< std::string > in( { 'd', 'u', 's', 'e', 'l', 't', 'r', 'o', 'n', 'i', 'k' } );
         test_01( in );
      }
   }

   void test_01_argv()
   {
      std::string a = "foo";
      std::string b = "duseltronik";
      std::string c = "bar";
      char* args[] = { a.data(), b.data(), c.data(), nullptr };
      internal::argv_input in( args, 1 );
      test_01( in );
   }

   const std::filesystem::path filename( "src/test/pegtl/data/duseltronik.txt" );

   void test_01_read()
   {
      {
         internal::read_input in( filename );
         test_01( in );
      } {
         internal::read_input in( filename );
         test_01_copy( in );
      }
   }

   void test_01_file()
   {
      {
         internal::file_input in( filename );
         test_01( in );
      } {
         internal::file_input in( filename );
         test_01_copy( in );
      }
   }

   void test_01_start()
   {
      {
         internal::input_with_start< internal::view_input< char > > in( data_01 );
         test_01( in );
      } {
         internal::input_with_start< internal::view_input< char > > in( data_01 );
         test_01_view( in );
      } {
         internal::input_with_start< internal::view_input< char > > in( data_01 );
         test_01_copy( in );
      }
   }

   template< typename Input >
   void test_01_peeks( Input& in )
   {
      in.template consume< void >( 1 );
      TAO_PEGTL_TEST_ASSERT( in.string_view() == "useltronik" );
      TAO_PEGTL_TEST_ASSERT( in.peek_char( 1 ) == 's' );
      TAO_PEGTL_TEST_ASSERT( in.peek_byte( 2 ) == std::byte( 'e' ) );
      TAO_PEGTL_TEST_ASSERT( in.peek_int8( 3 ) == std::int8_t( 'l' ) );
      TAO_PEGTL_TEST_ASSERT( in.peek_uint8( 4 ) == std::uint8_t( 't' ) );
   }

   void test_01_peeks()
   {
      {
         internal::input_with_funcs< internal::view_input< char > > in( data_01 );
         test_01( in );
      } {
         internal::input_with_funcs< internal::view_input< char > > in( data_01 );
         test_01_peeks( in );
      }
   }

   void unit_test()
   {
      test_01_view();
      test_01_copy();
      test_01_argv();
      test_01_read();
      test_01_file();
      test_01_start();
      test_01_peeks();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
