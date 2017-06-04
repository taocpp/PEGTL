// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace test1
      {
         struct fiz : if_must< at< one< 'a' > >, two< 'a' > >
         {
         };

         struct foo : sor< fiz, one< 'b' > >
         {
         };

         struct bar : until< eof, foo >
         {
         };

         void test_result()
         {
            TAOCPP_PEGTL_TEST_ASSERT( applied.size() == 10 );

            TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].first == internal::demangle< one< 'b' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 1 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 2 ].first == internal::demangle< at< one< 'a' > > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 3 ].first == internal::demangle< two< 'a' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 4 ].first == internal::demangle< fiz >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 5 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 6 ].first == internal::demangle< one< 'b' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 7 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 8 ].first == internal::demangle< eof >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 9 ].first == internal::demangle< bar >() );

            TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 1 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 2 ].second == "" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 3 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 4 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 5 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 6 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 7 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 8 ].second == "" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 9 ].second == "baab" );
         }

         struct state1
         {
            char c;

            template< typename Input >
            state1( const Input& /*unused*/, std::string& /*unused*/ )
            {
            }

            template< typename Input >
            void success( const Input& /*unused*/, std::string& s ) const
            {
               s += c;
            }
         };

         struct fobble : sor< state< state1, alpha >, digit >
         {
         };

         struct fibble : until< eof, fobble >
         {
         };

         template< typename Rule >
         struct action1 : nothing< Rule >
         {
         };

         template<>
         struct action1< alpha >
         {
            template< typename Input >
            static void apply( const Input& in, state1& s )
            {
               assert( in.size() == 1 );
               s.c = in.begin()[ 0 ];
            }
         };

         void state_test()
         {
            std::string result;
            memory_input<> in( "dk41sk41xk3", __FUNCTION__ );
            parse< fibble, action1 >( in, result );
            TAOCPP_PEGTL_TEST_ASSERT( result == "dkskxk" );
         }

         template< typename Rule >
         struct action0 : nothing< Rule >
         {
         };

         static int i0 = 0;

         template<>
         struct action0< alpha >
         {
            static void apply0()
            {
               ++i0;
            }
         };

         template<>
         struct action0< digit >
         {
            static void apply0( std::string& s )
            {
               s += '0';
            }
         };

         void apply0_test()
         {
            memory_input<> ina( "abcdefgh", __FUNCTION__ );
            parse< star< alpha >, action0 >( ina );
            TAOCPP_PEGTL_TEST_ASSERT( i0 == 8 );
            std::string s0;
            memory_input<> ind( "12345678", __FUNCTION__ );
            parse< star< digit >, action0 >( ind, s0 );
            TAOCPP_PEGTL_TEST_ASSERT( s0 == "00000000" );
         }

         const std::size_t count_byte = 12345;
         const std::size_t count_line = 42;
         const std::size_t count_byte_in_line = 12;

         const char* count_source = "count_source";

         template< typename Rule >
         struct count_action
         {
            template< typename Input >
            static void apply( const Input& in )
            {
               TAOCPP_PEGTL_TEST_ASSERT( in.iterator().byte == count_byte );
               TAOCPP_PEGTL_TEST_ASSERT( in.iterator().line == count_line );
               TAOCPP_PEGTL_TEST_ASSERT( in.iterator().byte_in_line == count_byte_in_line );
               TAOCPP_PEGTL_TEST_ASSERT( in.input().source() == count_source );
               TAOCPP_PEGTL_TEST_ASSERT( in.size() == 1 );
               TAOCPP_PEGTL_TEST_ASSERT( in.begin() + 1 == in.end() );
               TAOCPP_PEGTL_TEST_ASSERT( in.peek_char() == 'f' );
               TAOCPP_PEGTL_TEST_ASSERT( in.peek_byte() == static_cast< unsigned char >( 'f' ) );
            }
         };

         void count_test()
         {
            const char* foo = "f";
            memory_input<> in( foo, foo + 1, count_source, count_byte, count_line, count_byte_in_line );
            const auto result = parse< must< alpha >, count_action >( in );
            TAOCPP_PEGTL_TEST_ASSERT( result );
         }

      }  // namespace test1

      void unit_test()
      {
         parse< disable< test1::bar >, test_action >( memory_input<>( "baab", __FUNCTION__ ) );
         TAOCPP_PEGTL_TEST_ASSERT( applied.size() == 1 );

         TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].first == internal::demangle< disable< test1::bar > >() );
         TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].second == "baab" );

         applied.clear();

         parse< at< action< test_action, test1::bar > > >( memory_input<>( "baab", __FUNCTION__ ) );

         TAOCPP_PEGTL_TEST_ASSERT( applied.empty() );

         applied.clear();

         parse< test1::bar, test_action >( memory_input<>( "baab", __FUNCTION__ ) );

         test1::test_result();

         applied.clear();

         parse< action< test_action, test1::bar > >( memory_input<>( "baab", __FUNCTION__ ) );

         test1::test_result();

         applied.clear();

         parse< disable< enable< action< test_action, test1::bar > > > >( memory_input<>( "baab", __FUNCTION__ ) );

         test1::test_result();

         test1::state_test();
         test1::apply0_test();
         test1::count_test();
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
