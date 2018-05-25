// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
#include "verify_rule.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      template< typename Rule >
      struct my_action : nothing< Rule >
      {
      };

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
         verify_analyze< opt< any > >( __LINE__, __FILE__, false, false );
         verify_analyze< opt< eof > >( __LINE__, __FILE__, false, false );

         verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "aa", result_type::SUCCESS, 1 );
         verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 1 );
         verify_rule< opt< one< 'a' > > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 2 );

         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 1 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 0 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::SUCCESS, 1 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abab", result_type::SUCCESS, 2 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bab", result_type::SUCCESS, 3 );
         verify_rule< opt< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "cb", result_type::SUCCESS, 2 );

         verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "aa", result_type::SUCCESS, 1 );
         verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 1 );
         verify_rule< must< opt< one< 'a' > > > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 2 );

         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 1 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 0 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "aba", result_type::SUCCESS, 1 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "abab", result_type::SUCCESS, 2 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "bab", result_type::SUCCESS, 3 );
         verify_rule< must< opt< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "cb", result_type::SUCCESS, 2 );

         bool success = false;
         const bool result = parse< opt< eof >, my_action >( memory_input<>( "", __FUNCTION__ ), success );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( success );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
