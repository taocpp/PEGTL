// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
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

#include "open_metrics_check.hpp"
#include "open_metrics_parse.hpp"

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      const auto metrics = open_metrics::parse( argv[ i ] );
      open_metrics::check( metrics );
   }
   return 0;
}

#endif
