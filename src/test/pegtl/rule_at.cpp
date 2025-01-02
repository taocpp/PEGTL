// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   int at_counter = 0;

   template< typename Rule >
   struct at_action
   {};

   template<>
   struct at_action< any >
   {
      static void apply0()
      {
         ++at_counter;
      }
   };

   void unit_test()
   {
      TAO_PEGTL_TEST_ASSERT( at_counter == 0 );

      verify_meta< at<>, internal::success >();
      verify_meta< at< eof >, internal::at< eof >, eof >();
      verify_meta< at< eof, any >, internal::at< internal::seq< eof, any > >, internal::seq< eof, any > >();

      verify_analyze< at< eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< at< any > >( __LINE__, __FILE__, false, false );

      verify_rule< at< eof > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< at< eof > >( __LINE__, __FILE__, "a", result_type::local_failure, 1 );
      verify_rule< at< any > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< at< any > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< at< any > >( __LINE__, __FILE__, "aa", result_type::success, 2 );
      verify_rule< at< any > >( __LINE__, __FILE__, "aaaa", result_type::success, 4 );

#if defined( __cpp_exceptions )
      verify_rule< must< at< alpha > > >( __LINE__, __FILE__, "1", result_type::global_failure, 1 );
      verify_rule< must< at< alpha, alpha > > >( __LINE__, __FILE__, "a1a", result_type::global_failure, 3 );
#endif

      {
         text_view_input< scan::lf > in( "f" );
         parse< any, at_action >( in );
         TAO_PEGTL_TEST_ASSERT( at_counter == 1 );
      }
      {
         text_view_input< scan::lf > in( "f" );
         parse< at< any >, at_action >( in );
         TAO_PEGTL_TEST_ASSERT( at_counter == 1 );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
