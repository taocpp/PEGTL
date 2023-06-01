// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct my_action
   {};

   template<>
   struct my_action< eof >
   {
      static void apply0( bool& b )
      {
         b = true;
      }
   };

   void unit_test()
   {
      verify_meta< partial< eof >, internal::partial< eof >, eof >();
      verify_meta< partial< eof, any >, internal::partial< eof, any >, eof, any >();

      verify_analyze< partial< any > >( __LINE__, __FILE__, false, false );
      verify_analyze< partial< eof > >( __LINE__, __FILE__, false, false );

      verify_rule< partial< one< 'a' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< partial< one< 'a' > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< partial< one< 'a' > > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< partial< one< 'a' > > >( __LINE__, __FILE__, "ab", result_type::success, 1 );
      verify_rule< partial< one< 'a' > > >( __LINE__, __FILE__, "ba", result_type::success, 2 );

      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abab", result_type::success, 2 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bab", result_type::success, 3 );
      verify_rule< partial< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "cb", result_type::success, 2 );

#if defined( __cpp_exceptions )
      verify_rule< must< partial< one< 'a' > > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< must< partial< one< 'a' > > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< must< partial< one< 'a' > > > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< must< partial< one< 'a' > > > >( __LINE__, __FILE__, "ab", result_type::success, 1 );
      verify_rule< must< partial< one< 'a' > > > >( __LINE__, __FILE__, "ba", result_type::success, 2 );

      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "abab", result_type::success, 2 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "bab", result_type::success, 3 );
      verify_rule< must< partial< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "cb", result_type::success, 2 );
#endif

      bool success = false;
      TAO_PEGTL_TEST_ASSERT( parse< partial< eof >, my_action >( memory_input( "", __FUNCTION__ ), success ) );
      TAO_PEGTL_TEST_ASSERT( success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
