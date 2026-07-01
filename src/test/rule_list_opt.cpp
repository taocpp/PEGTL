// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< list_opt< alpha, digit >, internal::opt< internal::seq< alpha, internal::star< digit, alpha > > >, internal::seq< alpha, internal::star< digit, alpha > > >();
      verify_meta< list_opt< alpha, digit, blank >, internal::opt< internal::seq< alpha, internal::star< internal::pad< digit, blank >, alpha > > >, internal::seq< alpha, internal::star< internal::pad< digit, blank >, alpha > > >();

      verify_ctrl_enabled< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a" );

      verify_analyze< list_opt< eof, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< eof, any > >( __LINE__, __FILE__, false, false );
      verify_analyze< list_opt< any, eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< list_opt< any, any > >( __LINE__, __FILE__, false, false );

      verify_analyze< list_opt< eof, eof, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< eof, eof, any > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< eof, any, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< eof, any, any > >( __LINE__, __FILE__, false, false );
      verify_analyze< list_opt< any, eof, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< any, eof, any > >( __LINE__, __FILE__, false, false );
      verify_analyze< list_opt< any, any, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< list_opt< any, any, any > >( __LINE__, __FILE__, false, false );

      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "b", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, ",", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, ",a", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,b", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a,a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a,a,a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a,a,b", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a,a,a,,", result_type::success, 2 );

      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a ", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, " a", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a ,a", result_type::success, 3 );
      verify_rule< list_opt< one< 'a' >, one< ',' > > >( __LINE__, __FILE__, "a, a", result_type::success, 3 );

      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, " ", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, ",", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "a ", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, " a", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "a ,a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "a, a", result_type::success, 0 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "a, a,", result_type::success, 1 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, "a, a ,", result_type::success, 2 );
      verify_rule< list_opt< one< 'a' >, one< ',' >, blank > >( __LINE__, __FILE__, " a , a ", result_type::success, 7 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
