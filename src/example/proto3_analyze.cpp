// Copyright (c) 2017-2026 Dr. Colin Hirsch and Daniel Frey
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

#include <tao/pegtl/debug/analyze.hpp>
#include <tao/pegtl/example/proto3.hpp>

int main()  // NOLINT(bugprone-exception-escape)
{
   using namespace TAO_PEGTL_NAMESPACE;

   if( const auto problems = analyze< proto3::proto >() != 0 ) {
      std::cout << "problems: " << problems << std::endl;
      return 1;
   }
   return 0;
}

#endif
