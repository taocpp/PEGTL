// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/internal/char_string.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< internal::char_string<>, internal::success >();
      verify_meta< internal::char_string< 'a' >, internal::single< internal::one< internal::peek_char, 'a' > > >();
      verify_meta< internal::char_string< 'a', 'b' >, internal::char_string< 'a', 'b' > >();
      verify_meta< internal::char_string< '\0', '\n' >, internal::char_string< '\0', '\n' > >();

      verify_analyze< internal::char_string<> >( __LINE__, __FILE__, false, false );
      verify_analyze< internal::char_string< 'a' > >( __LINE__, __FILE__, true, false );
      verify_analyze< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, true, false );

      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "a", result_type::local_failure );
      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "aa", result_type::local_failure );
      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "aab", result_type::local_failure );

      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "ab", result_type::success );
      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "abb", result_type::success, 1 );
      verify_rule< internal::char_string< 'a', 'b' > >( __LINE__, __FILE__, "abab", result_type::success, 2 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
