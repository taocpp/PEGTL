// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "test_utility.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

#include <tao/pegtl/example/json_pointer.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   using abs_pointer = seq< json_pointer::json_pointer, eof >;
   using rel_pointer = seq< json_pointer::relative_json_pointer, eof >;

   void unit_test()
   {
      verify_analyze< abs_pointer >( __LINE__, __FILE__, false, false );
      verify_analyze< rel_pointer >( __LINE__, __FILE__, true, false );

      verify_rule< abs_pointer >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< abs_pointer >( __LINE__, __FILE__, "/", result_type::success );
      verify_rule< abs_pointer >( __LINE__, __FILE__, "/foo/0", result_type::success );
      verify_rule< abs_pointer >( __LINE__, __FILE__, "/a~1b/c~0d", result_type::success );

      verify_rule< abs_pointer >( __LINE__, __FILE__, "foo", result_type::local_failure );
      verify_rule< abs_pointer >( __LINE__, __FILE__, "/foo~", result_type::local_failure );
      verify_rule< abs_pointer >( __LINE__, __FILE__, "/foo~2", result_type::local_failure );

      verify_rule< rel_pointer >( __LINE__, __FILE__, "0", result_type::success );
      verify_rule< rel_pointer >( __LINE__, __FILE__, "0#", result_type::success );
      verify_rule< rel_pointer >( __LINE__, __FILE__, "2/foo", result_type::success );
      verify_rule< rel_pointer >( __LINE__, __FILE__, "10/a~1b", result_type::success );

      verify_rule< rel_pointer >( __LINE__, __FILE__, "", result_type::local_failure );
      verify_rule< rel_pointer >( __LINE__, __FILE__, "01", result_type::local_failure );
      verify_rule< rel_pointer >( __LINE__, __FILE__, "1~", result_type::local_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
