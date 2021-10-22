// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< shebang >( __LINE__, __FILE__, true, false );

      verify_rule< shebang >( __LINE__, __FILE__, "", result_type::local_failure, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#", result_type::local_failure, 1 );
      verify_rule< shebang >( __LINE__, __FILE__, "!", result_type::local_failure, 1 );
      verify_rule< shebang >( __LINE__, __FILE__, "!#", result_type::local_failure, 2 );
      verify_rule< shebang >( __LINE__, __FILE__, "#  ", result_type::local_failure, 3 );
      verify_rule< shebang >( __LINE__, __FILE__, "!  ", result_type::local_failure, 3 );
      verify_rule< shebang >( __LINE__, __FILE__, "## ", result_type::local_failure, 3 );
      verify_rule< shebang >( __LINE__, __FILE__, "!! ", result_type::local_failure, 3 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!", result_type::success, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#! ", result_type::success, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!/bin/bash", result_type::success, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!/bin/bash\n", result_type::success, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!/bin/bash\n#!/b", result_type::success, 4 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!\n", result_type::success, 0 );
      verify_rule< shebang >( __LINE__, __FILE__, "#!\n ", result_type::success, 1 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
