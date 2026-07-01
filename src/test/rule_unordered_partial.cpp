// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   using a = one< 'a' >;
   using b = one< 'b' >;
   using c = one< 'c' >;

   using ab = seq< a, b >;
   using ac = seq< a, c >;

   void unit_test()
   {
      verify_ctrl_enabled< unordered_partial< a > >( __LINE__, __FILE__, "a" );
      verify_ctrl_disabled< internal::unordered< true, a > >( __LINE__, __FILE__, "a" );
      verify_ctrl_enabled< unordered_partial< a, b, c > >( __LINE__, __FILE__, "abc" );
      verify_ctrl_disabled< internal::unordered< true, a, b, c > >( __LINE__, __FILE__, "abc" );

      verify_analyze< unordered_partial< a, b > >( __LINE__, __FILE__, false, false );

      verify_rule< unordered_partial<> >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< unordered_partial<> >( __LINE__, __FILE__, "a", result_type::success, 1 );

      verify_rule< unordered_partial< a > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< unordered_partial< a > >( __LINE__, __FILE__, "a", result_type::success );
      verify_rule< unordered_partial< a > >( __LINE__, __FILE__, "b", result_type::success, 1 );
      verify_rule< unordered_partial< a > >( __LINE__, __FILE__, "aa", result_type::success, 1 );
      verify_rule< unordered_partial< a > >( __LINE__, __FILE__, "ab", result_type::success, 1 );

      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "a", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "b", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "c", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "ab", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "bc", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "ca", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "abc", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "bca", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "cba", result_type::success );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "dabc", result_type::success, 4 );
      verify_rule< unordered_partial< a, b, c > >( __LINE__, __FILE__, "aba", result_type::success, 1 );

      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "a", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "aa", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "ab", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "aab", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "ba", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "baa", result_type::success );
      verify_rule< unordered_partial< a, a, b > >( __LINE__, __FILE__, "baaa", result_type::success, 1 );

      verify_rule< unordered_partial< ab, ac > >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< unordered_partial< ab, ac > >( __LINE__, __FILE__, "ab", result_type::success );
      verify_rule< unordered_partial< ab, ac > >( __LINE__, __FILE__, "ac", result_type::success );
      verify_rule< unordered_partial< ab, ac > >( __LINE__, __FILE__, "abac", result_type::success );
      verify_rule< unordered_partial< ab, ac > >( __LINE__, __FILE__, "acab", result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
