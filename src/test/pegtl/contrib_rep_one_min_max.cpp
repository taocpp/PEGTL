// Copyright (c) 2017-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/contrib/rep_one_min_max.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, true, false );

      verify_analyze< rep_one_min_max< 0, 1, '+' > >( __LINE__, __FILE__, false, false );
      verify_analyze< rep_one_min_max< 1, 1, '+' > >( __LINE__, __FILE__, true, false );

      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, ".", result_type::local_failure, 1 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "..", result_type::local_failure, 2 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "....", result_type::local_failure, 4 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "...", result_type::success, 0 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "... ", result_type::success, 1 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "...+", result_type::success, 1 );
      verify_rule< rep_one_min_max< 3, 3, '.' > >( __LINE__, __FILE__, "...a", result_type::success, 1 );

      verify_rule< rep_one_min_max< 0, 2, '+' > >( __LINE__, __FILE__, "", result_type::success, 0 );
      verify_rule< rep_one_min_max< 0, 2, '+' > >( __LINE__, __FILE__, "-", result_type::success, 1 );
      verify_rule< rep_one_min_max< 0, 2, '+' > >( __LINE__, __FILE__, "+-", result_type::success, 1 );
      verify_rule< rep_one_min_max< 0, 2, '+' > >( __LINE__, __FILE__, "++-", result_type::success, 1 );
      verify_rule< rep_one_min_max< 0, 2, '+' > >( __LINE__, __FILE__, "+++", result_type::local_failure, 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
