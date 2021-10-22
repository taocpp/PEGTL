// Copyright (c) 2020-2021 Dr. Colin Hirsch and Daniel Frey
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

#include <tao/pegtl/contrib/print.hpp>
#include <tao/pegtl/contrib/uri.hpp>

int main()  // NOLINT(bugprone-exception-escape)
{
   tao::pegtl::print_names< tao::pegtl::uri::URI >( std::cout );
   return 0;
}

#endif
