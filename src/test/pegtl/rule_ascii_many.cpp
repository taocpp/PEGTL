// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
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
      verify_meta< many7< 0 >, internal::success >();
      verify_meta< many7< 1 >, internal::any< internal::peek_seven > >();
      verify_meta< many7< 42 >, internal::many< 42, internal::peek_seven > >();

      verify_meta< many< 0 >, internal::success >();
      verify_meta< many< 1 >, internal::any< internal::peek_char > >();
      verify_meta< many< 42 >, internal::many< 42, internal::peek_char > >();

      verify_analyze< many7< 0 > >( __LINE__, __FILE__, false, false );
      verify_analyze< many< 0 > >( __LINE__, __FILE__, false, false );

      verify_rule< many7< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< many7< 0 > >( __LINE__, __FILE__, "a", result_type::success, 1 );
      verify_rule< many< 0 > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< many< 0 > >( __LINE__, __FILE__, "a", result_type::success, 1 );

      verify_analyze< many7< 1 > >( __LINE__, __FILE__, true, false );
      verify_analyze< many< 1 > >( __LINE__, __FILE__, true, false );

      for( char c = 0; c < 127; ++c ) {
         verify_char< many7< 1 > >( __LINE__, __FILE__, c, result_type::success );
         verify_char< many< 1 > >( __LINE__, __FILE__, c, result_type::success );
      }
      verify_rule< many7< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< many7< 1 > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< many< 1 > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< many< 1 > >( __LINE__, __FILE__, "aa", result_type::success, 1 );

      verify_analyze< many7< 2 > >( __LINE__, __FILE__, true, false );
      verify_analyze< many7< 42 > >( __LINE__, __FILE__, true, false );
      verify_analyze< many< 2 > >( __LINE__, __FILE__, true, false );
      verify_analyze< many< 42 > >( __LINE__, __FILE__, true, false );

      verify_rule< many7< 3 > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
      verify_rule< many7< 4 > >( __LINE__, __FILE__, "abcd", result_type::success, 0 );
      verify_rule< many7< 5 > >( __LINE__, __FILE__, "abcd", result_type::local_failure, 4 );

      verify_rule< many< 3 > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
      verify_rule< many< 4 > >( __LINE__, __FILE__, "abcd", result_type::success, 0 );
      verify_rule< many< 5 > >( __LINE__, __FILE__, "abcd", result_type::local_failure, 4 );

      verify_rule< many7< 4 > >( __LINE__, __FILE__, "abcdefghij", result_type::success, 6 );
      verify_rule< many< 4 > >( __LINE__, __FILE__, "abcdefghij", result_type::success, 6 );

      verify_char< many7< 1 > >( __LINE__, __FILE__, char( 200 ), result_type::local_failure );
      verify_char< many< 1 > >( __LINE__, __FILE__, char( 200 ), result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
