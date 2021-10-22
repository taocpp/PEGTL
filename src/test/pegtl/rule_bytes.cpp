// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_char.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< bytes< 0 >, internal::success >();
      verify_meta< bytes< 1 >, internal::bytes< 1 > >();
      verify_meta< bytes< 42 >, internal::bytes< 42 > >();

      verify_analyze< bytes< 0 > >( __LINE__, __FILE__, false, false );

      verify_rule< bytes< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< bytes< 0 > >( __LINE__, __FILE__, "a", result_type::success, 1 );

      verify_analyze< bytes< 1 > >( __LINE__, __FILE__, true, false );

      for( char c = 0; c < 127; ++c ) {
         verify_char< bytes< 1 > >( __LINE__, __FILE__, c, result_type::success );
      }
      verify_rule< bytes< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< bytes< 1 > >( __LINE__, __FILE__, "aa", result_type::success, 1 );

      verify_analyze< bytes< 2 > >( __LINE__, __FILE__, true, false );
      verify_analyze< bytes< 42 > >( __LINE__, __FILE__, true, false );

      verify_rule< bytes< 3 > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
      verify_rule< bytes< 4 > >( __LINE__, __FILE__, "abcd", result_type::success, 0 );
      verify_rule< bytes< 5 > >( __LINE__, __FILE__, "abcd", result_type::local_failure, 4 );

      verify_rule< bytes< 4 > >( __LINE__, __FILE__, "abcdefghij", result_type::success, 6 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
