// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "unit_test_rule_impl.hh"

namespace pegtl
{
   void unit_test()
   {
      verify< shebang >( "", false, 0, __LINE__ );
      verify< shebang >( "#", false, 1, __LINE__ );
      verify< shebang >( "!", false, 1, __LINE__ );
      verify< shebang >( "!#", false, 2, __LINE__ );
      verify< shebang >( "#  ", false, 3, __LINE__ );
      verify< shebang >( "!  ", false, 3, __LINE__ );
      verify< shebang >( "## ", false, 3, __LINE__ );
      verify< shebang >( "!! ", false, 3, __LINE__ );
      verify< shebang >( "#!", true, 0, __LINE__ );
      verify< shebang >( "#! ", true, 0, __LINE__ );
      verify< shebang >( "#!/bin/bash", true, 0, __LINE__ );
      verify< shebang >( "#!/bin/bash\n", true, 0, __LINE__ );
      verify< shebang >( "#!/bin/bash\n#!/b", true, 4, __LINE__ );
      verify< shebang >( "#!\n", true, 0, __LINE__ );
      verify< shebang >( "#!\n ", true, 1, __LINE__ );
   }

} // pegtl

#include "unit_test_main.hh"
