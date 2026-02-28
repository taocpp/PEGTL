// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/extra/unordered.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using a = one< 'a' >;
   using b = one< 'b' >;
   using c = one< 'c' >;

   using ab = seq< a, b >;
   using ac = seq< a, c >;

   void unit_test()
   {
      verify_analyze< unordered< a, b > >( __LINE__, __FILE__, true, false );
      verify_analyze< unordered_partial< a, b > >( __LINE__, __FILE__, false, false );

      verify_rule< unordered< a > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< unordered< a > >( __LINE__, __FILE__, "a", result_type::success );
      verify_rule< unordered< a > >( __LINE__, __FILE__, "b", result_type::local_failure );
      verify_rule< unordered< a > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< unordered< a > >( __LINE__, __FILE__, "ab", result_type::success, 1 );

      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abc", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "bac", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "cab", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "cba", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "acb", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "bca", result_type::success );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abca", result_type::success, 1 );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abcabc", result_type::success, 3 );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "bcaabc", result_type::success, 3 );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abcdab", result_type::success, 3 );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "bcadabc", result_type::success, 4 );

      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "aab", result_type::success );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "aba", result_type::success );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "baa", result_type::success );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "aabb", result_type::success, 1 );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "ababa", result_type::success, 2 );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "baaaab", result_type::success, 3 );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "aabd", result_type::success, 1 );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "abada", result_type::success, 2 );
      verify_rule< unordered< a, a, b > >( __LINE__, __FILE__, "baadbaa", result_type::success, 4 );

      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "a", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "b", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "c", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "ac", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "bc", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "aba", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "abac", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "aabc", result_type::local_failure  );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "aaa", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "aab", result_type::local_failure  );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "aaab", result_type::local_failure );
      verify_rule< unordered< a, b, c > >( __LINE__, __FILE__, "cacbabc", result_type::local_failure  );

      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "abac", result_type::success );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "acab", result_type::success );

      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "ac", result_type::local_failure );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "abab", result_type::local_failure );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "acac", result_type::local_failure );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "abaa", result_type::local_failure );
      verify_rule< unordered< ab, ac > >( __LINE__, __FILE__, "aabac", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
