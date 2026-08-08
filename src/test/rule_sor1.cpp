// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <string>

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/unicode/utf8.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   namespace test_sor1
   {
      struct a : one< 'a' >
      {
         using case_t = one< 'a' >;
      };

      struct b : one< 'b' >
      {
         using case_t = one< 'b' >;
      };

      struct ab : seq< one< 'a' >, one< 'b' > >
      {
         using case_t = one< 'a' >;
      };

      struct empty : success
      {
         using case_t = any;
      };

      struct fallback : any
      {
         using case_t = any;
      };

      struct euro : utf8::one< 0x20ac >
      {
         using case_t = utf8::ranges< 0x20ac >;
      };

      struct utf8_fallback : utf8::any
      {
         using case_t = utf8::any;
      };

      unsigned case_a_calls = 0;
      unsigned case_b_calls = 0;
      unsigned case_any_calls = 0;

      struct case_a
      {
         using peek_t = internal::peek_char;

         [[nodiscard]] static bool test( const char c ) noexcept
         {
            ++case_a_calls;
            return c == 'a';
         }
      };

      struct case_b
      {
         using peek_t = internal::peek_char;

         [[nodiscard]] static bool test( const char c ) noexcept
         {
            ++case_b_calls;
            return c == 'b';
         }
      };

      struct case_any
      {
         using peek_t = internal::peek_char;

         [[nodiscard]] static bool test( const char /*unused*/ ) noexcept
         {
            ++case_any_calls;
            return true;
         }
      };

      struct counted_a : one< 'a' >
      {
         using case_t = case_a;
      };

      struct counted_b : one< 'b' >
      {
         using case_t = case_b;
      };

      struct counted_fallback : any
      {
         using case_t = case_any;
      };

      struct counted_ab : seq< one< 'a' >, one< 'b' > >
      {
         using case_t = case_a;
      };

      template< typename Rule >
      struct selection_action
         : nothing< Rule >
      {};

      template<>
      struct selection_action< a >
      {
         static void apply0( std::string& selected )
         {
            selected += 'a';
         }
      };

      template<>
      struct selection_action< b >
      {
         static void apply0( std::string& selected )
         {
            selected += 'b';
         }
      };

      template<>
      struct selection_action< fallback >
      {
         static void apply0( std::string& selected )
         {
            selected += 'd';
         }
      };

      void reset_case_calls() noexcept
      {
         case_a_calls = 0;
         case_b_calls = 0;
         case_any_calls = 0;
      }

   }  // namespace test_sor1

   void unit_test()
   {
      using namespace test_sor1;

      verify_meta< sor1<>, internal::failure >();
      verify_meta< sor1< a >, internal::sor1< a >, a >();
      verify_meta< sor1< a, b >, internal::sor1< a, b >, a, b >();

      verify_ctrl_enabled< sor1<> >( __LINE__, __FILE__, "", false );
      verify_ctrl_disabled< internal::sor1<> >( __LINE__, __FILE__, "", false );
      verify_ctrl_enabled< sor1< a > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::sor1< a > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< sor1< a, b > >( __LINE__, __FILE__, "b" );
      verify_ctrl_disabled< internal::sor1< a, b > >( __LINE__, __FILE__, "b" );

      verify_analyze< sor1< a > >( __LINE__, __FILE__, true, false );
      verify_analyze< sor1< a, b > >( __LINE__, __FILE__, true, false );
      verify_analyze< sor1< empty > >( __LINE__, __FILE__, false, false );
      verify_analyze< star< sor1< empty > > >( __LINE__, __FILE__, false, true );

      verify_rule< sor1<> >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< sor1<> >( __LINE__, __FILE__, "a", result_type::local_failure, 1 );

      verify_rule< sor1< a > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< sor1< a > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< sor1< a > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< sor1< a > >( __LINE__, __FILE__, "b", result_type::local_failure, 1 );

      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "b", result_type::success, 0 );
      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "c", result_type::local_failure, 1 );
      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "ab", result_type::success, 1 );
      verify_rule< sor1< a, b > >( __LINE__, __FILE__, "ba", result_type::success, 1 );

      using with_default = sor1< a, b, fallback >;
      verify_rule< with_default >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< with_default >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< with_default >( __LINE__, __FILE__, "b", result_type::success, 0 );
      verify_rule< with_default >( __LINE__, __FILE__, "c", result_type::success, 0 );
      verify_rule< with_default >( __LINE__, __FILE__, "cd", result_type::success, 1 );

      using overlapping = sor1< ab, a, fallback >;
      verify_rule< overlapping >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< overlapping >( __LINE__, __FILE__, "a", result_type::local_failure, 1 );
      verify_rule< overlapping >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< overlapping >( __LINE__, __FILE__, "abc", result_type::success, 1 );
      verify_rule< overlapping >( __LINE__, __FILE__, "ac", result_type::local_failure, 2 );
      verify_rule< overlapping >( __LINE__, __FILE__, "b", result_type::success, 0 );

      {
         text_view_input< scan::lf > in( "ac" );
         TAO_PEGTL_TEST_ASSERT( !parse< overlapping >( in ) );
         TAO_PEGTL_TEST_ASSERT( in.size() == 1 );
      }

      {
         std::string selected;
         TAO_PEGTL_TEST_ASSERT( parse< with_default, selection_action >( text_view_input< scan::lf >( "a" ), selected ) );
         TAO_PEGTL_TEST_ASSERT( selected == "a" );
      }
      {
         std::string selected;
         TAO_PEGTL_TEST_ASSERT( parse< with_default, selection_action >( text_view_input< scan::lf >( "b" ), selected ) );
         TAO_PEGTL_TEST_ASSERT( selected == "b" );
      }
      {
         std::string selected;
         TAO_PEGTL_TEST_ASSERT( parse< with_default, selection_action >( text_view_input< scan::lf >( "c" ), selected ) );
         TAO_PEGTL_TEST_ASSERT( selected == "d" );
      }

      using counted = sor1< counted_a, counted_b, counted_fallback >;
      reset_case_calls();
      TAO_PEGTL_TEST_ASSERT( parse< counted >( text_view_input< scan::lf >( "a" ) ) );
      TAO_PEGTL_TEST_ASSERT( case_a_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_b_calls == 0 );
      TAO_PEGTL_TEST_ASSERT( case_any_calls == 0 );

      reset_case_calls();
      TAO_PEGTL_TEST_ASSERT( parse< counted >( text_view_input< scan::lf >( "b" ) ) );
      TAO_PEGTL_TEST_ASSERT( case_a_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_b_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_any_calls == 0 );

      reset_case_calls();
      TAO_PEGTL_TEST_ASSERT( parse< counted >( text_view_input< scan::lf >( "c" ) ) );
      TAO_PEGTL_TEST_ASSERT( case_a_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_b_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_any_calls == 1 );

      reset_case_calls();
      TAO_PEGTL_TEST_ASSERT( !parse< counted >( text_view_input< scan::lf >( "" ) ) );
      TAO_PEGTL_TEST_ASSERT( case_a_calls == 0 );
      TAO_PEGTL_TEST_ASSERT( case_b_calls == 0 );
      TAO_PEGTL_TEST_ASSERT( case_any_calls == 0 );

      using counted_overlapping = sor1< counted_ab, counted_fallback >;
      reset_case_calls();
      TAO_PEGTL_TEST_ASSERT( !parse< counted_overlapping >( text_view_input< scan::lf >( "ac" ) ) );
      TAO_PEGTL_TEST_ASSERT( case_a_calls == 1 );
      TAO_PEGTL_TEST_ASSERT( case_any_calls == 0 );

      using utf8_cases = sor1< euro, utf8_fallback >;
      verify_rule< utf8_cases, scan::lf_crlf, internal::peek_utf8 >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< utf8_cases, scan::lf_crlf, internal::peek_utf8 >( __LINE__, __FILE__, "\xe2\x82\xac", result_type::success, 0 );
      verify_rule< utf8_cases, scan::lf_crlf, internal::peek_utf8 >( __LINE__, __FILE__, "\xe2\x82\xac!", result_type::success, 1 );
      verify_rule< utf8_cases, scan::lf_crlf, internal::peek_utf8 >( __LINE__, __FILE__, "x", result_type::success, 0 );
      verify_rule< utf8_cases, scan::lf_crlf, internal::peek_utf8 >( __LINE__, __FILE__, "\xff", result_type::local_failure, 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
