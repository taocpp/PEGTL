// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <iostream>

#include <tao/pegtl/contrib/uri.hpp>
#include <tao/pegtl/debug/print.hpp>

int main()  // NOLINT(bugprone-exception-escape)
{
   TAO_PEGTL_NAMESPACE::print_debug< TAO_PEGTL_NAMESPACE::uri::URI >( std::cout );
   return 0;
}

#endif
