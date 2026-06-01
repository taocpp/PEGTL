// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/example/fp.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using grammar = seq< fp::value, eof >;

   void unit_test()
   {
      verify_analyze< fp::value >( __LINE__, __FILE__, true, false );
      verify_analyze< grammar >( __LINE__, __FILE__, true, false );

      verify_rule< grammar >( __LINE__, __FILE__, "0", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "-0.5", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "+.5", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "1.", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "1e-2", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "0x1.fp+3", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "-Infinity", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "NaN", result_type::success );
      verify_rule< grammar >( __LINE__, __FILE__, "nan(payload)", result_type::success );

      verify_rule< grammar >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "+", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, ".", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "1e", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "0x", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "0x.p1", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "nan()", result_type::local_failure );
      verify_rule< grammar >( __LINE__, __FILE__, "infinityx", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
