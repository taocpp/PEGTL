// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< star_strict< alpha >, internal::star_strict< alpha >, alpha >();
      verify_meta< star_strict< alpha, digit >, internal::star_strict< alpha, digit >, alpha, digit >();

      verify_analyze< star_strict< eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< star_strict< any > >( __LINE__, __FILE__, false, false );
      verify_analyze< star_strict< eof, eof, eof > >( __LINE__, __FILE__, false, true );
      verify_analyze< star_strict< any, eof, any > >( __LINE__, __FILE__, false, false );

      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "a", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "aa", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "aaa", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "aaab", result_type::success, 1 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "ba", result_type::success, 2 );
      verify_rule< star_strict< one< 'a' > > >( __LINE__, __FILE__, "b", result_type::success, 1 );

      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "a", result_type::local_failure );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "aba", result_type::local_failure );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abac", result_type::local_failure );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abb", result_type::success, 1 );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "abab", result_type::success, 0 );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ababc", result_type::success, 1 );
      verify_rule< star_strict< one< 'a' >, one< 'b' > > >( __LINE__, __FILE__, "ababab", result_type::success, 0 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
