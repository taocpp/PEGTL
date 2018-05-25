// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
#include "verify_rule.hpp"

namespace tao
{
   namespace TAO_PEGTL_NAMESPACE
   {
      struct my_rule
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... > class Action,
                   template< typename... > class Control,
                   typename Input >
         static bool match( Input& /*unused*/, bool& v )
         {
            return v;
         }
      };

      template< typename Rule >
      struct my_action : nothing< Rule >
      {
      };

      template<>
      struct my_action< eof >
      {
         static void apply0( bool& v )
         {
            v = true;
         }
      };

      void unit_test()
      {
         verify_analyze< until< eof > >( __LINE__, __FILE__, false, false );
         verify_analyze< until< any > >( __LINE__, __FILE__, true, false );
         verify_analyze< until< eof, any > >( __LINE__, __FILE__, false, false );
         verify_analyze< until< any, any > >( __LINE__, __FILE__, true, false );

         verify_rule< until< eof > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< until< any > >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "bba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "bab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "bbab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' > > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbbab", result_type::SUCCESS, 1 );

         verify_rule< must< until< one< 'a' > > > >( __LINE__, __FILE__, "bbb", result_type::GLOBAL_FAILURE, 0 );

         verify_rule< try_catch< must< until< one< 'a' > > > > >( __LINE__, __FILE__, "bbb", result_type::LOCAL_FAILURE, 3 );

         verify_rule< until< eof, any > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< until< any, any > >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "bba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "bab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "bbab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, any > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbbab", result_type::SUCCESS, 1 );

         verify_rule< until< eof, one< 'a' > > >( __LINE__, __FILE__, "", result_type::SUCCESS, 0 );
         verify_rule< until< eof, one< 'a' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< until< eof, one< 'a' > > >( __LINE__, __FILE__, "aa", result_type::SUCCESS, 0 );
         verify_rule< until< eof, one< 'a' > > >( __LINE__, __FILE__, "aaaaab", result_type::LOCAL_FAILURE, 6 );
         verify_rule< until< eof, one< 'a' > > >( __LINE__, __FILE__, "baaaaa", result_type::LOCAL_FAILURE, 6 );

         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aa", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "b", result_type::LOCAL_FAILURE, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bb", result_type::LOCAL_FAILURE, 2 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bbbbbbbbbbbbbba", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "baa", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bbaa", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbaa", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bbab", result_type::SUCCESS, 1 );
         verify_rule< until< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "bbbbbbbbbbbbbbab", result_type::SUCCESS, 1 );

         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "", result_type::LOCAL_FAILURE, 0 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "a", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bca", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bcbca", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bcbcbcbcbca", result_type::SUCCESS, 0 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "babca", result_type::LOCAL_FAILURE, 5 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bcbcb", result_type::LOCAL_FAILURE, 5 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "cbcbc", result_type::LOCAL_FAILURE, 5 );
         verify_rule< until< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "bcbcbc", result_type::LOCAL_FAILURE, 6 );

         verify_rule< must< until< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "bbb", result_type::GLOBAL_FAILURE, 0 );
         verify_rule< must< until< one< 'a' >, one< 'b' > > > >( __LINE__, __FILE__, "bbbc", result_type::GLOBAL_FAILURE, 1 );

         verify_rule< try_catch< must< until< one< 'a' >, one< 'b' > > > > >( __LINE__, __FILE__, "bbb", result_type::LOCAL_FAILURE, 3 );
         verify_rule< try_catch< must< until< one< 'a' >, one< 'b' > > > > >( __LINE__, __FILE__, "bbbc", result_type::LOCAL_FAILURE, 4 );

         bool success = false;
         const bool result = parse< until< my_rule, eof >, my_action >( memory_input<>( "", __FUNCTION__ ), success );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( success );
      }

   }  // namespace TAO_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
