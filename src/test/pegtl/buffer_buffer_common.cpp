// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/buffer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Buffer, typename... As >
   void test_common( As&&... as )
   {
      const std::string st = "abcdefghijklmnopqrstuvwxyz";

      internal::buffer_common< Buffer > bc( std::forward< As >( as )..., st );

      const auto* start = bc.buffer_start();

      static_assert( std::is_same_v< char, typename Buffer::data_t > );
      static_assert( std::is_same_v< char, typename internal::buffer_common< Buffer >::data_t > );

      TAO_PEGTL_TEST_ASSERT( bc.buffer_capacity() == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_chunk_size() == 20 );

      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 10 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 80 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      TAO_PEGTL_TEST_ASSERT( test::endless( st, 0, 20 ) == std::string( bc.current(), bc.buffer_used_size() ) );

      bc.discard();

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 10 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 80 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      bc.require( 10 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 10 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 80 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 20 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      bc.require( 90 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 90 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 90 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 90 ) == 90 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 90 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 10 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 10 ) == bc.current() + 90 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 3, 10 ) == bc.current() + 90 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      bc.require( 100 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 100 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 100 ) == bc.current() + 100 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 99, 100 ) == bc.current() + 100 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      TAO_PEGTL_TEST_ASSERT( test::endless( st, 0, 100 ) == std::string( bc.current(), bc.buffer_used_size() ) );

      bc.discard();

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 100 ) == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 100 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 100 ) == bc.current() + 100 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 99, 100 ) == bc.current() + 100 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      bc.consume( 60 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 10 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 60 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 40 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 40 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() + 60 );

      TAO_PEGTL_TEST_ASSERT( test::endless( st, 60, 40 ) == std::string( bc.current(), bc.buffer_used_size() ) );

      bc.discard();

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 20 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 40 ) == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 40 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 0 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 60 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 10 ) == bc.current() + 40 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 3, 10 ) == bc.current() + 40 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() );

      bc.consume( 30 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 10 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 10 ) == 10 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 10 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 30 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 60 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 10 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 10 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() + 30 );

      TAO_PEGTL_TEST_ASSERT( test::endless( st, 90, 10 ) == std::string( bc.current(), bc.buffer_used_size() ) );

      bc.require( 50 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.require( 101 ) );
      TAO_PEGTL_TEST_THROWS( (void)bc.end( 100, 101 ) );
#endif

      TAO_PEGTL_TEST_ASSERT( !bc.empty() );
      TAO_PEGTL_TEST_ASSERT( bc.size( 1 ) == 50 );
      TAO_PEGTL_TEST_ASSERT( bc.size( 50 ) == 50 );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == start );
      TAO_PEGTL_TEST_ASSERT( bc.mutable_start() == bc.buffer_start() );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_used_size() == 50 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_before_current() == 30 );
      TAO_PEGTL_TEST_ASSERT( bc.buffer_free_after_end() == 20 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 1 ) == bc.current() + 50 );
      TAO_PEGTL_TEST_ASSERT( bc.end( 0, 1 ) == bc.current() + 50 );
      TAO_PEGTL_TEST_ASSERT( bc.current() == bc.buffer_start() + 30 );

      bc.buffer_check_size( 50 );

#if defined( __cpp_exceptions )
      TAO_PEGTL_TEST_THROWS( bc.buffer_check_size( 51 ) );
#endif
   }

   void unit_test()
   {
      test_common< internal::dynamic_buffer< char, internal::endless_reader > >( 100, 20 );
      test_common< internal::static_buffer< char, internal::endless_reader, 100, 20 > >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
