// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< consume< 0 >, internal::success >();
      verify_meta< consume< 1 >, internal::consume< 1, void, void > >();
      verify_meta< consume< 42 >, internal::consume< 42, void, void > >();

      verify_analyze< consume< 0 > >( __LINE__, __FILE__, false, false );
      verify_analyze< consume< 1 > >( __LINE__, __FILE__, true, false );
      verify_analyze< consume< 42 > >( __LINE__, __FILE__, true, false );

      verify_rule< consume< 3 > >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< consume< 3 > >( __LINE__, __FILE__, "a", result_type::local_failure );
      verify_rule< consume< 3 > >( __LINE__, __FILE__, "ab", result_type::local_failure );
      verify_rule< consume< 3 > >( __LINE__, __FILE__, "abc", result_type::success );
      verify_rule< consume< 3 > >( __LINE__, __FILE__, "abcd", result_type::success, 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
