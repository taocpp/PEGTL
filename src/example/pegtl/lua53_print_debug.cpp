// Copyright (c) 2015-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iostream>

#include <tao/pegtl/contrib/print.hpp>

#include "lua53.hpp"

int main()  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_debug< lua53::grammar >( std::cout );
   return 0;
}

#endif
