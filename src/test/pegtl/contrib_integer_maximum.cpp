// Copyright (c) 2018-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <limits>
#include <sstream>
#include <utility>

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/contrib/integer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct int_action
      : nothing< Rule >
   {};

   template<>
   struct int_action< unsigned_rule >
      : maximum_action< unsigned, 42 >
   {};

   void test_overflow( const std::string& i )
   {
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         TAO_PEGTL_TEST_THROWS( parse< must< unsigned_rule, eof >, int_action >( in, st ) );
      }
      {
         text_view_input< scan::lf > in( i );
         TAO_PEGTL_TEST_THROWS( parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in ) );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         TAO_PEGTL_TEST_THROWS( parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in, st ) );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         TAO_PEGTL_TEST_THROWS( parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in, st, in ) );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         TAO_PEGTL_TEST_THROWS( parse< must< maximum_rule_with_action< unsigned, 42 >, eof > >( in, st ) );
      }
   }

   void test_unsigned( const std::string& i, const unsigned s )
   {
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< unsigned_rule, eof >, int_action >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
      {
         text_view_input< scan::lf > in( i );
         parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == 123 );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< maximum_rule_with_action< unsigned, 42 >, eof >, nothing, normal, apply_mode::nothing >( in, st, in );
         TAO_PEGTL_TEST_ASSERT( st == 123 );
      }
      {
         unsigned st = 123;
         text_view_input< scan::lf > in( i );
         parse< must< maximum_rule_with_action< unsigned, 42 >, eof > >( in, st );
         TAO_PEGTL_TEST_ASSERT( st == s );
      }
   }

   template< auto M >
   using max_seq_rule = seq< one< 'a' >, maximum_rule< std::uint64_t, M >, one< 'b' >, eof >;

   void unit_test()
   {
      test_overflow( "43" );
      test_overflow( "100" );

      test_unsigned( "0", 0 );
      test_unsigned( "1", 1 );
      test_unsigned( "42", 42 );

      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a1b", result_type::local_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a9b", result_type::local_failure );
      verify_rule< max_seq_rule< 0 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a1b", result_type::success );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a2b", result_type::local_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a9b", result_type::local_failure );
      verify_rule< max_seq_rule< 1 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a1b", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a2b", result_type::success );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a3b", result_type::local_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a9b", result_type::local_failure );
      verify_rule< max_seq_rule< 2 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a3b", result_type::success );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a4b", result_type::local_failure );
      verify_rule< max_seq_rule< 3 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 4 > >( __LINE__, __FILE__, "a5b", result_type::local_failure );
      verify_rule< max_seq_rule< 4 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a9b", result_type::success );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a10b", result_type::local_failure );
      verify_rule< max_seq_rule< 9 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );

      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "a9b", result_type::success );
      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "a10b", result_type::success );
      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "a11b", result_type::local_failure );
      verify_rule< max_seq_rule< 10 > >( __LINE__, __FILE__, "a19b", result_type::local_failure );

      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a9b", result_type::success );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a10b", result_type::success );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a11b", result_type::success );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a12b", result_type::local_failure );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a13b", result_type::local_failure );
      verify_rule< max_seq_rule< 11 > >( __LINE__, __FILE__, "a111b", result_type::local_failure );

      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a0b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a1b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a9b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a10b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a11b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a12b", result_type::success );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a13b", result_type::local_failure );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a19b", result_type::local_failure );
      verify_rule< max_seq_rule< 12 > >( __LINE__, __FILE__, "a111b", result_type::local_failure );

      verify_rule< max_seq_rule< 18446744073709551614ULL > >( __LINE__, __FILE__, "a18446744073709551614b", result_type::success );
      verify_rule< max_seq_rule< 18446744073709551614ULL > >( __LINE__, __FILE__, "a18446744073709551615b", result_type::local_failure );

      verify_rule< max_seq_rule< 18446744073709551615ULL > >( __LINE__, __FILE__, "a18446744073709551615b", result_type::success );
      verify_rule< max_seq_rule< 18446744073709551615ULL > >( __LINE__, __FILE__, "a18446744073709551616b", result_type::local_failure );
      verify_rule< max_seq_rule< 18446744073709551615ULL > >( __LINE__, __FILE__, "a98446744073709551614b", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
