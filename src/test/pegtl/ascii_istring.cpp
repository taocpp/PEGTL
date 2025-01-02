// Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< istring<>, internal::success >();
      verify_meta< istring< 'a' >, internal::tester< internal::ione< internal::peek_char, 'a' > > >();
      verify_meta< istring< 'a', 'b' >, internal::ascii_istring< 'a', 'b' > >();
      verify_meta< istring< '\0', '\n' >, internal::ascii_istring< '\0', '\n' > >();

      verify_analyze< istring<> >( __LINE__, __FILE__, false, false );
      verify_analyze< istring< 1 > >( __LINE__, __FILE__, true, false );
      verify_analyze< istring< 1, 2 > >( __LINE__, __FILE__, true, false );
      verify_analyze< istring< 1, 2, 3, 4 > >( __LINE__, __FILE__, true, false );
      verify_analyze< istring< 1, 2, 3, 4, 5, 6, 7 > >( __LINE__, __FILE__, true, false );

      verify_rule< istring<> >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "a", result_type::local_failure, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "b", result_type::local_failure, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "c", result_type::local_failure, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aa", result_type::local_failure, 2 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aB", result_type::success, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "AB", result_type::success, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "Ab", result_type::success, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "ac", result_type::local_failure, 2 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "ba", result_type::local_failure, 2 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "bb", result_type::local_failure, 2 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aab", result_type::local_failure, 3 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aab", result_type::local_failure, 3 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aba", result_type::success, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "Abb", result_type::success, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "abc", result_type::success, 1 );
      verify_rule< istring< 'a', 'b' > >( __LINE__, __FILE__, "aBab", result_type::success, 2 );
      verify_rule< istring< 'a', '0' > >( __LINE__, __FILE__, "a0A0", result_type::success, 2 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
