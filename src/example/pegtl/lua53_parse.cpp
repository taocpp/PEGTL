// Copyright (c) 2015-2021 Dr. Colin Hirsch and Daniel Frey
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

#include "lua53.hpp"

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      TAO_PEGTL_NAMESPACE::file_input in( argv[ i ] );
      const auto r = TAO_PEGTL_NAMESPACE::parse< lua53::grammar >( in );
      std::cout << argv[ i ] << " " << r << std::endl;
   }
   return 0;
}

#endif
