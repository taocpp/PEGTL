// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
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
      verify_meta< opt<>, internal::opt<> >();
      verify_meta< opt< eof >, internal::opt< eof >, eof >();
      verify_meta< opt< eof, any >, internal::opt< eof, any >, eof, any >();

      verify_analyze< opt< any > >( __LINE__, __FILE__, false, false );
      verify_analyze< opt< eof > >( __LINE__, __FILE__, false, false );

      verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "ab", result_type::success, 1 );
      verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "ba", result_type::success, 2 );

      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abab", result_type::success, 2 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bab", result_type::success, 3 );
      verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "cb", result_type::success, 2 );

      verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "ab", result_type::success, 1 );
      verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "ba", result_type::success, 2 );

      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "abab", result_type::success, 2 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "bab", result_type::success, 3 );
      verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "cb", result_type::success, 2 );

      bool success = false;
      const bool result = parse< opt< eof >, my_action >( memory_input( "", __FUNCTION__ ), success );
      TAO_PEGTL_TEST_ASSERT( result );
      TAO_PEGTL_TEST_ASSERT( success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
